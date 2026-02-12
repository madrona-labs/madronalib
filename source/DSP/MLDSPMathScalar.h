// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// templates for common scalar math functions on int, float, double.
// other small scalar utilities.

#pragma once

//#include <cmath>
#include <stdint.h>
#include <stdlib.h>

#include <limits>

#ifdef WIN32
#undef min
#undef max
#endif

namespace ml
{
constexpr float kTwoPi = 6.2831853071795864769252867f;
constexpr float kPi = 3.1415926535897932384626433f;
constexpr float kOneOverTwoPi = 1.0f / kTwoPi;
constexpr float kE = 2.718281828459045f;
constexpr float kTwelfthRootOfTwo = 1.05946309436f;
constexpr float kMinGain = 0.00001f;  // 10e-5 = -120dB

// return the exponent of the smallest power of 2 that is >= x.
inline size_t bitsToContain(int x)
{
  int exp;
  for (exp = 0; (1 << exp) < x; exp++);
  return (exp);
}

// return the smallest multiple of 2^N equal to or larger to x.
inline int chunkSizeToContain(int chunkSizeExponent, int x)
{
  const int chunkSize = 1 << chunkSizeExponent;
  const int chunkMask = ~(chunkSize - 1);
  return ((x + (chunkSize - 1)) & chunkMask);
}

// modulo for positive and negative integers
inline int modulo(int a, int b) { return a >= 0 ? (a % b) : (b - std::abs(a % b)) % b; }

// modulo for floats
inline float modulo(float a, float b) { return a - b * floor(a / b); }

// ----------------------------------------------------------------
#pragma mark scalar-type templates


template <class c>
constexpr inline std::enable_if_t<std::is_scalar_v<c>, c>
min(const c& a, const c& b)
{
  return (a < b) ? a : b;
}

template <class c>
constexpr inline std::enable_if_t<std::is_scalar_v<c>, c>
max(const c& a, const c& b)
{
  return (a > b) ? a : b;
}

// clamp to closed interval [min, max] - only for scalar types
template <class c>
constexpr inline std::enable_if_t<std::is_scalar_v<c>, c>
clamp(const c& x, const c& min, const c& max)
{
  return (x < min) ? min : (x > max ? max : x);
}

template <class c>
constexpr inline c lerp(const c& a, const c& b, const c& m)
{
  return (a + m * (b - a));
}

// return bool value of within half-open interval [min, max).
template <class c>
constexpr inline bool within(const c& x, const c& min, const c& max)
{
  return ((x >= min) && (x < max));
}

// return bool value of within closed interval [min, max].
template <class c>
constexpr inline bool withinClosedInterval(const c& x, const c& min, const c& max)
{
  return ((x >= min) && (x <= max));
}

template <class c>
constexpr inline std::enable_if_t<std::is_scalar_v<c>, c>
sign (const c& x)
{
  return (x == 0) ? 0 : ((x > 0) ? 1 : -1);
}

#pragma mark utility functions on scalars

inline int ilog2(int x)
{
  int b = 0;
  if (x >= 1 << 16)
  {
    x >>= 16;
    b |= 16;
  }
  if (x >= 1 << 8)
  {
    x >>= 8;
    b |= 8;
  }
  if (x >= 1 << 4)
  {
    x >>= 4;
    b |= 4;
  }
  if (x >= 1 << 2)
  {
    x >>= 2;
    b |= 2;
  }
  if (x >= 1 << 1) b |= 1;
  return b;
}

inline int isNaN(float x) { return std::isnan(x); }

inline int isNaN(double x) { return std::isnan(x); }

inline int isInfinite(float x) { return std::isinf(x); }

inline int isInfinite(double x) { return std::isinf(x); }

inline float smoothstep(float a, float b, float x)
{
  x = clamp((x - a) / (b - a), 0.f, 1.f);
  return x * x * (3 - 2 * x);
}

// return bool as float 0. or 1.
inline float boolToFloat(uint32_t b)
{
  uint32_t temp = 0x3F800000 & (!b - 1);
  return *((float*)&temp);
}

// return sign bit of float as float, 1. for positive, 0. for negative.
inline float fSignBit(float f)
{
  uint32_t a = *((uint32_t*)&f);
  a = (((a & 0x80000000) >> 31) - 1) & 0x3F800000;
  return *((float*)&a);
}

inline float lerpBipolar(const float a, const float b, const float c, const float m)
{
  float absm = fabsf(m);  // TODO fast abs etc
  float pos = m > 0.;
  float neg = m < 0.;
  float q = pos * c + neg * a;
  return (b + (q - b) * absm);
}


// amp <-> dB conversions, where ratio of the given amplitude is to 1.
inline float ampTodB(float a) { return 20.f * log10f(a); }

inline float dBToAmp(float dB) { return powf(10.f, dB / 20.f); }

// tiny, bad random generator
class RandomScalarSource
{
 public:
  RandomScalarSource() : seed_(0) {}
  ~RandomScalarSource() {}
  inline void step() { seed_ = seed_ * 0x0019660D + 0x3C6EF35F; }

  // return single-precision floating point number on [-1, 1]
  float getFloat()
  {
    step();
    uint32_t temp = (seed_ >> 9) & 0x007FFFFF;
    temp &= 0x007FFFFF;
    temp |= 0x3F800000;
    float* pf = reinterpret_cast<float*>(&temp);
    *pf *= 2.f;
    *pf -= 3.f;
    return *pf;
  }

  // return 32 pseudorandom bits
  uint32_t getUInt32()
  {
    step();
    return seed_;
  }
  uint32_t seed_;
};

}  // namespace ml
