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
#include "MLDSPMath.h"
#include "MLDSPFilters.h"

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



// Combining the integer delay and first order allpass section
// gives us an allpass-interpolated fractional delay. In general, modulating the
// delay time will change the allpass coefficient, producing clicks in the
// output.
class FractionalDelay
{
  IntegerDelay mIntegerDelay;
  Allpass1<float> mAllpassSection{0.f};
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
    mAllpassSection.coeffs = Allpass1<float>::makeCoeffs({delayFrac});
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
      vy[n] = mAllpassSection.nextFrame(mIntegerDelay.processSample(vx[n]));
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
      
      vy[n] = mAllpassSection.nextFrame(mIntegerDelay.processSample(vx[n]));
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
  
  FDN()
  {
    static constexpr size_t kDefaultMaxDelay{128};
    setMaxDelayInSamples(kDefaultMaxDelay);
  }

  void setMaxDelayInSamples(size_t d)
  {
    for (auto& delay : mDelays)
    {
      delay.setMaxDelayInSamples(d);
    }
  }
  
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


// OverlapAddFunction TODO
/*
 template<int LENGTH, int DIVISIONS, int IN_ROWS, int OUT_ROWS>
 class OverlapAddFunction
 {
 typedef std::function<SignalBlockArray<OUT_ROWS>(const
 SignalBlockArray<IN_ROWS>)> ProcessFn;
 
 public:
 inline SignalBlockArray<OUT_ROWS> operator()(ProcessFn fn, const
 SignalBlockArray<IN_ROWS> vx)
 {
 }
 
 private:
 //Matrix mHistory;
 const SignalBlock& mWindow;
 };
 */

// FeedbackDelayFunction
// Wraps a function in a pitchbendable delay with feedback per row.
// Since the feedback adds the output of the function to its input, the function
// must input and output the same number of rows.

// template<int ROWS>
class FeedbackDelayFunction
{
  static constexpr int ROWS = 1;  // see above
  using inputType = const SignalBlockArray<ROWS>;
  using outputType = SignalBlockArray<1>;  // ROWS
  using ProcessFn = std::function<outputType(inputType)>;
  
public:
  float feedbackGain{1.f};
  
  inline SignalBlockArray<ROWS> operator()(const SignalBlockArray<ROWS> vx, ProcessFn fn,
                                           const SignalBlock vDelayTime)
  {
    SignalBlockArray<ROWS> vFnOutput;
    vFnOutput = fn(vx + vy1 * SignalBlockArray<ROWS>(feedbackGain));
    
    for (int j = 0; j < ROWS; ++j)
    {
      vy1.setRow(j, mDelays[j](vFnOutput.getRow(j), vDelayTime - SignalBlock(kFramesPerBlock)));
    }
    return vFnOutput;
  }
  
private:
  std::array<PitchbendableDelay, ROWS> mDelays;
  SignalBlockArray<ROWS> vy1;
};

// FeedbackDelayFunctionWithTap
// Wraps a function in a pitchbendable delay with feedback per row. The function
// outputs a tap that can be different from the feedback signal sent to the
// input. Since the feedback adds the output of the function to its input, the
// function must input and output the same number of rows.

// template<int ROWS>
class FeedbackDelayFunctionWithTap
{
  static constexpr int ROWS = 1;  // see above
  using inputType = const SignalBlockArray<ROWS>;
  using tapType = SignalBlockArray<ROWS>&;
  using outputType = SignalBlockArray<1>;  // ROWS
  using ProcessFn = std::function<outputType(inputType, tapType)>;
  
public:
  float feedbackGain{1.f};
  
  inline SignalBlockArray<ROWS> operator()(const SignalBlockArray<ROWS> vx, ProcessFn fn,
                                           const SignalBlock vDelayTime)
  {
    SignalBlockArray<ROWS> vFeedback;
    SignalBlockArray<ROWS> vOutputTap;
    vFeedback = fn(vx + vy1 * SignalBlockArray<ROWS>(feedbackGain), vOutputTap);
    
    for (int j = 0; j < ROWS; ++j)
    {
      vy1.setRow(j, mDelays[j](vFeedback.getRow(j), vDelayTime - SignalBlock(kFramesPerBlock)));
    }
    return vOutputTap;
  }
  
private:
  std::array<PitchbendableDelay, ROWS> mDelays;
  SignalBlockArray<ROWS> vy1;
};


}  // namespace ml

