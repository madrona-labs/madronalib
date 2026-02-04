// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// MLDSPMathSSE.h
// SSE implementations of madronalib SIMD primitives

#include "MLPlatform.h"

// SSE 4.1 required
#include <smmintrin.h>

#include <float.h>
#include <assert.h>

#pragma once

// ----------------------------------------------------------------
// Type definitions

struct float4 {
  __m128 v;
  
  float4() = default;
  explicit float4(__m128 x) : v(x) {}
  float4(float x) : v(_mm_set1_ps(x)) {}
  explicit float4(float a, float b, float c, float d) : v(_mm_setr_ps(a, b, c, d)) {}
  operator __m128() const { return v; }
};

struct int4 {
  __m128i v;
  
  int4() = default;
  explicit int4(__m128i x) : v(x) {}
  int4(int32_t x) : v(_mm_set1_epi32(x)) {}
  explicit int4(int32_t a, int32_t b, int32_t c, int32_t d) : v(_mm_setr_epi32(a, b, c, d)) {}
  operator __m128i() const { return v; }
};


// ----------------------------------------------------------------
// Load/store functions
inline float4 loadFloat4(const float* ptr) { return float4(_mm_load_ps(ptr)); }
inline void storeFloat4(float* ptr, float4 v) { _mm_store_ps(ptr, v); }

inline int4 loadInt4(const int32_t* ptr) { return int4(_mm_load_si128((const __m128i*)ptr)); }
inline void storeInt4(int32_t* ptr, int4 v) { _mm_store_si128((__m128i*)ptr, v); }

// ----------------------------------------------------------------
// Lane access (slow - avoid!)

inline float getFloat4Lane(float4 v, size_t lane) {
  assert(lane < 4);
  alignas(16) float tmp[4];
  _mm_store_ps(tmp, v);
  return tmp[lane];
}

inline void setFloat4Lane(float4& v, size_t lane, float val) {
  assert(lane < 4);
  alignas(16) float tmp[4];
  _mm_store_ps(tmp, v);
  tmp[lane] = val;
  v = float4(_mm_load_ps(tmp));
}

// ----------------------------------------------------------------
// Arithmetic operators for float4

inline float4 operator+(float4 a, float4 b) { return float4(_mm_add_ps(a, b)); }
inline float4 operator-(float4 a, float4 b) { return float4(_mm_sub_ps(a, b)); }
inline float4 operator*(float4 a, float4 b) { return float4(_mm_mul_ps(a, b)); }
inline float4 operator/(float4 a, float4 b) { return float4(_mm_div_ps(a, b)); }

inline float4& operator+=(float4& a, float4 b) { a = a + b; return a; }
inline float4& operator-=(float4& a, float4 b) { a = a - b; return a; }
inline float4& operator*=(float4& a, float4 b) { a = a * b; return a; }
inline float4& operator/=(float4& a, float4 b) { a = a / b; return a; }

inline float4 operator-(float4 a) {
  return float4(_mm_xor_ps(a, _mm_set1_ps(-0.0f)));
}

// ----------------------------------------------------------------
// Arithmetic operators for int4

inline int4 operator+(int4 a, int4 b) { return int4(_mm_add_epi32(a, b)); }
inline int4 operator-(int4 a, int4 b) { return int4(_mm_sub_epi32(a, b)); }

inline int4& operator+=(int4& a, int4 b) { a = a + b; return a; }
inline int4& operator-=(int4& a, int4 b) { a = a - b; return a; }

inline int4 operator-(int4 a) {
  return int4(_mm_sub_epi32(_mm_setzero_si128(), a));
}

// ----------------------------------------------------------------
// Math functions

inline float4 min(float4 a, float4 b) { return float4(_mm_min_ps(a, b)); }
inline float4 max(float4 a, float4 b) { return float4(_mm_max_ps(a, b)); }
inline float4 sqrt(float4 a) { return float4(_mm_sqrt_ps(a)); }
inline float4 rsqrt(float4 a) { return float4(_mm_rsqrt_ps(a)); }
inline float4 rcp(float4 a) { return float4(_mm_rcp_ps(a)); }

// multiply-add
inline float4 multiplyAdd(float4 a, float4 b, float4 c) { return float4(_mm_add_ps(_mm_mul_ps(a, b), c)); }

// Float logical
inline float4 andBits(float4 a, float4 b) { return float4(_mm_and_ps(a, b)); }
inline float4 andNotBits(float4 a, float4 b) { return float4(_mm_andnot_ps(a, b)); }
inline float4 orBits(float4 a, float4 b) { return float4(_mm_or_ps(a, b)); }
inline float4 xorBits(float4 a, float4 b) { return float4(_mm_xor_ps(a, b)); }

// Float comparisons (return float4 masks)
inline float4 compareEqual(float4 a, float4 b) { return float4(_mm_cmpeq_ps(a, b)); }
inline float4 compareNotEqual(float4 a, float4 b) { return float4(_mm_cmpneq_ps(a, b)); }
inline float4 compareGreaterThan(float4 a, float4 b) { return float4(_mm_cmpgt_ps(a, b)); }
inline float4 compareGreaterThanOrEqual(float4 a, float4 b) { return float4(_mm_cmpge_ps(a, b)); }
inline float4 compareLessThan(float4 a, float4 b) { return float4(_mm_cmplt_ps(a, b)); }
inline float4 compareLessThanOrEqual(float4 a, float4 b) { return float4(_mm_cmple_ps(a, b)); }

// Float special
inline float4 setZero() { return float4(_mm_setzero_ps()); }
inline float4 set1Float(float a) { return float4(_mm_set1_ps(a)); }
inline float4 setrFloat(float a, float b, float c, float d) { return float4(_mm_setr_ps(a, b, c, d)); }

// Float shuffle/move
template<int i0, int i1, int i2, int i3>
inline float4 shuffle(float4 a, float4 b) {
  return float4(_mm_shuffle_ps(a, b, _MM_SHUFFLE(i3, i2, i1, i0)));
}
inline float4 unpackLo(float4 a, float4 b) { return float4(_mm_unpacklo_ps(a, b)); }
inline float4 unpackHi(float4 a, float4 b) { return float4(_mm_unpackhi_ps(a, b)); }
inline float4 moveLH(float4 a, float4 b) { return float4(_mm_movelh_ps(a, b)); }
inline float4 moveHL(float4 a, float4 b) { return float4(_mm_movehl_ps(a, b)); }

// Scalar operations (operate on lowest element)
inline float4 addScalar(float4 a, float4 b) { return float4(_mm_add_ss(a, b)); }
inline float4 maxScalar(float4 a, float4 b) { return float4(_mm_max_ss(a, b)); }
inline float4 minScalar(float4 a, float4 b) { return float4(_mm_min_ss(a, b)); }
inline float extractScalar(float4 a) { return _mm_cvtss_f32(a); }

// Integer arithmetic
inline int4 multiplyUnsigned(int4 a, int4 b) { return int4(_mm_mullo_epi32(a, b)); }

// Integer logical
inline int4 andBits(int4 a, int4 b) { return int4(_mm_and_si128(a, b)); }
inline int4 andNotBits(int4 a, int4 b) { return int4(_mm_andnot_si128(a, b)); }
inline int4 orBits(int4 a, int4 b) { return int4(_mm_or_si128(a, b)); }
inline int4 xorBits(int4 a, int4 b) { return int4(_mm_xor_si128(a, b)); }

// Integer special
inline int4 setZeroInt() { return int4(_mm_setzero_si128()); }
inline int4 set1Int(int32_t a) { return int4(_mm_set1_epi32(a)); }
inline int4 setInt(int32_t a, int32_t b, int32_t c, int32_t d) {
  return int4(_mm_set_epi32(d, c, b, a));
}
inline int4 setrInt(int32_t a, int32_t b, int32_t c, int32_t d) {
  return int4(_mm_setr_epi32(a, b, c, d));
}

// Integer shifts (byte shifts) - require compile-time constants
template<int count>
inline int4 shiftLeftBytes(int4 a) { return int4(_mm_slli_si128(a, count)); }

template<int count>
inline int4 shiftRightBytes(int4 a) { return int4(_mm_srli_si128(a, count)); }

// Integer shifts (element shifts)
inline int4 shiftLeftElements(int4 a, int count) { return int4(_mm_slli_epi32(a, count)); }
inline int4 shiftRightElements(int4 a, int count) { return int4(_mm_srli_epi32(a, count)); }

// Integer comparisons
inline int4 compareEqualInt(int4 a, int4 b) { return int4(_mm_cmpeq_epi32(a, b)); }

// Conversions
inline int4 floatToIntRound(float4 a) { return int4(_mm_cvtps_epi32(a)); }
inline int4 floatToIntTruncate(float4 a) { return int4(_mm_cvttps_epi32(a)); }
inline float4 intToFloat(int4 a) { return float4(_mm_cvtepi32_ps(a)); }

// _mm_cvtepi32_ps approximation for unsigned int data
inline float4 unsignedIntToFloat(int4 v) {
  int4 v_hi = shiftRightElements(v, 1);
  float4 v_hi_flt = intToFloat(v_hi);
  return v_hi_flt + v_hi_flt;
}

// Casts (reinterpret bits)
inline int4 castFloatToInt(float4 a) { return int4(_mm_castps_si128(a)); }
inline float4 castIntToFloat(int4 a) { return float4(_mm_castsi128_ps(a)); }

// ----------------------------------------------------------------
// select functions

inline float4 vecSelectFFI(float4 a, float4 b, int4 conditionMask) {
  int4 ones = set1Int(-1);
  return orBits(andBits(castIntToFloat(conditionMask), a),
                andBits(xorBits(castIntToFloat(conditionMask), castIntToFloat(ones)), b));
}

inline float4 vecSelectFFF(float4 a, float4 b, float4 conditionMask) {
  int4 ones = set1Int(-1);
  return orBits(andBits(conditionMask, a),
                andBits(xorBits(conditionMask, castIntToFloat(ones)), b));
}

inline int4 vecSelectIII(int4 a, int4 b, int4 conditionMask) {
  int4 ones = set1Int(-1);
  return orBits(andBits(conditionMask, a),
                andBits(xorBits(conditionMask, ones), b));
}

// ----------------------------------------------------------------
// horizontal operations returning float

inline float vecSumH(float4 v) {
  float4 tmp0 = v + moveHL(v, v);
  float4 tmp1 = addScalar(tmp0, shuffle<1,1,1,1>(tmp0, tmp0));
  return extractScalar(tmp1);
}

inline float vecMaxH(float4 v) {
  float4 tmp0 = max(v, moveHL(v, v));
  float4 tmp1 = maxScalar(tmp0, shuffle<1,1,1,1>(tmp0, tmp0));
  return extractScalar(tmp1);
}

inline float vecMinH(float4 v) {
  float4 tmp0 = min(v, moveHL(v, v));
  float4 tmp1 = minScalar(tmp0, shuffle<1,1,1,1>(tmp0, tmp0));
  return extractScalar(tmp1);
}

