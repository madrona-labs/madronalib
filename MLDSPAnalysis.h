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

namespace ml
{
// use this, not dBToAmp for calculating filter gain parameter A.
inline float dBToGain(float dB) { return powf(10.f, dB / 40.f); }

// Differentiator

class Differentiator
{
  float _x1{0};

 public:
  inline SignalBlock operator()(const SignalBlock vx)
  {
    SignalBlock vy;
    vy[0] = vx[0] - _x1;

    // TODO SIMD
    for (int n = 1; n < kFramesPerBlock; ++n)
    {
      vy[n] = vx[n] - vx[n - 1];
    }
    _x1 = vx[kFramesPerBlock - 1];
    return vy;
  }
};

// Integrator

class Integrator
{
  float y1{0};

 public:
  // set leak to a value such as 0.001 for stability
  float mLeak{0};

  inline SignalBlock operator()(const SignalBlock vx)
  {
    SignalBlock vy;
    for (int n = 0; n < kFramesPerBlock; ++n)
    {
      y1 -= y1 * mLeak;
      y1 += vx[n];
      vy[n] = y1;
    }
    return vy;
  }
};




// Peak with exponential decay

class Peak
{
  struct Coeffs
  {
    float a0, b1;
  };
  
  float y1{0};
  int peakHoldCounter{0};
  
public:
  Coeffs coeffs{0};
  int peakHoldSamples{44100};
  
  static Coeffs makeCoeffs(float omega)
  {
    float x = expf(-omega * kTwoPi);
    return {1.f - x, x};
  }
  
  static Coeffs passthru() { return {1.f, 0.f}; }
  
  inline SignalBlock operator()(const SignalBlock vx)
  {
    SignalBlock vy;
    SignalBlock vxSquared = vx * vx;
    for (int n = 0; n < kFramesPerBlock; ++n)
    {
      if (vxSquared[n] > y1)
      {
        // set peak and reset counter
        y1 = vxSquared[n];
        peakHoldCounter = peakHoldSamples;
      }
      else
      {
        // decay
        if (peakHoldCounter <= 0)
        {
          y1 = coeffs.a0 * vxSquared[n] + coeffs.b1 * y1;
        }
      }
      vy[n] = y1;
    }
    
    if (peakHoldCounter > 0)
    {
      peakHoldCounter -= kFramesPerBlock;
    }
    
    // use sqrt approximation. Return 0 for inputs near 0.
    return select(sqrtApprox(vy), SignalBlock{0.f}, greaterThan(vy, SignalBlock{float(1e-20)}));
  }
};



// filtered RMS

class RMS
{
  struct Coeffs
  {
    float a0, b1;
  };

  float y1{0};

 public:
  Coeffs coeffs{0};

  static Coeffs makeCoeffs(float omega)
  {
    float x = expf(-omega * kTwoPi);
    return {1.f - x, x};
  }

  static Coeffs passthru() { return {1.f, 0.f}; }

  inline SignalBlock operator()(const SignalBlock vx)
  {
    SignalBlock vy;
    SignalBlock vxSquared = vx * vx;

    for (int n = 0; n < kFramesPerBlock; ++n)
    {
      y1 = coeffs.a0 * vxSquared[n] + coeffs.b1 * y1;
      vy[n] = y1;
    }

    // use sqrt approximation. Return 0 for inputs near 0.
    return select(sqrtApprox(vy), SignalBlock{0.f}, greaterThan(vy, SignalBlock{float(1e-20)}));
  }
};




}  // namespace ml

