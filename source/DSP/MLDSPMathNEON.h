// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// MLDSPMathNEON.h
// Native NEON implementations of madronalib SIMD primitives

#pragma once

#include "MLPlatform.h"
#include <arm_neon.h>

#include <cstddef>
#include <ostream>
#include <float.h>
#include <assert.h>

// ----------------------------------------------------------------
// Type definitions

struct float4 {
  float32x4_t v;

  float4() = default;
  explicit float4(float32x4_t x) : v(x) {}
  float4(float x) : v(vdupq_n_f32(x)) {}
  explicit float4(float a, float b, float c, float d)
      : v(vcombine_f32(vset_lane_f32(b, vdup_n_f32(a), 1),
                       vset_lane_f32(d, vdup_n_f32(c), 1))) {}
  operator float32x4_t() const { return v; }
};

struct int4 {
  int32x4_t v;

  int4() = default;
  explicit int4(int32x4_t x) : v(x) {}
  int4(int32_t x) : v(vdupq_n_s32(x)) {}
  explicit int4(int32_t a, int32_t b, int32_t c, int32_t d)
      : v(vcombine_s32(vset_lane_s32(b, vdup_n_s32(a), 1),
                       vset_lane_s32(d, vdup_n_s32(c), 1))) {}
  operator int32x4_t() const { return v; }
};


// ----------------------------------------------------------------
// Arithmetic operators for float4

inline float4 operator+(float4 a, float4 b) { return float4(vaddq_f32(a.v, b.v)); }
inline float4 operator-(float4 a, float4 b) { return float4(vsubq_f32(a.v, b.v)); }
inline float4 operator*(float4 a, float4 b) { return float4(vmulq_f32(a.v, b.v)); }
inline float4 operator/(float4 a, float4 b) { return float4(vdivq_f32(a.v, b.v)); }

inline float4& operator+=(float4& a, float4 b) { a = a + b; return a; }
inline float4& operator-=(float4& a, float4 b) { a = a - b; return a; }
inline float4& operator*=(float4& a, float4 b) { a = a * b; return a; }
inline float4& operator/=(float4& a, float4 b) { a = a / b; return a; }

inline float4 operator-(float4 a) {
  return float4(vnegq_f32(a.v));
}

// ----------------------------------------------------------------
// Arithmetic operators for int4

inline int4 operator+(int4 a, int4 b) { return int4(vaddq_s32(a.v, b.v)); }
inline int4 operator-(int4 a, int4 b) { return int4(vsubq_s32(a.v, b.v)); }

inline int4& operator+=(int4& a, int4 b) { a = a + b; return a; }
inline int4& operator-=(int4& a, int4 b) { a = a - b; return a; }

inline int4 operator-(int4 a) {
  return int4(vnegq_s32(a.v));
}

// ----------------------------------------------------------------
// Math functions

inline float4 min(float4 a, float4 b) { return float4(vminq_f32(a.v, b.v)); }
inline float4 max(float4 a, float4 b) { return float4(vmaxq_f32(a.v, b.v)); }
inline float4 sqrt(float4 a) { return float4(vsqrtq_f32(a.v)); }

// rsqrt: hardware estimate + one Newton-Raphson step (~12-bit precision,
// matching SSE _mm_rsqrt_ps)
inline float4 rsqrt(float4 a) {
  float32x4_t e = vrsqrteq_f32(a.v);
  e = vmulq_f32(e, vrsqrtsq_f32(vmulq_f32(a.v, e), e));
  return float4(e);
}

// rcp: hardware estimate + one Newton-Raphson step (~12-bit precision,
// matching SSE _mm_rcp_ps)
inline float4 rcp(float4 a) {
  float32x4_t e = vrecpeq_f32(a.v);
  e = vmulq_f32(e, vrecpsq_f32(e, a.v));
  return float4(e);
}

// fused multiply-add: a*b + c in a single instruction
// note: typically your DSP code will not need this, because the compiler is very good at finding opportunities to fuse
// multiplies and adds when the -ffp-contract=fast flag is set. However, if other code prevents that flag from being
// set, this function provides a way to use the fused multiply-add instruction anyway.
inline float4 multiplyAdd(float4 a, float4 b, float4 c) { return float4(vmlaq_f32(c.v, a.v, b.v)); }

// Float logical
inline float4 andBits(float4 a, float4 b) {
  return float4(vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(a.v), vreinterpretq_u32_f32(b.v))));
}
inline float4 andNotBits(float4 a, float4 b) {
  return float4(vreinterpretq_f32_u32(vbicq_u32(vreinterpretq_u32_f32(b.v), vreinterpretq_u32_f32(a.v))));
}
inline float4 orBits(float4 a, float4 b) {
  return float4(vreinterpretq_f32_u32(vorrq_u32(vreinterpretq_u32_f32(a.v), vreinterpretq_u32_f32(b.v))));
}
inline float4 xorBits(float4 a, float4 b) {
  return float4(vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(a.v), vreinterpretq_u32_f32(b.v))));
}

// Float comparisons (return float4 masks)
inline float4 operator==(float4 a, float4 b) {
  return float4(vreinterpretq_f32_u32(vceqq_f32(a.v, b.v)));
}
inline float4 operator!=(float4 a, float4 b) {
  return float4(vreinterpretq_f32_u32(vmvnq_u32(vceqq_f32(a.v, b.v))));
}
inline float4 operator>(float4 a, float4 b) {
  return float4(vreinterpretq_f32_u32(vcgtq_f32(a.v, b.v)));
}
inline float4 operator>=(float4 a, float4 b) {
  return float4(vreinterpretq_f32_u32(vcgeq_f32(a.v, b.v)));
}
inline float4 operator<(float4 a, float4 b) {
  return float4(vreinterpretq_f32_u32(vcltq_f32(a.v, b.v)));
}
inline float4 operator<=(float4 a, float4 b) {
  return float4(vreinterpretq_f32_u32(vcleq_f32(a.v, b.v)));
}

// Float special
inline float4 setZero() { return float4(vdupq_n_f32(0.0f)); }
inline float4 set1Float(float a) { return float4(vdupq_n_f32(a)); }
inline float4 setrFloat(float a, float b, float c, float d) { return float4(a, b, c, d); }

// Float shuffle/move
// shuffle<i0, i1, i2, i3>(a, b): result = [a[i0], a[i1], b[i2], b[i3]]
template<int i0, int i1, int i2, int i3>
inline float4 shuffle(float4 a, float4 b) {
  return float4((float32x4_t)__builtin_shufflevector(a.v, b.v, i0, i1, i2 + 4, i3 + 4));
}
inline float4 unpackLo(float4 a, float4 b) { return float4(vzip1q_f32(a.v, b.v)); }
inline float4 unpackHi(float4 a, float4 b) { return float4(vzip2q_f32(a.v, b.v)); }
inline float4 moveLH(float4 a, float4 b) { return float4(vcombine_f32(vget_low_f32(a.v), vget_low_f32(b.v))); }
inline float4 moveHL(float4 a, float4 b) { return float4(vcombine_f32(vget_high_f32(b.v), vget_high_f32(a.v))); }

// Scalar operations (operate on lowest element, upper lanes from first operand)
inline float4 addScalar(float4 a, float4 b) {
  return float4(vsetq_lane_f32(vgetq_lane_f32(a.v, 0) + vgetq_lane_f32(b.v, 0), a.v, 0));
}
inline float4 maxScalar(float4 a, float4 b) {
  float a0 = vgetq_lane_f32(a.v, 0), b0 = vgetq_lane_f32(b.v, 0);
  return float4(vsetq_lane_f32(a0 > b0 ? a0 : b0, a.v, 0));
}
inline float4 minScalar(float4 a, float4 b) {
  float a0 = vgetq_lane_f32(a.v, 0), b0 = vgetq_lane_f32(b.v, 0);
  return float4(vsetq_lane_f32(a0 < b0 ? a0 : b0, a.v, 0));
}
inline float extractScalar(float4 a) { return vgetq_lane_f32(a.v, 0); }

// Integer arithmetic
inline int4 multiplyUnsigned(int4 a, int4 b) { return int4(vmulq_s32(a.v, b.v)); }

// Integer logical
inline int4 andBits(int4 a, int4 b) { return int4(vandq_s32(a.v, b.v)); }
inline int4 andNotBits(int4 a, int4 b) { return int4(vbicq_s32(b.v, a.v)); }
inline int4 orBits(int4 a, int4 b) { return int4(vorrq_s32(a.v, b.v)); }
inline int4 xorBits(int4 a, int4 b) { return int4(veorq_s32(a.v, b.v)); }

// Integer special
inline int4 setZeroInt() { return int4(vdupq_n_s32(0)); }
inline int4 set1Int(int32_t a) { return int4(vdupq_n_s32(a)); }
inline int4 setInt(int32_t a, int32_t b, int32_t c, int32_t d) {
  return int4(a, b, c, d);
}
inline int4 setrInt(int32_t a, int32_t b, int32_t c, int32_t d) {
  return int4(a, b, c, d);
}

// Integer shifts (byte shifts) - require compile-time constants
// shiftLeftBytes: shift entire 128-bit value left by N bytes (zero-fill low end)
template<int count>
inline int4 shiftLeftBytes(int4 a) {
  return int4(vreinterpretq_s32_u8(
    vextq_u8(vdupq_n_u8(0), vreinterpretq_u8_s32(a.v), 16 - count)
  ));
}

// shiftRightBytes: shift entire 128-bit value right by N bytes (zero-fill high end)
template<int count>
inline int4 shiftRightBytes(int4 a) {
  return int4(vreinterpretq_s32_u8(
    vextq_u8(vreinterpretq_u8_s32(a.v), vdupq_n_u8(0), count)
  ));
}

// Integer shifts (element shifts) - logical (unsigned) semantics
inline int4 shiftLeftElements(int4 a, int count) {
  return int4(vshlq_s32(a.v, vdupq_n_s32(count)));
}
inline int4 shiftRightElements(int4 a, int count) {
  // USHL with negative shift amount = logical right shift
  return int4(vreinterpretq_s32_u32(vshlq_u32(vreinterpretq_u32_s32(a.v), vdupq_n_s32(-count))));
}

// Integer comparisons
inline int4 compareEqualInt(int4 a, int4 b) {
  return int4(vreinterpretq_s32_u32(vceqq_s32(a.v, b.v)));
}

// Conversions
inline int4 floatToIntRound(float4 a) { return int4(vcvtnq_s32_f32(a.v)); }
inline int4 floatToIntTruncate(float4 a) { return int4(vcvtq_s32_f32(a.v)); }
inline float4 intToFloat(int4 a) { return float4(vcvtq_f32_s32(a.v)); }

// Native NEON unsigned int->float
inline float4 unsignedIntToFloat(int4 v) {
  return float4(vcvtq_f32_u32(vreinterpretq_u32_s32(v.v)));
}

// Casts (reinterpret bits)
inline int4 castFloatToInt(float4 a) { return int4(vreinterpretq_s32_f32(a.v)); }
inline float4 castIntToFloat(int4 a) { return float4(vreinterpretq_f32_s32(a.v)); }

// ----------------------------------------------------------------
// select

inline float4 select(float4 a, float4 b, float4 conditionMask) {
  return float4(vreinterpretq_f32_u32(vbslq_u32(
    vreinterpretq_u32_f32(conditionMask.v),
    vreinterpretq_u32_f32(a.v),
    vreinterpretq_u32_f32(b.v)
  )));
}

// ----------------------------------------------------------------
// Horizontal operations — native pairwise ops

inline float vecSumH(float4 v) {
  float32x2_t lo = vget_low_f32(v.v);
  float32x2_t hi = vget_high_f32(v.v);
  float32x2_t sum = vadd_f32(lo, hi);
  sum = vpadd_f32(sum, sum);
  return vget_lane_f32(sum, 0);
}

inline float vecMaxH(float4 v) {
  float32x2_t lo = vget_low_f32(v.v);
  float32x2_t hi = vget_high_f32(v.v);
  float32x2_t m = vmax_f32(lo, hi);
  m = vpmax_f32(m, m);
  return vget_lane_f32(m, 0);
}

inline float vecMinH(float4 v) {
  float32x2_t lo = vget_low_f32(v.v);
  float32x2_t hi = vget_high_f32(v.v);
  float32x2_t m = vmin_f32(lo, hi);
  m = vpmin_f32(m, m);
  return vget_lane_f32(m, 0);
}

// ----------------------------------------------------------------
// Load/store functions

inline float4 loadFloat4(const float* ptr) { return float4(vld1q_f32(ptr)); }
inline void storeFloat4(float* ptr, float4 v) { vst1q_f32(ptr, v.v); }
inline int4 loadInt4(const int32_t* ptr) { return int4(vld1q_s32(ptr)); }
inline void storeInt4(int32_t* ptr, int4 v) { vst1q_s32(ptr, v.v); }

// ----------------------------------------------------------------
// Lane access (slow - avoid!)

inline float getFloat4Lane(float4 v, size_t lane) {
  assert(lane < 4);
  switch(lane) {
    case 0: return vgetq_lane_f32(v.v, 0);
    case 1: return vgetq_lane_f32(v.v, 1);
    case 2: return vgetq_lane_f32(v.v, 2);
    default: return vgetq_lane_f32(v.v, 3);
  }
}

inline void setFloat4Lane(float4& v, size_t lane, float val) {
  assert(lane < 4);
  switch(lane) {
    case 0: v = float4(vsetq_lane_f32(val, v.v, 0)); break;
    case 1: v = float4(vsetq_lane_f32(val, v.v, 1)); break;
    case 2: v = float4(vsetq_lane_f32(val, v.v, 2)); break;
    default: v = float4(vsetq_lane_f32(val, v.v, 3)); break;
  }
}
