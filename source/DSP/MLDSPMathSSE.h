// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// MLDSPMathSSE.h
// SSE implementations of madronalib SIMD primitives

// NEW CODE!

#include "MLPlatform.h"

#ifndef ML_SSE_TO_NEON
// SSE 4.1 required
#include <smmintrin.h>
#endif

#include <float.h>
#include <assert.h>

#pragma once

// ----------------------------------------------------------------
// Type aliases - the compiler already knows these are 16-byte aligned

typedef __m128 float4;
typedef __m128i int4;

// ----------------------------------------------------------------
// float4 construction functions (replace constructors)

inline float4 makeFloat4(__m128 x) { return x; }
inline float4 makeFloat4(float a, float b, float c, float d) { return _mm_setr_ps(a, b, c, d); }
inline float4 makeFloat4(float x) { return _mm_set1_ps(x); }

// int4 construction functions
inline int4 makeInt4(__m128i x) { return x; }
inline int4 makeInt4(int32_t a, int32_t b, int32_t c, int32_t d) { return _mm_setr_epi32(a, b, c, d); }
inline int4 makeInt4(int32_t x) { return _mm_set1_epi32(x); }

// ----------------------------------------------------------------
// Load/store functions

inline float4 loadFloat4(const float* ptr) { return _mm_load_ps(ptr); }
inline void storeFloat4(float* ptr, float4 v) { _mm_store_ps(ptr, v); }

inline int4 loadInt4(const int32_t* ptr) { return _mm_load_si128((const __m128i*)ptr); }
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
  v = _mm_load_ps(tmp);
}

// ----------------------------------------------------------------
// Wrapper functions for SSE intrinsics operating on float4/int4

// Float arithmetic
inline float4 add4(float4 a, float4 b) { return _mm_add_ps(a, b); }
inline float4 sub4(float4 a, float4 b) { return _mm_sub_ps(a, b); }
inline float4 mul4(float4 a, float4 b) { return _mm_mul_ps(a, b); }
inline float4 div4(float4 a, float4 b) { return _mm_div_ps(a, b); }
inline float4 min4(float4 a, float4 b) { return _mm_min_ps(a, b); }
inline float4 max4(float4 a, float4 b) { return _mm_max_ps(a, b); }
inline float4 sqrt4(float4 a) { return _mm_sqrt_ps(a); }
inline float4 rsqrt4(float4 a) { return _mm_rsqrt_ps(a); }
inline float4 rcp4(float4 a) { return _mm_rcp_ps(a); }

// Float logical
inline float4 andBits4(float4 a, float4 b) { return _mm_and_ps(a, b); }
inline float4 andNotBits4(float4 a, float4 b) { return _mm_andnot_ps(a, b); }
inline float4 orBits4(float4 a, float4 b) { return _mm_or_ps(a, b); }
inline float4 xorBits4(float4 a, float4 b) { return _mm_xor_ps(a, b); }

// Float comparisons (return float4 masks)
inline float4 compareEqual4(float4 a, float4 b) { return _mm_cmpeq_ps(a, b); }
inline float4 compareNotEqual4(float4 a, float4 b) { return _mm_cmpneq_ps(a, b); }
inline float4 compareGreaterThan4(float4 a, float4 b) { return _mm_cmpgt_ps(a, b); }
inline float4 compareGreaterThanOrEqual4(float4 a, float4 b) { return _mm_cmpge_ps(a, b); }
inline float4 compareLessThan4(float4 a, float4 b) { return _mm_cmplt_ps(a, b); }
inline float4 compareLessThanOrEqual4(float4 a, float4 b) { return _mm_cmple_ps(a, b); }

// Float special
inline float4 setZero4() { return _mm_setzero_ps(); }
inline float4 set1Float4(float a) { return _mm_set1_ps(a); }
inline float4 setrFloat4(float a, float b, float c, float d) { return _mm_setr_ps(a, b, c, d); }

// Float shuffle/move
template<int i0, int i1, int i2, int i3>
inline float4 shuffle4(float4 a, float4 b) {
  return _mm_shuffle_ps(a, b, _MM_SHUFFLE(i3, i2, i1, i0));
}
inline float4 unpackLo4(float4 a, float4 b) { return _mm_unpacklo_ps(a, b); }
inline float4 unpackHi4(float4 a, float4 b) { return _mm_unpackhi_ps(a, b); }
inline float4 moveLH4(float4 a, float4 b) { return _mm_movelh_ps(a, b); }
inline float4 moveHL4(float4 a, float4 b) { return _mm_movehl_ps(a, b); }

// Scalar operations (operate on lowest element)
inline float4 addScalar4(float4 a, float4 b) { return _mm_add_ss(a, b); }
inline float4 maxScalar4(float4 a, float4 b) { return _mm_max_ss(a, b); }
inline float4 minScalar4(float4 a, float4 b) { return _mm_min_ss(a, b); }
inline float extractScalar4(float4 a) { return _mm_cvtss_f32(a); }

// Integer arithmetic
inline int4 add4(int4 a, int4 b) { return _mm_add_epi32(a, b); }
inline int4 sub4(int4 a, int4 b) { return _mm_sub_epi32(a, b); }
inline int4 multiplyUnsigned4(int4 a, int4 b) { return _mm_mullo_epi32(a, b); }

// Integer logical
inline int4 andBits4(int4 a, int4 b) { return _mm_and_si128(a, b); }
inline int4 andNotBits4(int4 a, int4 b) { return _mm_andnot_si128(a, b); }
inline int4 orBits4(int4 a, int4 b) { return _mm_or_si128(a, b); }
inline int4 xorBits4(int4 a, int4 b) { return _mm_xor_si128(a, b); }

// Integer special
inline int4 setZeroInt4() { return _mm_setzero_si128(); }
inline int4 set1Int4(int32_t a) { return _mm_set1_epi32(a); }
inline int4 setInt4(int32_t a, int32_t b, int32_t c, int32_t d) {
  return _mm_set_epi32(d, c, b, a);
}
inline int4 setrInt4(int32_t a, int32_t b, int32_t c, int32_t d) {
  return _mm_setr_epi32(a, b, c, d);
}

// Integer shifts (byte shifts) - require compile-time constants
template<int count>
inline int4 shiftLeftBytes4(int4 a) { return _mm_slli_si128(a, count); }

template<int count>
inline int4 shiftRightBytes4(int4 a) { return _mm_srli_si128(a, count); }

// Integer shifts (element shifts)
inline int4 shiftLeftElements4(int4 a, int count) { return _mm_slli_epi32(a, count); }
inline int4 shiftRightElements4(int4 a, int count) { return _mm_srli_epi32(a, count); }

// Integer comparisons
inline int4 compareEqualInt4(int4 a, int4 b) { return _mm_cmpeq_epi32(a, b); }

// Conversions
inline int4 floatToIntRound4(float4 a) { return _mm_cvtps_epi32(a); }
inline int4 floatToIntTruncate4(float4 a) { return _mm_cvttps_epi32(a); }
inline float4 intToFloat4(int4 a) { return _mm_cvtepi32_ps(a); }

// Casts (reinterpret bits)
inline int4 castFloatToInt4(float4 a) { return _mm_castps_si128(a); }
inline float4 castIntToFloat4(int4 a) { return _mm_castsi128_ps(a); }

// Other functions
inline float4 clamp4(float4 a, float4 b, float4 c) { return min4(max4(a, b), c); }
inline void transpose4x4(float4& r0, float4& r1, float4& r2, float4& r3) {
  float4 t0 = unpackLo4(r0, r1);
  float4 t1 = unpackLo4(r2, r3);
  float4 t2 = unpackHi4(r0, r1);
  float4 t3 = unpackHi4(r2, r3);
  r0 = moveLH4(t0, t1);
  r1 = moveHL4(t1, t0);
  r2 = moveLH4(t2, t3);
  r3 = moveHL4(t3, t2);
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

// SSE casts (keep old names for compatibility)
#define float4ToInt4 castFloatToInt4
#define int4ToFloat4 castIntToFloat4

// primitive SSE operations (for backward compatibility with old macros)
#define vecAdd _mm_add_ps
#define vecSub _mm_sub_ps
#define vecMul _mm_mul_ps
#define vecDiv _mm_div_ps
#define vecRecipApprox _mm_rcp_ps
#define vecDivApprox(x1, x2) (_mm_mul_ps(x1, _mm_rcp_ps(x2)))
#define vecMin _mm_min_ps
#define vecMax _mm_max_ps
#define vecSqrt _mm_sqrt_ps
#define vecRSqrt _mm_rsqrt_ps
#define vecSqrtApprox(x) (vecMul(x, vecRSqrt(x)))
#define vecAbs(x) (_mm_andnot_ps(_mm_set_ps1(-0.0f), x))

#define vecSign(x) \
(_mm_and_ps(_mm_or_ps(_mm_and_ps(_mm_set_ps1(-0.0f), x), _mm_set_ps1(1.0f)), \
_mm_cmpneq_ps(_mm_set_ps1(-0.0f), x)))

#define vecSignBit(x) (_mm_or_ps(_mm_and_ps(_mm_set_ps1(-0.0f), x), _mm_set_ps1(1.0f)))
#define vecClamp(x1, x2, x3) _mm_min_ps(_mm_max_ps(x1, x2), x3)
#define vecWithin(x1, x2, x3) _mm_and_ps(_mm_cmpge_ps(x1, x2), _mm_cmplt_ps(x1, x3))

#define vecEqual _mm_cmpeq_ps
#define vecNotEqual _mm_cmpneq_ps
#define vecGreaterThan _mm_cmpgt_ps
#define vecGreaterThanOrEqual _mm_cmpge_ps
#define vecLessThan _mm_cmplt_ps
#define vecLessThanOrEqual _mm_cmple_ps

#define vecSet1 _mm_set1_ps

#define vecStore _mm_store_ps
#define vecLoad _mm_load_ps

#define vecStoreUnaligned _mm_storeu_ps
#define vecLoadUnaligned _mm_loadu_ps

#define vecAnd _mm_and_ps
#define vecOr _mm_or_ps

#define vecZeros _mm_setzero_ps
#define vecOnes vecEqual(vecZeros, vecZeros)

#define vecShiftLeft _mm_slli_si128
#define vecShiftRight _mm_srli_si128

#define vecFloatToIntRound _mm_cvtps_epi32
#define vecFloatToIntTruncate _mm_cvttps_epi32
#define vecIntToFloat _mm_cvtepi32_ps

// _mm_cvtepi32_ps approximation for unsigned int data
inline float4 vecUnsignedIntToFloat(int4 v) {
  int4 v_hi = shiftRightElements4(v, 1);
  float4 v_hi_flt = intToFloat4(v_hi);
  return add4(v_hi_flt, v_hi_flt);
}

#define vecAddInt _mm_add_epi32
#define vecSubInt _mm_sub_epi32
#define vecSet1Int _mm_set1_epi32

inline int4 vecSetInt1(uint32_t a) { return set1Int4(a); }
inline int4 vecSetInt4(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
  return setrInt4(a, b, c, d);
}

#define SHUFFLE(a, b, c, d) ((a << 6) | (b << 4) | (c << 2) | (d))
#define vecBroadcast3(x1) _mm_shuffle_ps(x1, x1, SHUFFLE(3, 3, 3, 3))

#define vecShiftElementsLeft(x1, i) _mm_slli_si128(x1, 4 * i);
#define vecShiftElementsRight(x1, i) _mm_srli_si128(x1, 4 * i);

// ----------------------------------------------------------------
// select functions

inline float4 vecSelectFFI(float4 a, float4 b, int4 conditionMask) {
  int4 ones = set1Int4(-1);
  return orBits4(andBits4(castIntToFloat4(conditionMask), a),
                 andBits4(xorBits4(castIntToFloat4(conditionMask), castIntToFloat4(ones)), b));
}

inline float4 vecSelectFFF(float4 a, float4 b, float4 conditionMask) {
  int4 ones = set1Int4(-1);
  return orBits4(andBits4(conditionMask, a),
                 andBits4(xorBits4(conditionMask, castIntToFloat4(ones)), b));
}

inline int4 vecSelectIII(int4 a, int4 b, int4 conditionMask) {
  int4 ones = set1Int4(-1);
  return orBits4(andBits4(conditionMask, a),
                 andBits4(xorBits4(conditionMask, ones), b));
}

// ----------------------------------------------------------------
// horizontal operations returning float

inline float vecSumH(float4 v) {
  float4 tmp0 = add4(v, moveHL4(v, v));
  float4 tmp1 = addScalar4(tmp0, shuffle4<1,1,1,1>(tmp0, tmp0));
  return extractScalar4(tmp1);
}

inline float vecMaxH(float4 v) {
  float4 tmp0 = max4(v, moveHL4(v, v));
  float4 tmp1 = maxScalar4(tmp0, shuffle4<1,1,1,1>(tmp0, tmp0));
  return extractScalar4(tmp1);
}

inline float vecMinH(float4 v) {
  float4 tmp0 = min4(v, moveHL4(v, v));
  float4 tmp1 = minScalar4(tmp0, shuffle4<1,1,1,1>(tmp0, tmp0));
  return extractScalar4(tmp1);
}

// cephes-derived approximate math functions adapted from code by Julien Pommier
// Copyright (C) 2007 Julien Pommier and licensed under the zlib license
// Float constants
static const float4 _ps_1 = makeFloat4(1.0f);
static const float4 _ps_0p5 = makeFloat4(0.5f);
static const float4 _ps_cephes_SQRTHF = makeFloat4(0.707106781186547524f);
static const float4 _ps_cephes_log_p0 = makeFloat4(7.0376836292E-2f);
static const float4 _ps_cephes_log_p1 = makeFloat4(-1.1514610310E-1f);
static const float4 _ps_cephes_log_p2 = makeFloat4(1.1676998740E-1f);
static const float4 _ps_cephes_log_p3 = makeFloat4(-1.2420140846E-1f);
static const float4 _ps_cephes_log_p4 = makeFloat4(+1.4249322787E-1f);
static const float4 _ps_cephes_log_p5 = makeFloat4(-1.6668057665E-1f);
static const float4 _ps_cephes_log_p6 = makeFloat4(+2.0000714765E-1f);
static const float4 _ps_cephes_log_p7 = makeFloat4(-2.4999993993E-1f);
static const float4 _ps_cephes_log_p8 = makeFloat4(+3.3333331174E-1f);
static const float4 _ps_cephes_log_q1 = makeFloat4(-2.12194440e-4f);
static const float4 _ps_cephes_log_q2 = makeFloat4(0.693359375f);

static const float4 _ps_exp_hi = makeFloat4(88.3762626647949f);
static const float4 _ps_exp_lo = makeFloat4(-88.3762626647949f);

static const float4 _ps_cephes_LOG2EF = makeFloat4(1.44269504088896341f);
static const float4 _ps_cephes_exp_C1 = makeFloat4(0.693359375f);
static const float4 _ps_cephes_exp_C2 = makeFloat4(-2.12194440e-4f);

static const float4 _ps_cephes_exp_p0 = makeFloat4(1.9875691500E-4f);
static const float4 _ps_cephes_exp_p1 = makeFloat4(1.3981999507E-3f);
static const float4 _ps_cephes_exp_p2 = makeFloat4(8.3334519073E-3f);
static const float4 _ps_cephes_exp_p3 = makeFloat4(4.1665795894E-2f);
static const float4 _ps_cephes_exp_p4 = makeFloat4(1.6666665459E-1f);
static const float4 _ps_cephes_exp_p5 = makeFloat4(5.0000001201E-1f);

static const float4 _ps_minus_cephes_DP1 = makeFloat4(-0.78515625f);
static const float4 _ps_minus_cephes_DP2 = makeFloat4(-2.4187564849853515625e-4f);
static const float4 _ps_minus_cephes_DP3 = makeFloat4(-3.77489497744594108e-8f);
static const float4 _ps_sincof_p0 = makeFloat4(-1.9515295891E-4f);
static const float4 _ps_sincof_p1 = makeFloat4(8.3321608736E-3f);
static const float4 _ps_sincof_p2 = makeFloat4(-1.6666654611E-1f);
static const float4 _ps_coscof_p0 = makeFloat4(2.443315711809948E-005f);
static const float4 _ps_coscof_p1 = makeFloat4(-1.388731625493765E-003f);
static const float4 _ps_coscof_p2 = makeFloat4(4.166664568298827E-002f);
static const float4 _ps_cephes_FOPI = makeFloat4(1.27323954473516f);  // 4 / M_PI

// Integer constants
static const int4 _pi32_min_norm_pos = makeInt4(0x00800000);  // the smallest non denormalized float number
static const int4 _pi32_mant_mask = makeInt4(0x7f800000);
static const int4 _pi32_inv_mant_mask = makeInt4(~0x7f800000);
static const int4 _pi32_sign_mask = makeInt4((int32_t)0x80000000);
static const int4 _pi32_inv_sign_mask = makeInt4(~0x80000000);
static const int4 _pi32_1 = makeInt4(1);
static const int4 _pi32_inv1 = makeInt4(~1);
static const int4 _pi32_2 = makeInt4(2);
static const int4 _pi32_4 = makeInt4(4);
static const int4 _pi32_0x7f = makeInt4(0x7f);

// polynomial approximation constants

static const float4 kSinC1Vec = makeFloat4(0.99997937679290771484375f);
static const float4 kSinC2Vec = makeFloat4(-0.166624367237091064453125f);
static const float4 kSinC3Vec = makeFloat4(8.30897875130176544189453125e-3f);
static const float4 kSinC4Vec = makeFloat4(-1.92649182281456887722015380859375e-4f);
static const float4 kSinC5Vec = makeFloat4(2.147840177713078446686267852783203125e-6f);

static const float4 kCosC1Vec = makeFloat4(0.999959766864776611328125f);
static const float4 kCosC2Vec = makeFloat4(-0.4997930824756622314453125f);
static const float4 kCosC3Vec = makeFloat4(4.1496001183986663818359375e-2f);
static const float4 kCosC4Vec = makeFloat4(-1.33926304988563060760498046875e-3f);
static const float4 kCosC5Vec = makeFloat4(1.8791708498611114919185638427734375e-5f);

static const float4 kExpC1Vec = makeFloat4(2139095040.f);
static const float4 kExpC2Vec = makeFloat4(12102203.1615614f);
static const float4 kExpC3Vec = makeFloat4(1065353216.f);
static const float4 kExpC4Vec = makeFloat4(0.510397365625862338668154f);
static const float4 kExpC5Vec = makeFloat4(0.310670891004095530771135f);
static const float4 kExpC6Vec = makeFloat4(0.168143436463395944830000f);
static const float4 kExpC7Vec = makeFloat4(-2.88093587581985443087955e-3f);
static const float4 kExpC8Vec = makeFloat4(1.3671023382430374383648148e-2f);

static const float4 kLogC1Vec = makeFloat4(-89.970756366f);
static const float4 kLogC2Vec = makeFloat4(3.529304993f);
static const float4 kLogC3Vec = makeFloat4(-2.461222105f);
static const float4 kLogC4Vec = makeFloat4(1.130626167f);
static const float4 kLogC5Vec = makeFloat4(-0.288739945f);
static const float4 kLogC6Vec = makeFloat4(3.110401639e-2f);
static const float4 kLogC7Vec = makeFloat4(0.69314718055995f);

static const float4 k9Vec = makeFloat4(9.0f);
static const float4 k27Vec = makeFloat4(27.0f);

// ----------------------------------------------------------------
// vecLog - natural logarithm

inline float4 vecLog(float4 x) {
  int4 emm0;
  float4 one = _ps_1;
  float4 invalid_mask = compareLessThanOrEqual4(x, setZero4());
  
  x = max4(x, castIntToFloat4(_pi32_min_norm_pos));
  
  emm0 = shiftRightElements4(castFloatToInt4(x), 23);
  
  x = andBits4(x, castIntToFloat4(_pi32_inv_mant_mask));
  x = orBits4(x, _ps_0p5);
  
  emm0 = sub4(emm0, _pi32_0x7f);
  float4 e = intToFloat4(emm0);
  
  e = add4(e, one);
  
  float4 mask = compareLessThan4(x, _ps_cephes_SQRTHF);
  float4 tmp = andBits4(x, mask);
  x = sub4(x, one);
  e = sub4(e, andBits4(one, mask));
  x = add4(x, tmp);
  
  float4 z = mul4(x, x);
  
  float4 y = _ps_cephes_log_p0;
  y = mul4(y, x);
  y = add4(y, _ps_cephes_log_p1);
  y = mul4(y, x);
  y = add4(y, _ps_cephes_log_p2);
  y = mul4(y, x);
  y = add4(y, _ps_cephes_log_p3);
  y = mul4(y, x);
  y = add4(y, _ps_cephes_log_p4);
  y = mul4(y, x);
  y = add4(y, _ps_cephes_log_p5);
  y = mul4(y, x);
  y = add4(y, _ps_cephes_log_p6);
  y = mul4(y, x);
  y = add4(y, _ps_cephes_log_p7);
  y = mul4(y, x);
  y = add4(y, _ps_cephes_log_p8);
  y = mul4(y, x);
  
  y = mul4(y, z);
  
  tmp = mul4(e, _ps_cephes_log_q1);
  y = add4(y, tmp);
  
  tmp = mul4(z, _ps_0p5);
  y = sub4(y, tmp);
  
  tmp = mul4(e, _ps_cephes_log_q2);
  x = add4(x, y);
  x = add4(x, tmp);
  x = orBits4(x, invalid_mask);
  return x;
}

// ----------------------------------------------------------------
// vecExp - exponential

inline float4 vecExp(float4 x) {
  float4 tmp = setZero4(), fx;
  int4 emm0;
  float4 one = _ps_1;
  
  x = min4(x, _ps_exp_hi);
  x = max4(x, _ps_exp_lo);
  
  fx = mul4(x, _ps_cephes_LOG2EF);
  fx = add4(fx, _ps_0p5);
  
  emm0 = floatToIntTruncate4(fx);
  tmp = intToFloat4(emm0);
  
  float4 mask = compareGreaterThan4(tmp, fx);
  mask = andBits4(mask, one);
  fx = sub4(tmp, mask);
  
  tmp = mul4(fx, _ps_cephes_exp_C1);
  float4 z = mul4(fx, _ps_cephes_exp_C2);
  x = sub4(x, tmp);
  x = sub4(x, z);
  z = mul4(x, x);
  
  float4 y = _ps_cephes_exp_p0;
  y = mul4(y, x);
  y = add4(y, _ps_cephes_exp_p1);
  y = mul4(y, x);
  y = add4(y, _ps_cephes_exp_p2);
  y = mul4(y, x);
  y = add4(y, _ps_cephes_exp_p3);
  y = mul4(y, x);
  y = add4(y, _ps_cephes_exp_p4);
  y = mul4(y, x);
  y = add4(y, _ps_cephes_exp_p5);
  y = mul4(y, z);
  y = add4(y, x);
  y = add4(y, one);
  
  emm0 = floatToIntTruncate4(fx);
  emm0 = add4(emm0, _pi32_0x7f);
  emm0 = shiftLeftElements4(emm0, 23);
  float4 pow2n = castIntToFloat4(emm0);
  
  y = mul4(y, pow2n);
  return y;
}

// ----------------------------------------------------------------
// vecSin - sine

inline float4 vecSin(float4 x) {
  float4 xmm1, xmm2 = setZero4(), xmm3, sign_bit, y;
  int4 emm0, emm2;
  
  sign_bit = x;
  x = andBits4(x, castIntToFloat4(_pi32_inv_sign_mask));
  sign_bit = andBits4(sign_bit, castIntToFloat4(_pi32_sign_mask));
  
  y = mul4(x, _ps_cephes_FOPI);
  
  emm2 = floatToIntTruncate4(y);
  emm2 = add4(emm2, _pi32_1);
  emm2 = andBits4(emm2, _pi32_inv1);
  y = intToFloat4(emm2);
  
  emm0 = andBits4(emm2, _pi32_4);
  emm0 = shiftLeftElements4(emm0, 29);
  
  emm2 = andBits4(emm2, _pi32_2);
  emm2 = compareEqualInt4(emm2, setZeroInt4());
  
  float4 swap_sign_bit = castIntToFloat4(emm0);
  float4 poly_mask = castIntToFloat4(emm2);
  sign_bit = xorBits4(sign_bit, swap_sign_bit);
  
  xmm1 = _ps_minus_cephes_DP1;
  xmm2 = _ps_minus_cephes_DP2;
  xmm3 = _ps_minus_cephes_DP3;
  xmm1 = mul4(y, xmm1);
  xmm2 = mul4(y, xmm2);
  xmm3 = mul4(y, xmm3);
  x = add4(x, xmm1);
  x = add4(x, xmm2);
  x = add4(x, xmm3);
  
  y = _ps_coscof_p0;
  float4 z = mul4(x, x);
  
  y = mul4(y, z);
  y = add4(y, _ps_coscof_p1);
  y = mul4(y, z);
  y = add4(y, _ps_coscof_p2);
  y = mul4(y, z);
  y = mul4(y, z);
  float4 tmp = mul4(z, _ps_0p5);
  y = sub4(y, tmp);
  y = add4(y, _ps_1);
  
  float4 y2 = _ps_sincof_p0;
  y2 = mul4(y2, z);
  y2 = add4(y2, _ps_sincof_p1);
  y2 = mul4(y2, z);
  y2 = add4(y2, _ps_sincof_p2);
  y2 = mul4(y2, z);
  y2 = mul4(y2, x);
  y2 = add4(y2, x);
  
  xmm3 = poly_mask;
  y2 = andBits4(xmm3, y2);
  y = andNotBits4(xmm3, y);
  y = add4(y, y2);
  y = xorBits4(y, sign_bit);
  return y;
}

// ----------------------------------------------------------------
// vecCos - cosine

inline float4 vecCos(float4 x) {
  float4 xmm1, xmm2 = setZero4(), xmm3, y;
  int4 emm0, emm2;
  
  x = andBits4(x, castIntToFloat4(_pi32_inv_sign_mask));
  
  y = mul4(x, _ps_cephes_FOPI);
  
  emm2 = floatToIntTruncate4(y);
  emm2 = add4(emm2, _pi32_1);
  emm2 = andBits4(emm2, _pi32_inv1);
  y = intToFloat4(emm2);
  emm2 = sub4(emm2, _pi32_2);
  
  emm0 = andNotBits4(emm2, _pi32_4);
  emm0 = shiftLeftElements4(emm0, 29);
  
  emm2 = andBits4(emm2, _pi32_2);
  emm2 = compareEqualInt4(emm2, setZeroInt4());
  
  float4 sign_bit = castIntToFloat4(emm0);
  float4 poly_mask = castIntToFloat4(emm2);
  
  xmm1 = _ps_minus_cephes_DP1;
  xmm2 = _ps_minus_cephes_DP2;
  xmm3 = _ps_minus_cephes_DP3;
  xmm1 = mul4(y, xmm1);
  xmm2 = mul4(y, xmm2);
  xmm3 = mul4(y, xmm3);
  x = add4(x, xmm1);
  x = add4(x, xmm2);
  x = add4(x, xmm3);
  
  y = _ps_coscof_p0;
  float4 z = mul4(x, x);
  
  y = mul4(y, z);
  y = add4(y, _ps_coscof_p1);
  y = mul4(y, z);
  y = add4(y, _ps_coscof_p2);
  y = mul4(y, z);
  y = mul4(y, z);
  float4 tmp = mul4(z, _ps_0p5);
  y = sub4(y, tmp);
  y = add4(y, _ps_1);
  
  float4 y2 = _ps_sincof_p0;
  y2 = mul4(y2, z);
  y2 = add4(y2, _ps_sincof_p1);
  y2 = mul4(y2, z);
  y2 = add4(y2, _ps_sincof_p2);
  y2 = mul4(y2, z);
  y2 = mul4(y2, x);
  y2 = add4(y2, x);
  
  xmm3 = poly_mask;
  y2 = andBits4(xmm3, y2);
  y = andNotBits4(xmm3, y);
  y = add4(y, y2);
  y = xorBits4(y, sign_bit);
  
  return y;
}

// ----------------------------------------------------------------
// vecSinCos - simultaneous sine and cosine

inline void vecSinCos(float4 x, float4* s, float4* c) {
  float4 xmm1, xmm2, xmm3 = setZero4(), sign_bit_sin, y;
  int4 emm0, emm2, emm4;
  
  sign_bit_sin = x;
  x = andBits4(x, castIntToFloat4(_pi32_inv_sign_mask));
  sign_bit_sin = andBits4(sign_bit_sin, castIntToFloat4(_pi32_sign_mask));
  
  y = mul4(x, _ps_cephes_FOPI);
  
  emm2 = floatToIntTruncate4(y);
  
  emm2 = add4(emm2, _pi32_1);
  emm2 = andBits4(emm2, _pi32_inv1);
  y = intToFloat4(emm2);
  
  emm4 = emm2;
  
  emm0 = andBits4(emm2, _pi32_4);
  emm0 = shiftLeftElements4(emm0, 29);
  float4 swap_sign_bit_sin = castIntToFloat4(emm0);
  
  emm2 = andBits4(emm2, _pi32_2);
  emm2 = compareEqualInt4(emm2, setZeroInt4());
  float4 poly_mask = castIntToFloat4(emm2);
  
  xmm1 = _ps_minus_cephes_DP1;
  xmm2 = _ps_minus_cephes_DP2;
  xmm3 = _ps_minus_cephes_DP3;
  xmm1 = mul4(y, xmm1);
  xmm2 = mul4(y, xmm2);
  xmm3 = mul4(y, xmm3);
  x = add4(x, xmm1);
  x = add4(x, xmm2);
  x = add4(x, xmm3);
  
  emm4 = sub4(emm4, _pi32_2);
  emm4 = andNotBits4(emm4, _pi32_4);
  emm4 = shiftLeftElements4(emm4, 29);
  float4 sign_bit_cos = castIntToFloat4(emm4);
  
  sign_bit_sin = xorBits4(sign_bit_sin, swap_sign_bit_sin);
  
  float4 z = mul4(x, x);
  y = _ps_coscof_p0;
  
  y = mul4(y, z);
  y = add4(y, _ps_coscof_p1);
  y = mul4(y, z);
  y = add4(y, _ps_coscof_p2);
  y = mul4(y, z);
  y = mul4(y, z);
  float4 tmp = mul4(z, _ps_0p5);
  y = sub4(y, tmp);
  y = add4(y, _ps_1);
  
  float4 y2 = _ps_sincof_p0;
  y2 = mul4(y2, z);
  y2 = add4(y2, _ps_sincof_p1);
  y2 = mul4(y2, z);
  y2 = add4(y2, _ps_sincof_p2);
  y2 = mul4(y2, z);
  y2 = mul4(y2, x);
  y2 = add4(y2, x);
  
  xmm3 = poly_mask;
  float4 ysin2 = andBits4(xmm3, y2);
  float4 ysin1 = andNotBits4(xmm3, y);
  y2 = sub4(y2, ysin2);
  y = sub4(y, ysin1);
  
  xmm1 = add4(ysin1, ysin2);
  xmm2 = add4(y, y2);
  
  *s = xorBits4(xmm1, sign_bit_sin);
  *c = xorBits4(xmm2, sign_bit_cos);
}

// ----------------------------------------------------------------
// Fast polynomial approximations

inline float4 vecSinApprox(float4 x) {
  float4 x2 = mul4(x, x);
  return mul4(
              x, add4(kSinC1Vec,
                      mul4(x2,
                           add4(kSinC2Vec,
                                mul4(x2, add4(kSinC3Vec,
                                              mul4(x2, add4(kSinC4Vec,
                                                            mul4(x2, kSinC5Vec)))))))));
}

inline float4 vecCosApprox(float4 x) {
  float4 x2 = mul4(x, x);
  return add4(kCosC1Vec,
              mul4(x2,
                   add4(kCosC2Vec,
                        mul4(x2, add4(kCosC3Vec,
                                      mul4(x2, add4(kCosC4Vec,
                                                    mul4(x2, kCosC5Vec))))))));
}

inline float4 vecExpApprox(float4 x) {
  const float4 kZeroVec = setZero4();
  
  float4 val2, val3, val4;
  int4 val4i;
  
  val2 = add4(mul4(x, kExpC2Vec), kExpC3Vec);
  val3 = min4(val2, kExpC1Vec);
  val4 = max4(val3, kZeroVec);
  val4i = floatToIntTruncate4(val4);
  
  float4 xu = andBits4(castIntToFloat4(val4i), castIntToFloat4(set1Int4(0x7F800000)));
  float4 b = orBits4(andBits4(castIntToFloat4(val4i), castIntToFloat4(set1Int4(0x7FFFFF))),
                     castIntToFloat4(set1Int4(0x3F800000)));
  
  return mul4(
              xu,
              (add4(kExpC4Vec,
                    mul4(b, add4(kExpC5Vec,
                                 mul4(b, add4(kExpC6Vec,
                                              mul4(b, add4(kExpC7Vec,
                                                           mul4(b, kExpC8Vec))))))))));
}

inline float4 vecLogApprox(float4 val) {
  int4 vZero = setZeroInt4();
  int4 valAsInt = castFloatToInt4(val);
  int4 expi = shiftRightElements4(valAsInt, 23);
  float4 addcst = vecSelectFFI(kLogC1Vec, set1Float4(FLT_MIN),
                               castFloatToInt4(compareGreaterThan4(val, castIntToFloat4(vZero))));
  int4 valAsIntMasked =
  castFloatToInt4(orBits4(andBits4(castIntToFloat4(valAsInt), castIntToFloat4(set1Int4(0x7FFFFF))),
                          castIntToFloat4(set1Int4(0x3F800000))));
  float4 x = castIntToFloat4(valAsIntMasked);
  
  float4 poly = mul4(
                     x, add4(kLogC2Vec,
                             mul4(x, add4(kLogC3Vec,
                                          mul4(x, add4(kLogC4Vec,
                                                       mul4(x, add4(kLogC5Vec,
                                                                    mul4(x, kLogC6Vec)))))))));
  
  float4 addCstResult = add4(addcst, mul4(kLogC7Vec, intToFloat4(expi)));
  return add4(poly, addCstResult);
}

inline float4 vecTanhApprox(float4 x) {
  float4 x2 = mul4(x, x);
  float4 denom = add4(k27Vec, mul4(k9Vec, x2));
  float4 frac = div4(add4(k27Vec, x2), (denom));
  return mul4(x, frac);
}

// ----------------------------------------------------------------
// Conversions

inline float4 vecIntPart(float4 val) {
  int4 vi = floatToIntTruncate4(val);
  return (intToFloat4(vi));
}

inline float4 vecFracPart(float4 val) {
  int4 vi = floatToIntTruncate4(val);
  float4 intPart = intToFloat4(vi);
  return sub4(val, intPart);
}

// ----------------------------------------------------------------
// Shuffles

inline float4 vecShuffleRight(float4 v1, float4 v2) {
  return shuffle4<3, 0, 1, 2>(shuffle4<0, 0, 3, 3>(v2, v1), v2);
}

inline float4 vecShuffleLeft(float4 v1, float4 v2) {
  return shuffle4<1, 2, 0, 3>(v1, shuffle4<3, 3, 0, 0>(v1, v2));
}
