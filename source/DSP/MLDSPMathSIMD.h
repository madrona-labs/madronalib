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
// Utilities

inline float4 clamp(float4 a, float4 b, float4 c) { return min(max(a, b), c); }

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
// Shuffle and transpose

inline float4 vecShuffleRight(float4 v1, float4 v2) {
  return shuffle<3, 0, 1, 2>(shuffle<0, 0, 3, 3>(v2, v1), v2);
}

inline float4 vecShuffleLeft(float4 v1, float4 v2) {
  return shuffle<1, 2, 0, 3>(v1, shuffle<3, 3, 0, 0>(v1, v2));
}

inline void transpose4x4InPlace(float4* ptr) {
  float4 r0 = ptr[0];
  float4 r1 = ptr[1];
  float4 r2 = ptr[2];
  float4 r3 = ptr[3];
  
  float4 t0 = unpackLo(r0, r1);
  float4 t1 = unpackLo(r2, r3);
  float4 t2 = unpackHi(r0, r1);
  float4 t3 = unpackHi(r2, r3);
  
  ptr[0] = moveLH(t0, t1);
  ptr[1] = moveHL(t1, t0);
  ptr[2] = moveLH(t2, t3);
  ptr[3] = moveHL(t3, t2);
}

inline void transpose4x4AndSwap(float4* ptrA, float4* ptrB) {
  // Load from A and B
  float4 a0 = ptrA[0];
  float4 a1 = ptrA[1];
  float4 a2 = ptrA[2];
  float4 a3 = ptrA[3];
  
  float4 b0 = ptrB[0];
  float4 b1 = ptrB[1];
  float4 b2 = ptrB[2];
  float4 b3 = ptrB[3];
  
  // Transpose A
  float4 t0 = unpackLo(a0, a1);
  float4 t1 = unpackLo(a2, a3);
  float4 t2 = unpackHi(a0, a1);
  float4 t3 = unpackHi(a2, a3);
  
  float4 ta0 = moveLH(t0, t1);
  float4 ta1 = moveHL(t1, t0);
  float4 ta2 = moveLH(t2, t3);
  float4 ta3 = moveHL(t3, t2);
  
  // Transpose B
  t0 = unpackLo(b0, b1);
  t1 = unpackLo(b2, b3);
  t2 = unpackHi(b0, b1);
  t3 = unpackHi(b2, b3);
  
  float4 tb0 = moveLH(t0, t1);
  float4 tb1 = moveHL(t1, t0);
  float4 tb2 = moveLH(t2, t3);
  float4 tb3 = moveHL(t3, t2);
  
  // Swap: write transposed A to B, transposed B to A
  ptrB[0] = ta0;
  ptrB[1] = ta1;
  ptrB[2] = ta2;
  ptrB[3] = ta3;
  
  ptrA[0] = tb0;
  ptrA[1] = tb1;
  ptrA[2] = tb2;
  ptrA[3] = tb3;
}
