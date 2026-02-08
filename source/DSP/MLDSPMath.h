// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#pragma once

// Load definitions for low-level SIMD math.
// These define float4, int4, and a bunch of operations on them.
// We are currently only using 4-element vectors on both SSE and NEON.

constexpr size_t kSIMDVectorElems{4};

#if (defined __ARM_NEON) || (defined __ARM_NEON__)
#include "MLDSPMathNEON.h" // NEON
#else
#include "MLDSPMathSSE.h" // SSE 4.1
#endif

// ----------------------------------------------------------------
// Simple functions that don't use intrinsics directly

inline float4 clamp(float4 a, float4 b, float4 c) { return min(max(a, b), c); }
inline void transpose4x4(float4& r0, float4& r1, float4& r2, float4& r3) {
  float4 t0 = unpackLo(r0, r1);
  float4 t1 = unpackLo(r2, r3);
  float4 t2 = unpackHi(r0, r1);
  float4 t3 = unpackHi(r2, r3);
  r0 = moveLH(t0, t1);
  r1 = moveHL(t1, t0);
  r2 = moveLH(t2, t3);
  r3 = moveHL(t3, t2);
}

inline std::ostream& operator<<(std::ostream& out, float4 x) {
  out << "[";
  out << getFloat4Lane(x, 0);
  out << ", ";
  out << getFloat4Lane(x, 1);
  out << ", ";
  out << getFloat4Lane(x, 2);
  out << ", ";
  out << getFloat4Lane(x, 3);
  out << "]";
  return out;
}

// ----------------------------------------------------------------
// Conversions

inline float4 vecIntPart(float4 val) {
  return intToFloat(floatToIntTruncate(val));
}

inline float4 vecFracPart(float4 val) {
  return val - vecIntPart(val);
}

// ----------------------------------------------------------------
// Shuffles

inline float4 vecShuffleRight(float4 v1, float4 v2) {
  return shuffle<3, 0, 1, 2>(shuffle<0, 0, 3, 3>(v2, v1), v2);
}

inline float4 vecShuffleLeft(float4 v1, float4 v2) {
  return shuffle<1, 2, 0, 3>(v1, shuffle<3, 3, 0, 0>(v1, v2));
}
