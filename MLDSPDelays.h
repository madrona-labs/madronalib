// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// DSP filters: functor objects implementing an operator()(Block<T> input, ...).
//
// Filter cutoffs are set by a parameter omega, equal to frequency / sample
// rate. This lets filter objects be unaware of the sample rate, resulting in
// less code overall. For all filters, k is a damping parameter equal to 1/Q
// where Q is the analog filter "quality." For bell and shelf filters, gain is
// specified as an output / input ratio A.

#pragma once

#include <vector>

#include "MLDSPOps.h"

namespace ml
{

// IntegerDelay delays a signal a whole number of samples.

class IntegerDelay
{
  std::vector<float> mBuffer;
  int mIntDelayInSamples{0};
  uintptr_t mWriteIndex{0};
  uintptr_t mLengthMask{0};

 public:
  IntegerDelay() = default;
  IntegerDelay(int d)
  {
    setMaxDelayInSamples(static_cast<float>(d));
    setDelayInSamples(d);
  }
  ~IntegerDelay() = default;

  // for efficiency, no bounds checking is done. Because mLengthMask is used to
  // constrain all reads, bad values here may make bad sounds (buffer wraps) but
  // will not attempt to read from outside the buffer.
  inline void setDelayInSamples(int d) { mIntDelayInSamples = d; }

  void setMaxDelayInSamples(float d)
  {
    int dMax = static_cast<int>(floorf(d));
    int newSize = 1 << bitsToContain(dMax + kFramesPerBlock);
    mBuffer.resize(newSize);
    mLengthMask = newSize - 1;
    mWriteIndex = 0;
    clear();
  }

  inline void clear() { std::fill(mBuffer.begin(), mBuffer.end(), 0.f); }

  inline SignalBlock operator()(const SignalBlock vx)
  {
    // write
    uintptr_t writeEnd = mWriteIndex + kFramesPerBlock;
    if (writeEnd <= mLengthMask + 1)
    {
      const float* srcStart = vx.data();
      std::copy(srcStart, srcStart + kFramesPerBlock, mBuffer.data() + mWriteIndex);
    }
    else
    {
      uintptr_t excess = writeEnd - mLengthMask - 1;
      const float* srcStart = vx.data();
      const float* srcSplice = srcStart + kFramesPerBlock - excess;
      const float* srcEnd = srcStart + kFramesPerBlock;
      std::copy(srcStart, srcSplice, mBuffer.data() + mWriteIndex);
      std::copy(srcSplice, srcEnd, mBuffer.data());
    }

    // read
    SignalBlock vy;
    uintptr_t readStart = (mWriteIndex - mIntDelayInSamples) & mLengthMask;
    uintptr_t readEnd = readStart + kFramesPerBlock;
    float* srcBuf = mBuffer.data();
    if (readEnd <= mLengthMask + 1)
    {
      std::copy(srcBuf + readStart, srcBuf + readEnd, vy.data());
    }
    else
    {
      uintptr_t excess = readEnd - mLengthMask - 1;
      uintptr_t readSplice = readStart + kFramesPerBlock - excess;
      float* pDest = vy.data();
      std::copy(srcBuf + readStart, srcBuf + readSplice, pDest);
      std::copy(srcBuf, srcBuf + excess, pDest + (kFramesPerBlock - excess));
    }

    // update index
    mWriteIndex += kFramesPerBlock;
    mWriteIndex &= mLengthMask;
    return vy;
  }

  inline SignalBlock operator()(const SignalBlock x, const SignalBlock delay)
  {
    SignalBlock y;

    for (int n = 0; n < kFramesPerBlock; ++n)
    {
      // write
      mBuffer[mWriteIndex] = x[n];

      // read
      mIntDelayInSamples = static_cast<int>(delay[n]);
      uintptr_t readIndex = (mWriteIndex - mIntDelayInSamples) & mLengthMask;

      y[n] = mBuffer[readIndex];
      mWriteIndex++;
      mWriteIndex &= mLengthMask;
    }

    return y;
  }

  inline float processSample(float x)
  {
    // write
    // note that, for performance, there is no bounds checking. If you crash
    // here, you probably didn't allocate enough delay memory.
    mBuffer[mWriteIndex] = x;

    // read
    uintptr_t readIndex = (mWriteIndex - mIntDelayInSamples) & mLengthMask;
    float y = mBuffer[readIndex];

    // update index
    mWriteIndex++;
    mWriteIndex &= mLengthMask;
    return y;
  }
};



// First order allpass section with a single sample of delay.
// One-multiply form, see
// https://ccrma.stanford.edu/~jos/pasp/One_Multiply_Scattering_Junctions.html

class Allpass1
{
 private:
  float x1{0}, y1{0};

 public:
  typedef float Coeffs;
  Coeffs coeffs{0.f};

  Allpass1(float a) : coeffs(a) {}
  ~Allpass1() {}

  inline void clear()
  {
    x1 = 0.f;
    y1 = 0.f;
  }

  // get allpass coefficient from a delay fraction d.
  // to minimize modulation noise, d should be in the range [0.618 - 1.618].
  static float makeCoeffs(float d)
  {
    // return 2nd order approx around 1 to (1.f - d) / (1.f + d)
    float xm1 = (d - 1.f);
    return -0.53f * xm1 + 0.24f * xm1 * xm1;
  }

  inline float processSample(const float x)
  {
    // one-multiply form. see
    // https://ccrma.stanford.edu/~jos/pasp/One_Multiply_Scattering_Junctions.html
    float y = x1 + (x - y1) * coeffs;
    x1 = x;
    y1 = y;
    return y;
  }

  inline SignalBlock operator()(const SignalBlock vx)
  {
    SignalBlock vy;
    for (int n = 0; n < kFramesPerBlock; ++n)
    {
      vy[n] = processSample(vx[n]);
    }
    return vy;
  }
};

// Combining the integer delay and first order allpass section
// gives us an allpass-interpolated fractional delay. In general, modulating the
// delay time will change the allpass coefficient, producing clicks in the
// output.

class FractionalDelay
{
  IntegerDelay mIntegerDelay;
  Allpass1 mAllpassSection{0.f};
  float mDelayInSamples{};

 public:
  FractionalDelay() = default;
  FractionalDelay(float d)
  {
    setMaxDelayInSamples(d);
    setDelayInSamples(d);
  }
  ~FractionalDelay() = default;

  inline void clear()
  {
    mIntegerDelay.clear();
    mAllpassSection.clear();
  }

  inline void setDelayInSamples(float d)
  {
    mDelayInSamples = d;
    float fDelayInt = floorf(d);
    int delayInt = static_cast<int>(fDelayInt);
    float delayFrac = d - fDelayInt;

    // constrain D to [0.618 - 1.618] if possible
    if ((delayFrac < 0.618f) && (delayInt > 0))
    {
      delayFrac += 1.f;
      delayInt -= 1;
    }
    mIntegerDelay.setDelayInSamples(delayInt);
    mAllpassSection.coeffs = Allpass1::makeCoeffs(delayFrac);
  }

  inline void setMaxDelayInSamples(float d) { mIntegerDelay.setMaxDelayInSamples(floorf(d)); }

  // return the input signal, delayed by the constant delay time
  // mDelayInSamples.
  inline SignalBlock operator()(const SignalBlock vx) { return mAllpassSection(mIntegerDelay(vx)); }

  // return the input signal, delayed by the varying delay time vDelayInSamples.
  inline SignalBlock operator()(const SignalBlock vx, const SignalBlock vDelayInSamples)
  {
    SignalBlock vy;
    for (int n = 0; n < kFramesPerBlock; ++n)
    {
      setDelayInSamples(vDelayInSamples[n]);
      vy[n] = mAllpassSection.processSample(mIntegerDelay.processSample(vx[n]));
    }
    return vy;
  }

  // return the input signal, delayed by the varying delay time vDelayInSamples,
  // but only allow changes to the delay time when vChangeTicks is nonzero.
  inline SignalBlock operator()(const SignalBlock vx, const SignalBlock vDelayInSamples,
                              const SignalBlockInt vChangeTicks)
  {
    SignalBlock vy;
    for (int n = 0; n < kFramesPerBlock; ++n)
    {
      if (vChangeTicks[n] != 0)
      {
        setDelayInSamples(vDelayInSamples[n]);
      }

      vy[n] = mAllpassSection.processSample(mIntegerDelay.processSample(vx[n]));
    }
    return vy;
  }
};

// Crossfading two allpass-interpolated delays allows modulating the delay
// time without clicks. See "A Lossless, Click-free, Pitchbend-able Delay Line
// Loop Interpolation Scheme", Van Duyne, Jaffe, Scandalis, Stilson, ICMC 1997.

namespace PitchbendableDelayConsts
{
// period in samples of allpass fade cycle. must be a power of 2 less than or
// equal to kFramesPerBlock. 32 sounds good.
constexpr int kFadePeriod{32};
constexpr int fadeRamp(size_t n) { return n % kFadePeriod; }
constexpr int ticks1(size_t n) { return fadeRamp(n) == kFadePeriod / 2; }
constexpr int ticks2(size_t n) { return fadeRamp(n) == 0; }
constexpr float fadeFn(size_t n)
{
  // triangle from 0 to 1 to 0
  return 2.f * ((fadeRamp(n)) > kFadePeriod / 2 ? 1.0f - (fadeRamp(n)) / (kFadePeriod + 0.f)
                                                : (fadeRamp(n)) / (kFadePeriod + 0.f));
}

// generate vectors of ticks indicating when delays can change
// equality operators on vectors return 0 or 0xFFFFFFFF
// note: mDelay1's delay time will be 0 when the object is created and before
// the first half fade period. so there is a warmup time of one half fade
// period: any input before this will be attenuated.
//
// constexpr fill is used. unfortunately this cannot be made to work with a
// lambda in C++11. TODO Revisit.
constexpr SignalBlockInt test1(fadeRamp);
constexpr SignalBlockInt kvDelay1Changes(ticks1);
constexpr SignalBlockInt kvDelay2Changes(ticks2);
constexpr SignalBlock kvFade(fadeFn);
};  // namespace PitchbendableDelayConsts

class PitchbendableDelay
{
  FractionalDelay mDelay1, mDelay2;

 public:
  PitchbendableDelay() = default;

  inline void setMaxDelayInSamples(float d)
  {
    mDelay1.setMaxDelayInSamples(d);
    mDelay2.setMaxDelayInSamples(d);
  }

  inline void clear()
  {
    mDelay1.clear();
    mDelay2.clear();
  }

  inline SignalBlock operator()(const SignalBlock vInput, const SignalBlock vDelayInSamples)
  {
    using namespace PitchbendableDelayConsts;

    // run the fractional delays and crossfade the results.
    return lerp(mDelay1(vInput, vDelayInSamples, kvDelay1Changes),
                mDelay2(vInput, vDelayInSamples, kvDelay2Changes), kvFade);
  }
};

// General purpose allpass filter with arbitrary delay length.
// For efficiency, the minimum delay time is one SignalBlock.

template <typename DELAY_TYPE>
class Allpass
{
  DELAY_TYPE mDelay;
  SignalBlock vy1{};

 public:
  float mGain{0.f};

  // use setDelayInSamples to set a constant delay time with DELAY_TYPE of
  // IntegerDelay or FractionalDelay.
  inline void setDelayInSamples(float d) { mDelay.setDelayInSamples(d - kFramesPerBlock); }

  inline void setMaxDelayInSamples(float d)
  {
    mDelay.setMaxDelayInSamples(d - kFramesPerBlock);
  }

  inline void clear()
  {
    mDelay.clear();
    vy1 = SignalBlock();
  }

  // use with constant delay time.
  inline SignalBlock operator()(const SignalBlock vInput)
  {
    SignalBlock vGain(-mGain);
    SignalBlock vDelayInput = vInput - vy1 * vGain;
    SignalBlock y = vDelayInput * vGain + vy1;
    vy1 = mDelay(vDelayInput);
    return y;
  }

  // use vDelayInSamples parameter to set a varying delay time with DELAY_TYPE =
  // PitchbendableDelay.
  inline SignalBlock operator()(const SignalBlock vInput, const SignalBlock vDelayInSamples)
  {
    SignalBlock vGain(-mGain);
    SignalBlock vDelayInput = vInput - vy1 * vGain;
    SignalBlock y = vDelayInput * vGain + vy1;
    vy1 = mDelay(vDelayInput, vDelayInSamples - SignalBlock(kFramesPerBlock));
    return y;
  }
};


// FDN
// A general Feedback Delay Network with N delay lines connected in an NxN
// matrix.
// TODO DELAY_TYPE parameter for modulation?

template <int SIZE>
class FDN
{
  std::array<IntegerDelay, SIZE> mDelays;
  std::array<OnePole<float>, SIZE> mFilters;
  std::array<SignalBlock, SIZE> mDelayInputVectors{{{SignalBlock(0.f)}}};

 public:
  // feedback gains array is public—just copy values to set.
  std::array<float, SIZE> mFeedbackGains{{0}};

  void setDelaysInSamples(std::array<float, SIZE> times)
  {
    for (int n = 0; n < SIZE; ++n)
    {
      // we have one SignalBlock feedback latency, so compensate delay times for
      // that.
      int len = times[n] - kFramesPerBlock;
      len = std::max(1, len);
      mDelays[n].setDelayInSamples(len);
    }
  }

  void setFilterCutoffs(std::array<float, SIZE> omegas)
  {
    for (int n = 0; n < SIZE; ++n)
    {
      mFilters[n].coeffs = ml::OnePole<float>::makeCoeffs(omegas[n]);
    }
  }

  // stereo output function
  // TODO generalize n-channel output function somehow
  SignalBlockArray<2> operator()(const SignalBlock x)
  {
    // run delays, getting SignalBlock for each delay
    for (int n = 0; n < SIZE; ++n)
    {
      mDelayInputVectors[n] = mDelays[n](mDelayInputVectors[n]);
    }

    // get output sum
    SignalBlock sumR, sumL;
    for (int n = 0; n < (SIZE & (~1)); ++n)
    {
      if (n & 1)
      {
        sumL += mDelayInputVectors[n];
      }
      else
      {
        sumR += mDelayInputVectors[n];
      }
    }

    // inputs = input gains*input sample + filters(M*delay outputs)
    // The feedback matrix M is a unit-gain Householder matrix, which is just
    // the identity matrix minus a constant k, where k = 2/size. Since
    // multiplying this can be simplified so much, you just see a few operations
    // here, not a general matrix multiply.

    SignalBlock sumOfDelays;
    for (int n = 0; n < SIZE; ++n)
    {
      sumOfDelays += mDelayInputVectors[n];
    }
    sumOfDelays *= SignalBlock(2.0f / SIZE);

    for (int n = 0; n < SIZE; ++n)
    {
      mDelayInputVectors[n] -= (sumOfDelays);
      mDelayInputVectors[n] = mFilters[n](mDelayInputVectors[n]) * SignalBlock(mFeedbackGains[n]);
      mDelayInputVectors[n] += x;
    }

    return concatRows(sumL, sumR);
  }
};

// Half Band Filter
// Polyphase allpass filter used to upsample or downsample a signal by 2x.
// Structure due to fred harris, A. G. Constantinides and Valenzuela.

class HalfBandFilter
{
 public:
  inline SignalBlock upsampleFirstHalf(const SignalBlock vx)
  {
    SignalBlock vy;
    int i2 = 0;
    for (int i = 0; i < kFramesPerBlock / 2; ++i)
    {
      vy[i2++] = apa1.processSample(apa0.processSample(vx[i]));
      vy[i2++] = apb1.processSample(apb0.processSample(vx[i]));
    }
    return vy;
  }

  inline SignalBlock upsampleSecondHalf(const SignalBlock vx)
  {
    SignalBlock vy;
    int i2 = 0;
    for (int i = kFramesPerBlock / 2; i < kFramesPerBlock; ++i)
    {
      vy[i2++] = apa1.processSample(apa0.processSample(vx[i]));
      vy[i2++] = apb1.processSample(apb0.processSample(vx[i]));
    }
    return vy;
  }

  inline SignalBlock downsample(const SignalBlock vx1, const SignalBlock vx2)
  {
    SignalBlock vy;
    int i2 = 0;
    for (int i = 0; i < kFramesPerBlock / 2; ++i)
    {
      float a0 = apa1.processSample(apa0.processSample(vx1[i2]));
      float b0 = apb1.processSample(apb0.processSample(vx1[i2 + 1]));
      vy[i] = (a0 + b1) * 0.5f;
      b1 = b0;
      i2 += 2;
    }
    i2 = 0;
    for (int i = kFramesPerBlock / 2; i < kFramesPerBlock; ++i)
    {
      float a0 = apa1.processSample(apa0.processSample(vx2[i2]));
      float b0 = apb1.processSample(apb0.processSample(vx2[i2 + 1]));
      vy[i] = (a0 + b1) * 0.5f;
      b1 = b0;
      i2 += 2;
    }
    return vy;
  }

  void clear()
  {
    apa0.clear();
    apa1.clear();
    apb0.clear();
    apb1.clear();
    b1 = 0;
  }

 private:
  // order=4, rejection=70dB, transition band=0.1.
  Allpass1 apa0{0.07986642623635751f}, apa1{0.5453536510711322f}, apb0{0.28382934487410993f},
      apb1{0.8344118914807379f};
  float b1{0};
};

// Downsampler
// a cascade of half band filters, one for each octave.
// TODO this is complicated. replace with single-channel version then use Bank<Downsampler> for
// multiple channels?
class Downsampler
{
  std::vector<HalfBandFilter> _filters;
  std::vector<float> _buffers;
  int _octaves;
  int _numBuffers;
  uint32_t _counter{0};

  float* bufferPtr(int idx) { return _buffers.data() + idx * kFramesPerBlock; }

 public:
  Downsampler(int octavesDown) : _octaves(octavesDown)
  {
    if (_octaves)
    {
      // one pair of buffers for each octave plus one output buffer.
      _numBuffers = 2 * _octaves + 1;

      // each octave uses one filter.
      _filters.resize(_octaves);

      // get all buffers as a single contiguous array of floats.
      _buffers.resize(kFramesPerBlock * _numBuffers);

      clear();
    }
  }
  ~Downsampler() = default;

  // write a vector of samples to the filter chain, run filters, and return
  // true if there is a new vector of output to read (every 2^octaves writes)
  bool write(SignalBlock v)
  {
    if (_octaves)
    {
      // write input to one of first two buffers
      const float* pSrc = v.data();
      float* pDest = bufferPtr(_counter & 1);
      std::copy(pSrc, pSrc + kFramesPerBlock, pDest);

      // look at the bits of the counter from lowest to highest.
      // there is one bit for each octave of downsampling.
      // each octave is run if its bit and all lesser bits are 1.
      uint32_t mask = 1;
      for (int h = 0; h < _octaves; ++h)
      {
        bool b0 = _counter & mask;
        if (!b0) break;
        mask <<= 1;
        bool b1 = _counter & mask;

        // run filter
        HalfBandFilter* f = &(_filters[h]);
        SignalBlock vSrc1(bufferPtr(h * 2));
        SignalBlock vSrc2(bufferPtr(h * 2 + 1));
        SignalBlock vDest = f->downsample(vSrc1, vSrc2);
        store(vDest, bufferPtr(h * 2 + 2 + b1));
      }

      // advance and wrap counter. If it's back to 0, we have output
      uint32_t counterMask = (1 << _octaves) - 1;
      _counter = (_counter + 1) & counterMask;
      return (_counter == 0);
    }
    else
    {
      // write input to final buffer
      const float* pSrc = v.data();
      float* pDest = bufferPtr(_numBuffers - 1);
      std::copy(pSrc, pSrc + kFramesPerBlock, pDest);
      return true;
    }
  }

  SignalBlock read() { return SignalBlock(bufferPtr(_numBuffers - 1)); }

  void clear()
  {
    for (auto& f : _filters)
    {
      f.clear();
    }
    std::fill(_buffers.begin(), _buffers.end(), 0.f);
    _counter = 0;
  }
};

struct Upsampler
{
  std::vector<HalfBandFilter> _filters;
  std::vector<float> _buffers;
  int _octaves;
  int _numBuffers;
  int readIdx_{0};

  float* bufferPtr(int idx) { return _buffers.data() + idx * kFramesPerBlock; }

  Upsampler(int octavesUp) : _octaves(octavesUp)
  {
    if (_octaves)
    {
      _numBuffers = 1 << _octaves;
      size_t numFilters = _octaves;
      _filters.resize(numFilters);

      // get all buffers as a single contiguous array of floats.
      _buffers.resize(kFramesPerBlock * _numBuffers);

      clear();
    }
  }
  ~Upsampler() = default;

  void write(SignalBlock x)
  {
    // write to last vector in buffer
    store(x, bufferPtr(_numBuffers - 1));

    // for each octave of upsampling, upsample blocks to twice as many, in place, ending at buffers
    // end
    for (int j = 0; j < _octaves; ++j)
    {
      int sourceBufs = 1 << j;
      int destBufs = sourceBufs << 1;
      int srcStart = _numBuffers - sourceBufs;
      int destStart = _numBuffers - destBufs;

      for (int i = 0; i < sourceBufs; ++i)
      {
        SignalBlock src, dest1, dest2;
        load(src, bufferPtr(srcStart + i));
        dest1 = _filters[j].upsampleFirstHalf(src);
        dest2 = _filters[j].upsampleSecondHalf(src);
        store(dest1, bufferPtr(destStart + (i * 2)));
        store(dest2, bufferPtr(destStart + (i * 2) + 1));
      }
    }
    readIdx_ = 0;
  }

  // after a write, 1 << octaves reads are available.
  SignalBlock read()
  {
    SignalBlock result;
    load(result, bufferPtr(readIdx_++));
    return result;
  }

  void clear()
  {
    for (auto& f : _filters)
    {
      f.clear();
    }
    std::fill(_buffers.begin(), _buffers.end(), 0.f);
    readIdx_ = 0;
  }
};

// From an input clock phasor and an output/input frequency ratio,
// produce an output clock at the given ratio that is phase-synched with the input.
//
class TempoLock
{
  // phasor on [0. - 1.), changes at rate of input phasor * input ratio
  float _omega{-1.f};  // current output phase
  float _x1v{0};       // input one vector ago

 public:
  // phase of -1 means we are stopped.
  void clear() { _omega = -1.0f; }

  // function call takes 3 inputs:
  // x: the input phasor to follow
  // dydx: the ratio to the input at which to lock the output phasor
  // isr: inverse of sample rate
  SignalBlock operator()(SignalBlock x, float dydx, float isr)
  {
    SignalBlock y;
    float x0 = x[0];
    float dxdt{0.f};
    float dydt{0.f};

    // if input phasor is inactive, reset and output 0.
    // we check against -1 because a running input phasor may be slightly
    // less than zero.
    if (x0 == -1.0f)
    {
      clear();
      y = SignalBlock(0.f);
    }
    else
    {
      // get dxdt and dydt from input and ratio
      if (_omega > -1.f)
      {
        // if we are already running: get average input slope every vector
        float dx = x0 - _x1v;
        if (dx < 0.f) dx += 1.f;
        dxdt = dx / kFramesPerBlock;
        dydt = dxdt * dydx;
        _x1v = x0;
      }
      else
      {
        // on startup: we are active but phase is unknown, so jump to
        // current phase based on input.
        dxdt = x[1] - x0;
        dydt = dxdt * dydx;
        _x1v = x0 - dxdt * kFramesPerBlock;
        _omega = fmod(x0 * dydx, 1.0f);
      }

      // if the ratio of its reciprocal is close to an integer, lock to input phase
      bool lock{false};
      constexpr float lockDist = 0.001f;
      if (fabs(dydx - roundf(dydx)) < lockDist) lock = true;
      float rdydx = 1.0f / dydx;
      if (fabs(rdydx - roundf(rdydx)) < lockDist) lock = true;

      if (lock)
      {
        // get error term at each vector by comparing output to scaled input
        // or scaled input to output depending on ratio.
        float ref, refWrap, error;
        if (dydx >= 1.f)
        {
          ref = x0 * dydx;
          refWrap = ref - floorf(ref);
          error = _omega - refWrap;
        }
        else
        {
          ref = _omega / dydx;
          refWrap = ref - floorf(ref);
          error = refWrap - x0;
        }

        // get error difference from closest sync target
        float errorDiff = roundf(error) - error;

        // add error correction term to dydt. Note that this is only added to the current vector.
        // this is different from a traditional PLL, which would need a filter in the feedback loop.
        //
        // this addition tweaks the slope to reach the target value in 1/4 second. However as
        // the target gets closer the slope is less, resulting in an exponentially slowing approach.
        float correction = errorDiff * isr * 4.0f;

        // don't allow going under 0.5x or over 2x speed
        correction = clamp(correction, -dydt * 0.5f, dydt * 1.0f);
        dydt += correction;
      }

      // make output vector with sample-accurate wrap
      for (int i = 0; i < kFramesPerBlock; ++i)
      {
        y[i] = _omega;
        _omega += dydt;
        if (_omega > 1.0f) _omega -= 1.0f;
      }
    }
    return y;
  }
};

}  // namespace ml

