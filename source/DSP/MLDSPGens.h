// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#pragma once

#include "MLDSPOps.h"
#include "MLDSPUtils.h"

namespace ml
{

// ----------------------------------------------------------------
// Block generation objects, used by Gens

template<typename T, typename Derived>
struct Gen0
{
  Block<T> operator()()
  {
    Block<T> output;
    for (size_t t = 0; t < kFramesPerBlock; ++t)
    {
      output[t] = static_cast<Derived*>(this)->nextFrame();
    }
    return output;
  }
};

template<typename T, typename Derived>
struct Gen1
{
  Block<T> operator()(const Block<T>& params)
  {
    Block<T> output;
    for (size_t t = 0; t < kFramesPerBlock; ++t)
    {
      output[t] = static_cast<Derived*>(this)->nextFrame(params[t]);
    }
    return output;
  }
};

// ----------------------------------------------------------------
// Frame-level waveform shaping helpers

// bandlimited step correction
template<typename T>
T polyBLEPSample(T phase, T dt)
{
  if constexpr (std::is_same_v<T, float>)
  {
    float c{0.f};
    if (phase < dt)
    {
      float t = phase / dt;
      c = t + t - t * t - 1.0f;
    }
    else if (phase > 1.0f - dt)
    {
      float t = (phase - 1.0f) / dt;
      c = t * t + t + t + 1.0f;
    }
    return c;
  }
  else
  {
    T rdt = rcp(dt);
    
    T t1 = phase * rdt;
    T blep1 = t1 + t1 - t1 * t1 - T{1.0f};
    T mask1 = (phase < dt);
    
    T t2 = (phase - T{1.0f}) * rdt;
    T blep2 = t2 * t2 + t2 + t2 + T{1.0f};
    T mask2 = (phase > T{1.0f} - dt);
    
    return andBits(mask1, blep1) + andBits(mask2, blep2);
  }
}

// phasor on (0, 1) to sine approximation using folded cubic.
// 3rd harmonic at about -40dB, odd harmonics only.
template<typename T>
T phasorToSineSample(T phasor)
{
  constexpr float sqrt2 = 1.4142135623730950488f;
  constexpr float domain = sqrt2 * 4.f;
  constexpr float range = sqrt2 - sqrt2 * sqrt2 * sqrt2 / 6.f;
  
  T omega = phasor * T{domain} - T{sqrt2};
  T centered = omega - T{sqrt2};
  T triangle = T{sqrt2} - max(centered, -centered);
  return T{1.f / range} * triangle * (T{1.f} - triangle * triangle * T{1.f / 6.f});
}

// phasor on (0, 1) to antialiased pulse
template<typename T>
T phasorToPulseSample(T phase, T freq, T pulseWidth)
{
  if constexpr (std::is_same_v<T, float>)
  {
    float pulse = (phase >= pulseWidth) ? 1.0f : -1.0f;
    pulse += polyBLEPSample<T>(phase, freq);
    float downPhase = phase - pulseWidth + 1.0f;
    downPhase = downPhase - intPart(downPhase);
    pulse -= polyBLEPSample<T>(downPhase, freq);
    return pulse;
  }
  else
  {
    T pulse = select(T{1.f}, T{-1.f}, phase >= pulseWidth);
    pulse = pulse + polyBLEPSample<T>(phase, freq);
    T downPhase = fracPart(phase - pulseWidth + T{1.0f});
    pulse = pulse - polyBLEPSample<T>(downPhase, freq);
    return pulse;
  }
}

// phasor on (0, 1) to antialiased saw on (-1, 1)
template<typename T>
T phasorToSawSample(T phase, T freq)
{
  T saw = phase * T{2.f} - T{1.f};
  return saw - polyBLEPSample<T>(phase, freq);
}

// ----------------------------------------------------------------
// Frame counter generator

template<typename T>
struct Counter : Gen0<T, Counter<T>>
{
  T state_{0.f};
  
  void clear() { state_ = T{0.f}; }
  
  T nextFrame() {
    T currentValue = state_;
    state_ += T{1.0f};
    return currentValue;
  }
};

// ----------------------------------------------------------------
// TickGen
// generate a single-sample tick, repeating at a frequency given by the input.

template<typename T>
struct TickGen : Gen1<T, TickGen<T>>
{
  enum { freq, nParams };
  
  using Params = std::array<T, nParams>;
  
  T omega_{0.f};
  
  void clear() { omega_ = T{0.f}; }
  
  T nextFrame(T f)
  {
    omega_ += f;
    if constexpr (std::is_same_v<T, float>)
    {
      if (omega_ >= 1.0f)
      {
        omega_ -= 1.0f;
        return 1.0f;
      }
      return 0.0f;
    }
    else
    {
      float4 ones(1.0f);
      float4 mask = (omega_ >= ones);
      omega_ = omega_ - andBits(mask, ones);
      return andBits(mask, ones);
    }
  }
};

// Bandlimited impulse train generator.
// Uses a windowed sinc table with two readout voices for crossfading
// when impulses overlap at high frequencies.

template<typename T>
struct ImpulseGen : Gen1<T, ImpulseGen<T>>
{
  static constexpr int kSincHalfWidth = 16;
  static constexpr int kOversample = 8;
  static constexpr int kTableSize = kSincHalfWidth * 2 * kOversample + 1; // 129
  static constexpr float kTableEnd = static_cast<float>(kTableSize - 1);  // 128.0
  static constexpr float kTableStep = static_cast<float>(kOversample);    // 8.0
  static constexpr float kSincOmega = 0.45f;
  
  // make windowed-sinc filter.
  static const std::array<float, kTableSize>& getTable()
  {
    static const auto table = [] {
      std::array<float, kTableSize> t{};
      constexpr int center = kSincHalfWidth * kOversample;
      float peak = 0.f;
      for (int i = 0; i < kTableSize; ++i)
      {
        float x = static_cast<float>(i - center) / static_cast<float>(kOversample);
        float w = 0.42f - 0.5f * cosf(kTwoPi * static_cast<float>(i) / static_cast<float>(kTableSize - 1))
        + 0.08f * cosf(2.f * kTwoPi * static_cast<float>(i) / static_cast<float>(kTableSize - 1));
        float pi_x = kTwoPi * kSincOmega * x;
        float s = (i == center) ? 1.f : sinf(pi_x) / pi_x;
        t[i] = s * w;
        peak = std::max(peak, std::abs(t[i]));
      }
      for (auto& v : t) v /= peak;
      return t;
    }();
    return table;
  }
  
  // State: two readout voices
  T phase_{0.f};
  T posA_{kTableEnd}, ampA_{0.f}, ampStepA_{0.f};
  T posB_{kTableEnd}, ampB_{0.f}, ampStepB_{0.f};
  
  void clear()
  {
    phase_ = T{1.f};  // wraps on first nextFrame call
    posA_ = posB_ = T{kTableEnd};
    ampA_ = ampB_ = T{0.f};
    ampStepA_ = ampStepB_ = T{0.f};
  }
  
  static float tableLookupScalar(float pos)
  {
    if (pos < 0.f || pos >= kTableEnd) return 0.f;
    const auto& table = getTable();
    int idx = static_cast<int>(pos);
    float frac = pos - static_cast<float>(idx);
    return table[idx] + frac * (table[idx + 1] - table[idx]);
  }
  
  static T tableLookup(T pos)
  {
    if constexpr (std::is_same_v<T, float>)
    {
      return tableLookupScalar(pos);
    }
    else
    {
      // Gather: extract lanes, look up individually, reassemble.
      // Requires storeFloat4 / loadFloat4 (see note below).
      alignas(16) float lanes[4];
      storeFloat4(lanes, pos);
      for (int i = 0; i < 4; ++i)
        lanes[i] = tableLookupScalar(lanes[i]);
      return loadFloat4(lanes);
    }
  }
  
  T nextFrame(T freq)
  {
    T prevPhase = phase_;
    phase_ = fracPart(phase_ + freq);
    
    if constexpr (std::is_same_v<T, float>)
    {
      if (phase_ < prevPhase) // wrap = trigger
      {
        float offset = phase_ / freq * kTableStep;
        
        if (posA_ < kTableEnd) // overlap: crossfade
        {
          posB_ = posA_;
          ampB_ = ampA_;
          float samplesLeft = (kTableEnd - posA_) / kTableStep;
          float fadeLen = std::max(1.f, std::min(samplesLeft, 1.f / freq));
          ampStepB_ = -ampB_ / fadeLen;
          ampA_ = 0.f;
          ampStepA_ = 1.f / fadeLen;
        }
        else // clean start
        {
          ampA_ = 1.f;
          ampStepA_ = 0.f;
        }
        posA_ = offset;
      }
      
      float output = tableLookup(posA_) * ampA_
      + tableLookup(posB_) * ampB_;
      
      posA_ = std::min(posA_ + kTableStep, kTableEnd);
      posB_ = std::min(posB_ + kTableStep, kTableEnd);
      ampA_ = std::max(0.f, ampA_ + ampStepA_);
      ampB_ = std::max(0.f, ampB_ + ampStepB_);
      
      return output;
    }
    else // float4
    {
      T trigMask = (phase_ < prevPhase);
      T offset = phase_ * rcp(freq) * T{kTableStep};
      
      T aActive = (posA_ < T{kTableEnd});
      T overlapMask = andBits(trigMask, aActive);
      T cleanMask = andNotBits(aActive, trigMask);
      
      // Overlap: move A→B, set up crossfade
      posB_ = select(posB_, posA_, overlapMask);
      ampB_ = select(ampB_, ampA_, overlapMask);
      T samplesLeft = (T{kTableEnd} - posA_) * T{1.f / kTableStep};
      T fadeLen = max(T{1.f}, min(samplesLeft, rcp(freq)));
      T rcpFade = rcp(fadeLen);
      ampStepB_ = select(ampStepB_, T{-1.f} * ampB_ * rcpFade, overlapMask);
      
      // Set amplitude for triggered lanes
      ampA_ = select(ampA_, T{0.f}, overlapMask);
      ampStepA_ = select(ampStepA_, rcpFade, overlapMask);
      ampA_ = select(ampA_, T{1.f}, cleanMask);
      ampStepA_ = select(ampStepA_, T{0.f}, cleanMask);
      
      // Clean trigger: explicitly silence voice B
      ampB_ = select(ampB_, T{0.f}, cleanMask);
      ampStepB_ = select(ampStepB_, T{0.f}, cleanMask);
      
      // Start position for triggered lanes
      posA_ = select(posA_, offset, trigMask);
      
      // Read and mix
      T output = tableLookup(posA_) * ampA_
      + tableLookup(posB_) * ampB_;
      
      // Advance
      posA_ = min(posA_ + T{kTableStep}, T{kTableEnd});
      posB_ = min(posB_ + T{kTableStep}, T{kTableEnd});
      ampA_ = max(T{0.f}, ampA_ + ampStepA_);
      ampB_ = max(T{0.f}, ampB_ + ampStepB_);
      
      return output;
    }
  }
};


template<typename T>
struct NoiseGen : Gen0<T, NoiseGen<T>>
{
  using IntState = std::conditional_t<std::is_same_v<T, float>, uint32_t, int4>;
  
  IntState seed_{};
  
  void clear() { setSeed(39792); }
  
  void setSeed(uint32_t x)
  {
    if constexpr (std::is_same_v<T, float>)
    {
      seed_ = x;
    }
    else
    {
      seed_ = setrInt(x, x * 2, x * 3, x * 4);
    }
  }
  
  T nextFrame()
  {
    using IntT = IntTypeFor_t<T>;
    seed_ = seed_ * IntT{0x0019660D} + IntT{0x3C6EF35F};
    IntT temp = (seed_ >> 9) & IntT{0x007FFFFF} | IntT{0x3F800000};
    return reinterpretIntAsFloat(temp) * T{2.f} - T{3.f};
  }
};

// ----------------------------------------------------------------
// PhasorGen: naive (not antialiased) sawtooth / phase ramp on (0, 1).
// Input: frequency in cycles per sample (f/sr).
// NOTE: uses float phase accumulation rather than the uint32
// trick in the original. Slight precision loss but works for float4.

template<typename T>
struct PhasorGen : Gen1<T, PhasorGen<T> >
{
  T omega_{0.f};
  
  void clear() { omega_ = T{0.f}; }
  
  T nextFrame(T freq)
  {
    omega_ = fracPart(omega_ + freq);
    return omega_;
  }
};

// ----------------------------------------------------------------
// super slow + accurate sine generator for testing

template<typename T>
struct TestSineGen : Gen1<T, TestSineGen<T> >
{
  T omega_{0.f};
  
  void clear() { omega_ = T{0.f}; }
  
  T nextFrame(T freq)
  {
    omega_ += T{kTwoPi} * freq;
    if constexpr (std::is_same_v<T, float>)
    {
      if (omega_ > kTwoPi) omega_ -= kTwoPi;
    }
    else
    {
      T wrap = T{kTwoPi};
      T mask = (omega_ >= wrap);
      omega_ = omega_ - andBits(mask, wrap);
    }
    return sin(omega_);
  }
};

// ----------------------------------------------------------------
// Antialiased waveform generators using PhasorGen

template<typename T>
struct SineGen : Gen1<T, SineGen<T> >
{
  PhasorGen<T> phasor_;
  
  // initial phase of 0.75 maps to zero-crossing of the sine approximation
  void clear() { phasor_.clear(); phasor_.omega_ = T{0.75f}; }
  
  T nextFrame(T freq)
  {
    return phasorToSineSample(phasor_.nextFrame(freq));
  }
};

template<typename T>
struct SawGen : Gen1<T, SawGen<T> >
{
  PhasorGen<T> phasor_;
  
  void clear() { phasor_.clear(); }
  
  T nextFrame(T freq)
  {
    T phase = phasor_.nextFrame(freq);
    return phasorToSawSample(phase, freq);
  }
};

// PulseGen takes two inputs (freq and width) so it has its own operator().
template<typename T>
struct PulseGen
{
  PhasorGen<T> phasor_;
  
  void clear() { phasor_.clear(); }
  
  Block<T> operator()(const Block<T>& freq, const Block<T>& width)
  {
    Block<T> output;
    for (size_t t = 0; t < kFramesPerBlock; ++t)
    {
      T phase = phasor_.nextFrame(freq[t]);
      output[t] = phasorToPulseSample(phase, freq[t], width[t]);
    }
    return output;
  }
};


}  // namespace ml
