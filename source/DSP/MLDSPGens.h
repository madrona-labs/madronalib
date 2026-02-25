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
struct Gen
{
  
  Gen() = default;

  // Block processing with signal-rate params (one Params per frame)
  template<size_t N_PARAMS>
  Block<T> operator()(const SignalBlockArrayBase<T, N_PARAMS>& paramBlock)
  {
    static_assert(N_PARAMS == Derived::nParams, "paramBlock row count must match nParams");
    auto& self = *static_cast<Derived*>(this);
    Block<T> output;
    
    for (size_t t = 0; t < kFramesPerBlock; ++t)
    {
      typename Derived::Params p;
      for (size_t i = 0; i < Derived::nParams; ++i)
        p[i] = paramBlock.rowPtr(i)[t];
      self.coeffs = Derived::makeCoeffs(p);
      output[t] = self.nextFrame(self.coeffs);
    }
    return output;
  }
  
  // Block processing with coefficient interpolation from std::array argument
  template<size_t N_PARAMS>
  Block<T> operator()(const std::array<T, N_PARAMS>& nextParams)
  {
    static_assert(N_PARAMS == Derived::nParams, "paramBlock row count must match nParams");
    auto& self = *static_cast<Derived*>(this);
    Block<T> output;
    
    auto nextCoeffs = Derived::makeCoeffs(nextParams);
    auto coeffsBlock = interpolateCoeffsLinear(self.coeffs, nextCoeffs);
    self.coeffs = nextCoeffs;
    
    for (size_t t = 0; t < kFramesPerBlock; ++t)
    {
      typename Derived::Coeffs c;
      for (size_t i = 0; i < Derived::nCoeffs; ++i)
        c[i] = coeffsBlock.rowPtr(i)[t];
      output[t] = self.nextFrame(c);
    }
    return output;
  }
  
  
  // Block processing with coefficient interpolation — list of float arguments
  template<typename... Args,
  typename = std::enable_if_t<(std::is_same_v<std::remove_cv_t<std::remove_reference_t<Args>>, float> && ...)>>
  Block<T> operator()(Args&&... args)
  {
    std::array<std::common_type_t<Args...>, sizeof...(Args)> arr = { std::forward<Args>(args)... };
    const std::array nextParams = arr;
    
    auto& self = *static_cast<Derived*>(this);
    Block<T> output;
    
    auto nextCoeffs = Derived::makeCoeffs(nextParams);
    auto coeffsBlock = interpolateCoeffsLinear(self.coeffs, nextCoeffs);
    self.coeffs = nextCoeffs;
    
    for (size_t t = 0; t < kFramesPerBlock; ++t)
    {
      typename Derived::Coeffs c;
      for (size_t i = 0; i < Derived::nCoeffs; ++i)
        c[i] = coeffsBlock.rowPtr(i)[t];
      output[t] = self.nextFrame(c);
    }
    return output;
  }

  // Block processing with constant stored coefficients
  Block<T> operator()()
  {
    auto& self = *static_cast<Derived*>(this);
    Block<T> output;
    for (size_t t = 0; t < kFramesPerBlock; ++t)
      output[t] = self.nextFrame(self.coeffs);
    return output;
  }
};

// a generator with no params.
template<typename T, typename Derived>
struct Gen0
{
  Block<T> operator()()
  {
    auto& self = *static_cast<Derived*>(this);
    Block<T> output;
    for (size_t t = 0; t < kFramesPerBlock; ++t)
      output[t] = self.nextFrame();
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
    pulse -= polyBLEPSample<T>(phase, freq);
    float downPhase = phase - pulseWidth + 1.0f;
    downPhase = downPhase - intPart(downPhase);
    pulse += polyBLEPSample<T>(downPhase, freq);
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
struct Counter : Gen<T, Counter<T>>
{
  enum { nParams = 0 };
  enum { nCoeffs = 0 };
  
  using Params = std::array<T, nParams>;
  using Coeffs = std::array<T, nCoeffs>;
  
  Coeffs coeffs{};
  T state_{0.f};
  
  void clear() { state_ = T{0.f}; }
  
  static Coeffs makeCoeffs(Params) { return {}; }
  
  T nextFrame(Coeffs)
  {
    T currentValue = state_;
    state_ += T{1.0f};
    return currentValue;
  }
};

// ----------------------------------------------------------------
// TickGen
// Generate a single-sample tick, repeating at a frequency given by the input.

template<typename T>
struct TickGen : Gen<T, TickGen<T>>
{
  enum { freq, nParams };
  enum { freqCoeff, nCoeffs };
  
  using Params = std::array<T, nParams>;
  using Coeffs = std::array<T, nCoeffs>;
  
  Coeffs coeffs{};
  T omega_{0.f};

  TickGen() = default;
  TickGen(T freq) { coeffs = makeCoeffs(Params{freq}); }

  void clear() { omega_ = T{0.f}; }

  static Coeffs makeCoeffs(Params p) { return Coeffs(p); }

  T nextFrame(Coeffs c)
  {
    omega_ += c[freqCoeff];
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
      T ones{1.0f};
      T mask = (omega_ >= ones);
      omega_ = omega_ - andBits(mask, ones);
      return andBits(mask, ones);
    }
  }
};

// ----------------------------------------------------------------
// Bandlimited impulse train generator.
// Uses a windowed sinc table with two readout voices for crossfading
// when impulses overlap at high frequencies.

template<typename T>
struct ImpulseGen : Gen<T, ImpulseGen<T>>
{
  enum { freq, nParams };
  enum { freqCoeff, nCoeffs };
  
  using Params = std::array<T, nParams>;
  using Coeffs = std::array<T, nCoeffs>;
  
  static constexpr int kSincHalfWidth = 16;
  static constexpr int kOversample = 8;
  static constexpr int kTableSize = kSincHalfWidth * 2 * kOversample + 1; // 129
  static constexpr float kTableEnd = static_cast<float>(kTableSize - 1);  // 128.0
  static constexpr float kTableStep = static_cast<float>(kOversample);    // 8.0
  static constexpr float kSincOmega = 0.45f;
  
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
  
  Coeffs coeffs{};

  ImpulseGen() = default;
  ImpulseGen(T freq) { coeffs = makeCoeffs(Params{freq}); }

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
  
  static Coeffs makeCoeffs(Params p) { return Coeffs(p); }
  
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
      alignas(16) float lanes[4];
      storeFloat4(lanes, pos);
      for (int i = 0; i < 4; ++i)
        lanes[i] = tableLookupScalar(lanes[i]);
      return loadFloat4(lanes);
    }
  }
  
  T nextFrame(Coeffs c)
  {
    T f = c[freqCoeff];
    T prevPhase = phase_;
    phase_ = fracPart(phase_ + f);
    
    if constexpr (std::is_same_v<T, float>)
    {
      if (phase_ < prevPhase) // wrap = trigger
      {
        float offset = phase_ / f * kTableStep;
        
        if (posA_ < kTableEnd) // overlap: crossfade
        {
          posB_ = posA_;
          ampB_ = ampA_;
          float samplesLeft = (kTableEnd - posA_) / kTableStep;
          float fadeLen = std::max(1.f, std::min(samplesLeft, 1.f / f));
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
      T offset = phase_ * rcp(f) * T{kTableStep};
      
      T aActive = (posA_ < T{kTableEnd});
      T overlapMask = andBits(trigMask, aActive);
      T cleanMask = andNotBits(aActive, trigMask);
      
      posB_ = select(posB_, posA_, overlapMask);
      ampB_ = select(ampB_, ampA_, overlapMask);
      T samplesLeft = (T{kTableEnd} - posA_) * T{1.f / kTableStep};
      T fadeLen = max(T{1.f}, min(samplesLeft, rcp(f)));
      T rcpFade = rcp(fadeLen);
      ampStepB_ = select(ampStepB_, T{-1.f} * ampB_ * rcpFade, overlapMask);
      
      ampA_ = select(ampA_, T{0.f}, overlapMask);
      ampStepA_ = select(ampStepA_, rcpFade, overlapMask);
      ampA_ = select(ampA_, T{1.f}, cleanMask);
      ampStepA_ = select(ampStepA_, T{0.f}, cleanMask);
      
      ampB_ = select(ampB_, T{0.f}, cleanMask);
      ampStepB_ = select(ampStepB_, T{0.f}, cleanMask);
      
      posA_ = select(posA_, offset, trigMask);
      
      T output = tableLookup(posA_) * ampA_
      + tableLookup(posB_) * ampB_;
      
      posA_ = min(posA_ + T{kTableStep}, T{kTableEnd});
      posB_ = min(posB_ + T{kTableStep}, T{kTableEnd});
      ampA_ = max(T{0.f}, ampA_ + ampStepA_);
      ampB_ = max(T{0.f}, ampB_ + ampStepB_);
      
      return output;
    }
  }
};

// ----------------------------------------------------------------
// NoiseGen

template<typename T>
struct NoiseGen : Gen0<T, NoiseGen<T>>
{
  using IntState = std::conditional_t<std::is_same_v<T, float>, uint32_t, int4>;
  IntState seed_{};
  
  void setSeed(uint32_t x)
  {
    if constexpr (std::is_same_v<T, float>)
    {
      seed_ = x;
    }
    else
    {
      // set different seeds for each lane
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

template<typename T>
struct PhasorGen : Gen<T, PhasorGen<T>>
{
  enum { freq, nParams };
  enum { freqCoeff, nCoeffs };
  
  using Params = std::array<T, nParams>;
  using Coeffs = std::array<T, nCoeffs>;
  
  Coeffs coeffs{};
  T omega_{0.f};

  PhasorGen() = default;
  PhasorGen(T freq) { coeffs = makeCoeffs(Params{freq}); }

  void clear() { omega_ = T{0.f}; }

  // just copying param to get the coefficient, needed for template compatibility
  static Coeffs makeCoeffs(Params p) { return Coeffs(p); }
  
  T nextFrame(Coeffs c)
  {
    omega_ = fracPart(omega_ + c[freqCoeff]);
    return omega_;
  }
};

// ----------------------------------------------------------------
// super slow + accurate sine generator for testing

template<typename T>
struct TestSineGen : Gen<T, TestSineGen<T>>
{
  enum { freq, nParams };
  enum { freqCoeff, nCoeffs };
  
  using Params = std::array<T, nParams>;
  using Coeffs = std::array<T, nCoeffs>;
  
  Coeffs coeffs{};
  T omega_{0.f};

  TestSineGen() = default;
  TestSineGen(T freq) { coeffs = makeCoeffs(Params{freq}); }

  void clear() { omega_ = T{0.f}; }

  static Coeffs makeCoeffs(Params p) { return Coeffs(p); }

  T nextFrame(Coeffs c)
  {
    omega_ += T{kTwoPi} * c[freqCoeff];
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
struct SineGen : Gen<T, SineGen<T>>
{
  enum { freq, nParams };
  enum { freqCoeff, nCoeffs };
  
  using Params = std::array<T, nParams>;
  using Coeffs = std::array<T, nCoeffs>;
  
  Coeffs coeffs{};
  PhasorGen<T> phasor_;

  SineGen() = default;
  SineGen(T freq) { coeffs = makeCoeffs(Params{freq}); }

  // initial phase of 0.75 maps to zero-crossing of the sine approximation
  void clear() { phasor_.clear(); phasor_.omega_ = T{0.75f}; }
  
  static Coeffs makeCoeffs(Params p) { return Coeffs(p); }
  
  T nextFrame(Coeffs c)
  {
    return phasorToSineSample(phasor_.nextFrame(typename PhasorGen<T>::Coeffs{c[freqCoeff]}));
  }
};

template<typename T>
struct SawGen : Gen<T, SawGen<T>>
{
  enum { freq, nParams };
  enum { freqCoeff, nCoeffs };
  
  using Params = std::array<T, nParams>;
  using Coeffs = std::array<T, nCoeffs>;
  
  Coeffs coeffs{};
  PhasorGen<T> phasor_;

  SawGen() = default;
  SawGen(T freq) { coeffs = makeCoeffs(Params{freq}); }

  void clear() { phasor_.clear(); }

  static Coeffs makeCoeffs(Params p) { return Coeffs(p); }

  T nextFrame(Coeffs c)
  {
    T f = c[freqCoeff];
    T phase = phasor_.nextFrame(typename PhasorGen<T>::Coeffs{f});
    return phasorToSawSample(phase, f);
  }
};

// PulseGen takes two inputs (freq and width).

template<typename T>
struct PulseGen : Gen<T, PulseGen<T>>
{
  enum { freq, width, nParams };
  enum { freqCoeff, widthCoeff, nCoeffs };
  
  using Params = std::array<T, nParams>;
  using Coeffs = std::array<T, nCoeffs>;
  
  const Params kDefaultParams{0.f, 0.5f};
  Coeffs coeffs{makeCoeffs(kDefaultParams)};
  PhasorGen<T> phasor_;

  PulseGen() = default;
  PulseGen(T freq, T width = 0.5f) { coeffs = makeCoeffs(Params{freq, width}); }

  void clear()
  {
    phasor_.clear();
  }
  
  static Coeffs makeCoeffs(Params p) { return Coeffs(p); }
  
  T nextFrame(Coeffs c)
  {
    T f = c[freqCoeff];
    T phase = phasor_.nextFrame(typename PhasorGen<T>::Coeffs{f});
    return phasorToPulseSample(phase, f, c[widthCoeff]);
  }
};

}  // namespace ml
