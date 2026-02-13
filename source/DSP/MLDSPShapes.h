// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#pragma once

#include <vector>

#include "MLDSPOps.h"

namespace ml
{

// ADSR envelope triggered and scaled by a single gate + amp signal.

struct ADSR
{
  static constexpr float bias = 0.1f;
  static constexpr float minSegmentTime = 0.0002f;

  struct Coeffs
  {
    float ka, kd, s, kr;
  };

  enum segments
  {
    A = 0,
    D = 1,
    S = 2,
    R = 3,
    off = 4
  };

  static Coeffs calcCoeffs(float a, float d, float s, float r, float sr)
  {
    const float invSr = 1.0f / sr;
    const float ka = kTwoPi * invSr / std::max(a, minSegmentTime);
    const float kd = kTwoPi * invSr / std::max(d, minSegmentTime);
    const float kr = kTwoPi * invSr / std::max(r, minSegmentTime);
    return {ka, kd, s, kr};
  }

  Coeffs coeffs{0};

  float y{0};          // current output
  float y1{0};         // previous output
  float x1{0};         // previous input
  float threshold{0};  // actual value to stop on
  float target{0};     // input to filter: value with bias added so we end in a finite time
  float k{0};          // IIR filter coefficient
  float amp{0};
  int segment{off};

  void clear() { segment = off; }

  inline float processSample(float x)
  {
    if ((segment == off) && (x == 0.f)) return 0.f;

    bool crossedThresh = ((y1 > threshold) != (y > threshold));
    bool recalc{false};

    // crossing threshold advances to next envelope segment
    if (crossedThresh && (segment < off))
    {
      segment++;  // << ?
      recalc = true;
    }

    bool trigOn = (x1 == 0.f) && (x > 0.f);
    bool trigOff = (x1 > 0.f) && (x == 0.f);

    // TODO use some bit twiddling
    if (trigOn)
    {
      segment = A;
      amp = x;
      recalc = true;
    }
    else if (trigOff)
    {
      segment = R;
      recalc = true;
    }

    if (recalc)
    {
      float startEnv, endEnv;
      switch (segment)
      {
        case A:
        {
          startEnv = 0.f;
          endEnv = 1.f;
          k = coeffs.ka;
          break;
        }
        case D:
        {
          startEnv = 1.f;
          endEnv = coeffs.s;
          k = coeffs.kd;
          break;
        }
        case S:
        {
          startEnv = coeffs.s;
          endEnv = coeffs.s;
          k = 0.f;
          y1 = coeffs.s;
          y = coeffs.s;
          break;
        }
        case R:
        {
          startEnv = coeffs.s;
          endEnv = 0.f;
          k = coeffs.kr;
          break;
        }
        case off:
        {
          startEnv = 0.f;
          endEnv = 0.f;
          k = 0.f;
          y1 = 0.f;
          y = 0.f;
          break;
        }
      }

      float segmentBias = (endEnv - startEnv) * bias;
      threshold = endEnv;
      target = endEnv + segmentBias;
    }

    // history and IIR filter
    x1 = x;
    y1 = y;
    y = y + k * (target - y);

    // scale by amp
    return y * amp;
  }

  inline SignalBlock operator()(const SignalBlock vx)
  {
    SignalBlock r;
    for (int i = 0; i < kFramesPerBlock; ++i)
    {
      r[i] = processSample(vx[i]);
    }
    return r;
  }
};


// Ramp, when triggered, makes a single ramp from 0-1 then resets to 0. The speed
// of the ramp is a signal input, giving a ramp with the same speed as PhasorGen.
struct Ramp
{
  static constexpr uint32_t start = 0;
  uint32_t mOmega32{start};
  uint32_t mGate{0};
  uint32_t mOmegaPrev{start};

  void trigger()
  {
    mOmega32 = mOmegaPrev = start;
    mGate = 1;
  }
  
  static constexpr float stepsPerCycle{static_cast<float>(1UL << 32UL)};
  static constexpr float cyclesPerStep{1.f / stepsPerCycle};
  
  SignalBlock operator()(const SignalBlock cyclesPerSample)
  {
    // calculate int steps per sample
    SignalBlock stepsPerSampleV = cyclesPerSample * SignalBlock(stepsPerCycle);
    SignalBlockInt intStepsPerSampleV = roundFloatToInt(stepsPerSampleV);
    
    // accumulate 32-bit phase with wrap
    // we test for wrap at every sample to get a clean ending
    SignalBlockInt omega32V;
    for (int n = 0; n < kFramesPerBlock; ++n)
    {
      mOmega32 += intStepsPerSampleV[n] * mGate;
      if (mOmega32 < mOmegaPrev)
      {
        mGate = 0;
        mOmega32 = start;
      }
      omega32V[n] = mOmegaPrev = mOmega32;
    }
    // convert counter to float output range
    return unsignedIntToFloat(omega32V) * SignalBlock(cyclesPerStep);
  }
  
  float nextSample(const float cyclesPerSample)
  {
    // calculate int steps per sample
    float stepsPerSample = cyclesPerSample * stepsPerCycle;
    uint32_t intStepsPerSample = (uint32_t)roundf(stepsPerSample);
    
    // accumulate 32-bit phase with wrap
    // we test for wrap at every sample to get a clean ending
    SignalBlockInt omega32V;
    
    mOmega32 += intStepsPerSample * mGate;
    if (mOmega32 < mOmegaPrev)
    {
      mGate = 0;
      mOmega32 = start;
    }
    mOmegaPrev = mOmega32;
    
    // convert counter to float output range
    return mOmega32 * cyclesPerStep;
  }
};


// ----------------------------------------------------------------
// Interpolator1

// linear interpolate over signal length to next value.

constexpr float unityRampFn(size_t i) { return (i + 1) / static_cast<float>(kFramesPerBlock); }
constexpr SignalBlock kUnityRampVec{unityRampFn};

struct Interpolator1
{
  float currentValue{0};
  
  SignalBlock operator()(float f)
  {
    float dydt = f - currentValue;
    SignalBlock outputVec = SignalBlock(currentValue) + kUnityRampVec * dydt;
    currentValue = f;
    return outputVec;
  }
};

// TODO needs test

// ----------------------------------------------------------------
// LinearGlide

// convert a scalar float input into a SignalBlock with linear slew.
// to allow optimization, glide time is quantized to SignalBlocks.

class LinearGlide
{
  SignalBlock mCurrVec{0.f};
  SignalBlock mStepVec{0.f};
  float mTargetValue{0};
  float mDyPerVector{1.f / 32};
  int mVectorsPerGlide{32};
  int mVectorsRemaining{-1};
  
public:
  void setGlideTimeInSamples(float t)
  {
    mVectorsPerGlide = static_cast<int>(t / kFramesPerBlock);
    if (mVectorsPerGlide < 1) mVectorsPerGlide = 1;
    mDyPerVector = 1.0f / (mVectorsPerGlide + 0.f);
  }
  
  // set the current value to the given value immediately, without gliding
  void setValue(float f)
  {
    mTargetValue = f;
    mVectorsRemaining = 0;
  }
  
  SignalBlock operator()(float f)
  {
    // set target value if different from current value.
    // const float currentValue = mCurrVec[kFramesPerBlock - 1];
    if (f != mTargetValue)
    {
      mTargetValue = f;
      
      // start counter
      mVectorsRemaining = mVectorsPerGlide;
    }
    
    // process glide
    if (mVectorsRemaining < 0)
    {
      // do nothing
    }
    else if (mVectorsRemaining == 0)
    {
      // end glide: write target value to output vector
      mCurrVec = SignalBlock(mTargetValue);
      mStepVec = SignalBlock(0.f);
      mVectorsRemaining--;
    }
    else if (mVectorsRemaining == mVectorsPerGlide)
    {
      // start glide: get change in output value per vector
      float currentValue = mCurrVec[kFramesPerBlock - 1];
      float dydv = (mTargetValue - currentValue) * mDyPerVector;
      
      // get constant step vector
      mStepVec = SignalBlock(dydv);
      
      // setup current vector with first interpolation ramp.
      mCurrVec = SignalBlock(currentValue) + kUnityRampVec * mStepVec;
      
      mVectorsRemaining--;
    }
    else
    {
      // continue glide
      // Note that repeated adding will create some error in target value.
      // Because we return the target value explicity when we are done, this
      // won't be a problem in reasonably short glides.
      mCurrVec += mStepVec;
      mVectorsRemaining--;
    }
    
    return mCurrVec;
  }
  
  void clear()
  {
    mCurrVec = 0.f;
    mStepVec = 0.f;
    mTargetValue = 0.f;
    mVectorsRemaining = -1;
  }
};

class SampleAccurateLinearGlide
{
  float mCurrValue{0.f};
  float mStepValue{0.f};
  float mTargetValue{0.f};
  int mSamplesPerGlide{32};
  float mDyPerSample{1.f / 32};
  int mSamplesRemaining{-1};
  
public:
  void setGlideTimeInSamples(float t)
  {
    mSamplesPerGlide = static_cast<int>(t);
    if (mSamplesPerGlide < 1) mSamplesPerGlide = 1;
    mDyPerSample = 1.0f / mSamplesPerGlide;
  }
  
  // set the current value to the given value immediately, without gliding
  void setValue(float f)
  {
    mTargetValue = f;
    mSamplesRemaining = 0;
  }
  
  float nextSample(float f)
  {
    // set target value if different from current value.
    // const float currentValue = mCurrVec[kFramesPerBlock - 1];
    if (f != mTargetValue)
    {
      mTargetValue = f;
      
      // start counter
      mSamplesRemaining = mSamplesPerGlide;
    }
    
    // process glide
    if (mSamplesRemaining < 0)
    {
      // do nothing
    }
    else if (mSamplesRemaining == 0)
    {
      // end glide: write target value to output vector
      mCurrValue = (mTargetValue);
      mStepValue = (0.f);
      mSamplesRemaining--;
    }
    else if (mSamplesRemaining == mSamplesPerGlide)
    {
      // start glide: get change in output value per sample
      mStepValue = (mTargetValue - mCurrValue) * mDyPerSample;
      mSamplesRemaining--;
    }
    else
    {
      // continue glide
      // Note that repeated adding will create some error in target value.
      // Because we return the target value explicity when we are done, this
      // won't be a problem in reasonably short glides.
      mCurrValue += mStepValue;
      mSamplesRemaining--;
    }
    
    return mCurrValue;
  }
  void clear()
  {
    mCurrValue = 0.f;
    mStepValue = 0.f;
    mTargetValue = 0.f;
    mSamplesRemaining = -1;
  }
};


// more shapes:
/*
 Envelopes
 Sequence
 LFO
 LFO 2D
 Slew
 Up / down slew
 Wander
 Shepard curves
 Gravity
 Gears
 Pendulums
 Ping pong
 Vortices
 Quantize
 Voltages to rhythm (8 gates?) - like comparators that put out a pulse
 */

}  // namespace ml

