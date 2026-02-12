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

