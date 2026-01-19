// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2022 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// MLDSPMathSSE.h
// SSE implementations of madronalib SIMD primitives

#include "MLPlatform.h"

#ifndef ML_SSE_TO_NEON
#include <emmintrin.h>
#endif

#include <float.h>
#include <assert.h>

#pragma once

struct alignas(16) float4
{
  union {
    __m128 v;
    float f[4];
  };
  
    float4() {}
  constexpr float4(float x) : f{x, x, x, x} {}
  constexpr float4(float a, float b, float c, float d) : f{a, b, c, d} {}
  float4(__m128 x) : v(x) {}  // Not constexpr - runtime only

  // Implicit conversions to __m128
  operator __m128&() { return v; }
  operator const __m128&() const { return v; }
  
  // Compound assignment operators
  float4& operator+=(const float4& rhs) { v = _mm_add_ps(v, rhs); return *this; }
  float4& operator-=(const float4& rhs) { v = _mm_sub_ps(v, rhs); return *this; }
  float4& operator*=(const float4& rhs) { v = _mm_mul_ps(v, rhs); return *this; }
  float4& operator/=(const float4& rhs) { v = _mm_div_ps(v, rhs); return *this; }
  
  static float4 load(const float* ptr) { return _mm_load_ps(ptr); }
  void store(float* ptr) const { _mm_store_ps(ptr, v); }
};

struct alignas(16) int4
{
    union {
      __m128i v;
      uint32_t i[4];
    };

    int4() {}
    constexpr int4(int32_t a, int32_t b, int32_t c, int32_t d) : i{(uint32_t)a, (uint32_t)b, (uint32_t)c, (uint32_t)d} {}
    constexpr int4(int32_t x) : i{(uint32_t)x, (uint32_t)x, (uint32_t)x, (uint32_t)x} {}
    int4(__m128i x) : v(x) {}  // Not constexpr - runtime only

  // Implicit conversions to __m128i
  operator __m128i&() { return v; }
  operator const __m128i&() const { return v; }
  
  // Compound assignment operators
  int4& operator+=(const int4& rhs) { v = _mm_add_epi32(v, rhs); return *this; }
  int4& operator-=(const int4& rhs) { v = _mm_sub_epi32(v, rhs); return *this; }
  // Note: integer multiplication requires SSE4.1 (_mm_mullo_epi32)
  // Note: integer division has no SIMD instruction in SSE2/SSE4
  
  static int4 load(const int32_t* ptr) { return _mm_load_si128((__m128i*)ptr); }
  void store(int32_t* ptr) const { _mm_store_si128((__m128i*)ptr, v); }
};

// Free function operators
// Note: float * float4 works via implicit conversion
inline float4 operator+(const float4& lhs, const float4& rhs) {
  return _mm_add_ps(lhs, rhs);
}
inline float4 operator-(const float4& lhs, const float4& rhs) {
  return _mm_sub_ps(lhs, rhs);
}
inline float4 operator*(const float4& lhs, const float4& rhs) {
  return _mm_mul_ps(lhs, rhs);
}
inline float4 operator/(const float4& lhs, const float4& rhs) {
  return _mm_div_ps(lhs, rhs);
}
inline float4 operator-(const float4& x) {
  return _mm_sub_ps(_mm_setzero_ps(), x);
}
inline float4 clamp(const float4& a, const float4& b, const float4& c) {
  return _mm_min_ps(_mm_max_ps(a, b), c);
}
inline float4 rcp(const float4& a) {
  return _mm_rcp_ps(a);
}

inline void transpose4x4(float4& r0, float4& r1, float4& r2, float4& r3) {
  __m128 t0 = _mm_unpacklo_ps(r0, r1);
  __m128 t1 = _mm_unpacklo_ps(r2, r3);
  __m128 t2 = _mm_unpackhi_ps(r0, r1);
  __m128 t3 = _mm_unpackhi_ps(r2, r3);
  r0 = _mm_movelh_ps(t0, t1);
  r1 = _mm_movehl_ps(t1, t0);
  r2 = _mm_movelh_ps(t2, t3);
  r3 = _mm_movehl_ps(t3, t2);
}

inline int4 operator|(const int4& a, const int4& b)
{
  return _mm_or_si128(a, b);
}
inline int4 operator&(const int4& a, const int4& b)
{
  return _mm_and_si128(a, b);
}

inline std::ostream& operator<< (std::ostream& out, const float4 x)
{
    float4 u{x};
    out << "[";
    out << u.f[0];
    out << ", ";
    out << u.f[1];
    out << ", ";
    out << u.f[2];
    out << ", ";
    out << u.f[3];
    out << "]";
    return out;
}

// SSE casts
#define float4ToInt4 _mm_castps_si128
#define int4ToFloat4 _mm_castsi128_ps

constexpr size_t kSIMDVectorsPerDSPVector = kFloatsPerDSPVector / 4;
constexpr size_t kBytesPerSIMDVector = 4 * sizeof(float);
constexpr size_t kSIMDVectorMask = ~(kBytesPerSIMDVector - 1);

inline bool isSIMDAligned(float* p)
{
  uintptr_t pM = (uintptr_t)p;
  return ((pM & kSIMDVectorMask) == 0);
}

// primitive SSE operations
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

#define vecSign(x)                                                             \
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
// this loses a bit of precision
inline float4 vecUnsignedIntToFloat(int4 v)
{
  __m128i v_hi = _mm_srli_epi32(v, 1);
  __m128 v_hi_flt = _mm_cvtepi32_ps(v_hi);
  return _mm_add_ps(v_hi_flt, v_hi_flt);
}

#define vecAddInt _mm_add_epi32
#define vecSubInt _mm_sub_epi32
#define vecSet1Int _mm_set1_epi32

inline int4 vecSetInt1(uint32_t a) { return _mm_set1_epi32(a); }

inline int4 vecSetInt4(uint32_t a, uint32_t b, uint32_t c, uint32_t d)
{
  return _mm_set_epi32(d, c, b, a);
}

#define SHUFFLE(a, b, c, d) ((a << 6) | (b << 4) | (c << 2) | (d))
#define vecBroadcast3(x1) _mm_shuffle_ps(x1, x1, SHUFFLE(3, 3, 3, 3))

#define vecShiftElementsLeft(x1, i) _mm_slli_si128(x1, 4 * i);
#define vecShiftElementsRight(x1, i) _mm_srli_si128(x1, 4 * i);

// ----------------------------------------------------------------
#pragma mark select

inline float4 vecSelectFFI(float4 a, float4 b, int4 conditionMask)
{
  __m128i ones = _mm_set1_epi32(-1);
  return _mm_or_ps(_mm_and_ps(int4ToFloat4(conditionMask), a),
                   _mm_and_ps(_mm_xor_ps(int4ToFloat4(conditionMask), int4ToFloat4(ones)), b));
}

inline float4 vecSelectFFF(float4 a, float4 b,
                                 float4 conditionMask)
{
  __m128i ones = _mm_set1_epi32(-1);
  return _mm_or_ps(_mm_and_ps((conditionMask), a),
                   _mm_and_ps(_mm_xor_ps((conditionMask), int4ToFloat4(ones)), b));
}

inline int4 vecSelectIII(int4 a, int4 b, int4 conditionMask)
{
  __m128i ones = _mm_set1_epi32(-1);
  return _mm_or_si128(_mm_and_si128(conditionMask, a),
                      _mm_and_si128(_mm_xor_si128(conditionMask, ones), b));
}

// ----------------------------------------------------------------
// horizontal operations returning float

inline float vecSumH(float4 v)
{
  float4 tmp0 = _mm_add_ps(v, _mm_movehl_ps(v, v));
  float4 tmp1 = _mm_add_ss(tmp0, _mm_shuffle_ps(tmp0, tmp0, 1));
  return _mm_cvtss_f32(tmp1);
}

inline float vecMaxH(float4 v)
{
  float4 tmp0 = _mm_max_ps(v, _mm_movehl_ps(v, v));
  float4 tmp1 = _mm_max_ss(tmp0, _mm_shuffle_ps(tmp0, tmp0, 1));
  return _mm_cvtss_f32(tmp1);
}

inline float vecMinH(float4 v)
{
  float4 tmp0 = _mm_min_ps(v, _mm_movehl_ps(v, v));
  float4 tmp1 = _mm_min_ss(tmp0, _mm_shuffle_ps(tmp0, tmp0, 1));
  return _mm_cvtss_f32(tmp1);
}
// cephes-derived approximate math functions adapted from code by Julien Pommier
// Copyright (C) 2007 Julien Pommier and licensed under the zlib license

// Float constants
constexpr float4 _ps_1{1.0f};
constexpr float4 _ps_0p5{0.5f};

constexpr float4 _ps_cephes_SQRTHF{0.707106781186547524f};
constexpr float4 _ps_cephes_log_p0{7.0376836292E-2f};
constexpr float4 _ps_cephes_log_p1{-1.1514610310E-1f};
constexpr float4 _ps_cephes_log_p2{1.1676998740E-1f};
constexpr float4 _ps_cephes_log_p3{-1.2420140846E-1f};
constexpr float4 _ps_cephes_log_p4{+1.4249322787E-1f};
constexpr float4 _ps_cephes_log_p5{-1.6668057665E-1f};
constexpr float4 _ps_cephes_log_p6{+2.0000714765E-1f};
constexpr float4 _ps_cephes_log_p7{-2.4999993993E-1f};
constexpr float4 _ps_cephes_log_p8{+3.3333331174E-1f};
constexpr float4 _ps_cephes_log_q1{-2.12194440e-4f};
constexpr float4 _ps_cephes_log_q2{0.693359375f};

constexpr float4 _ps_exp_hi{88.3762626647949f};
constexpr float4 _ps_exp_lo{-88.3762626647949f};

constexpr float4 _ps_cephes_LOG2EF{1.44269504088896341f};
constexpr float4 _ps_cephes_exp_C1{0.693359375f};
constexpr float4 _ps_cephes_exp_C2{-2.12194440e-4f};

constexpr float4 _ps_cephes_exp_p0{1.9875691500E-4f};
constexpr float4 _ps_cephes_exp_p1{1.3981999507E-3f};
constexpr float4 _ps_cephes_exp_p2{8.3334519073E-3f};
constexpr float4 _ps_cephes_exp_p3{4.1665795894E-2f};
constexpr float4 _ps_cephes_exp_p4{1.6666665459E-1f};
constexpr float4 _ps_cephes_exp_p5{5.0000001201E-1f};

constexpr float4 _ps_minus_cephes_DP1{-0.78515625f};
constexpr float4 _ps_minus_cephes_DP2{-2.4187564849853515625e-4f};
constexpr float4 _ps_minus_cephes_DP3{-3.77489497744594108e-8f};
constexpr float4 _ps_sincof_p0{-1.9515295891E-4f};
constexpr float4 _ps_sincof_p1{8.3321608736E-3f};
constexpr float4 _ps_sincof_p2{-1.6666654611E-1f};
constexpr float4 _ps_coscof_p0{2.443315711809948E-005f};
constexpr float4 _ps_coscof_p1{-1.388731625493765E-003f};
constexpr float4 _ps_coscof_p2{4.166664568298827E-002f};
constexpr float4 _ps_cephes_FOPI{1.27323954473516f};  // 4 / M_PI

// Integer constants
constexpr int4 _pi32_min_norm_pos{0x00800000};  // the smallest non denormalized float number
constexpr int4 _pi32_mant_mask{0x7f800000};
constexpr int4 _pi32_inv_mant_mask{~0x7f800000};
constexpr int4 _pi32_sign_mask{(int32_t)0x80000000};
constexpr int4 _pi32_inv_sign_mask{~0x80000000};
constexpr int4 _pi32_1{1};
constexpr int4 _pi32_inv1{~1};
constexpr int4 _pi32_2{2};
constexpr int4 _pi32_4{4};
constexpr int4 _pi32_0x7f{0x7f};

/* natural logarithm computed for 4 simultaneous float
 return NaN for x <= 0
 */
inline float4 vecLog(float4 x)
{
  int4 emm0;
  float4 one = _ps_1;
  float4 invalid_mask = _mm_cmple_ps(x, _mm_setzero_ps());

  x = _mm_max_ps(x, _pi32_min_norm_pos); /* cut off denormalized stuff */

  emm0 = _mm_srli_epi32(float4ToInt4(x), 23);

  /* keep only the fractional part */
  x = _mm_and_ps(x, _pi32_inv_mant_mask);
  x = _mm_or_ps(x, _ps_0p5);

  emm0 = _mm_sub_epi32(emm0, _pi32_0x7f);
  float4 e = _mm_cvtepi32_ps(emm0);

  e = _mm_add_ps(e, one);

  /* part2:
   if( x < SQRTHF ) {
   e -= 1;
   x = x + x - 1.0;
   } else { x = x - 1.0; }
   */
  float4 mask = _mm_cmplt_ps(x, _ps_cephes_SQRTHF);
  float4 tmp = _mm_and_ps(x, mask);
  x = _mm_sub_ps(x, one);
  e = _mm_sub_ps(e, _mm_and_ps(one, mask));
  x = _mm_add_ps(x, tmp);

  float4 z = _mm_mul_ps(x, x);

  float4 y = _ps_cephes_log_p0;
  y = _mm_mul_ps(y, x);
  y = _mm_add_ps(y, _ps_cephes_log_p1);
  y = _mm_mul_ps(y, x);
  y = _mm_add_ps(y, _ps_cephes_log_p2);
  y = _mm_mul_ps(y, x);
  y = _mm_add_ps(y, _ps_cephes_log_p3);
  y = _mm_mul_ps(y, x);
  y = _mm_add_ps(y, _ps_cephes_log_p4);
  y = _mm_mul_ps(y, x);
  y = _mm_add_ps(y, _ps_cephes_log_p5);
  y = _mm_mul_ps(y, x);
  y = _mm_add_ps(y, _ps_cephes_log_p6);
  y = _mm_mul_ps(y, x);
  y = _mm_add_ps(y, _ps_cephes_log_p7);
  y = _mm_mul_ps(y, x);
  y = _mm_add_ps(y, _ps_cephes_log_p8);
  y = _mm_mul_ps(y, x);

  y = _mm_mul_ps(y, z);

  tmp = _mm_mul_ps(e, _ps_cephes_log_q1);
  y = _mm_add_ps(y, tmp);

  tmp = _mm_mul_ps(z, _ps_0p5);
  y = _mm_sub_ps(y, tmp);

  tmp = _mm_mul_ps(e, _ps_cephes_log_q2);
  x = _mm_add_ps(x, y);
  x = _mm_add_ps(x, tmp);
  x = _mm_or_ps(x, invalid_mask);  // negative arg will be NAN
  return x;
}

inline float4 vecExp(float4 x)
{
  float4 tmp = _mm_setzero_ps(), fx;
  int4 emm0;
  float4 one = _ps_1;

  x = _mm_min_ps(x, _ps_exp_hi);
  x = _mm_max_ps(x, _ps_exp_lo);

  /* express exp(x) as exp(g + n*log(2)) */
  fx = _mm_mul_ps(x, _ps_cephes_LOG2EF);
  fx = _mm_add_ps(fx, _ps_0p5);

  /* how to perform a floorf with SSE: just below */
  emm0 = _mm_cvttps_epi32(fx);
  tmp = _mm_cvtepi32_ps(emm0);

  /* if greater, substract 1 */
  float4 mask = _mm_cmpgt_ps(tmp, fx);
  mask = _mm_and_ps(mask, one);
  fx = _mm_sub_ps(tmp, mask);

  tmp = _mm_mul_ps(fx, _ps_cephes_exp_C1);
  float4 z = _mm_mul_ps(fx, _ps_cephes_exp_C2);
  x = _mm_sub_ps(x, tmp);
  x = _mm_sub_ps(x, z);
  z = _mm_mul_ps(x, x);

  float4 y = _ps_cephes_exp_p0;
  y = _mm_mul_ps(y, x);
  y = _mm_add_ps(y, _ps_cephes_exp_p1);
  y = _mm_mul_ps(y, x);
  y = _mm_add_ps(y, _ps_cephes_exp_p2);
  y = _mm_mul_ps(y, x);
  y = _mm_add_ps(y, _ps_cephes_exp_p3);
  y = _mm_mul_ps(y, x);
  y = _mm_add_ps(y, _ps_cephes_exp_p4);
  y = _mm_mul_ps(y, x);
  y = _mm_add_ps(y, _ps_cephes_exp_p5);
  y = _mm_mul_ps(y, z);
  y = _mm_add_ps(y, x);
  y = _mm_add_ps(y, one);

  /* build 2^n */
  emm0 = _mm_cvttps_epi32(fx);
  emm0 = _mm_add_epi32(emm0, _pi32_0x7f);
  emm0 = _mm_slli_epi32(emm0, 23);
  float4 pow2n = int4ToFloat4(emm0);

  y = _mm_mul_ps(y, pow2n);
  return y;
}

inline float4 vecSin(float4 x)
{
  float4 xmm1, xmm2 = _mm_setzero_ps(), xmm3, sign_bit, y;
  int4 emm0, emm2;

  sign_bit = x;
  /* take the absolute value */
  x = _mm_and_ps(x, _pi32_inv_sign_mask);
  /* extract the sign bit (upper one) */
  sign_bit = _mm_and_ps(sign_bit, _pi32_sign_mask);

  /* scale by 4/Pi */
  y = _mm_mul_ps(x, _ps_cephes_FOPI);

  /* store the integer part of y in mm0 */
  emm2 = _mm_cvttps_epi32(y);
  /* j=(j+1) & (~1) (see the cephes sources) */
  emm2 = _mm_add_epi32(emm2, _pi32_1);
  emm2 = _mm_and_si128(emm2, _pi32_inv1);
  y = _mm_cvtepi32_ps(emm2);

  /* get the swap sign flag */
  emm0 = _mm_and_si128(emm2, _pi32_4);
  emm0 = _mm_slli_epi32(emm0, 29);
  /* get the polynom selection mask
   there is one polynom for 0 <= x <= Pi/4
   and another one for Pi/4<x<=Pi/2
   Both branches will be computed.
   */
  emm2 = _mm_and_si128(emm2, _pi32_2);
  emm2 = _mm_cmpeq_epi32(emm2, _mm_setzero_si128());

  float4 swap_sign_bit = int4ToFloat4(emm0);
  float4 poly_mask = int4ToFloat4(emm2);
  sign_bit = _mm_xor_ps(sign_bit, swap_sign_bit);

  /* The magic pass: "Extended precision modular arithmetic"
   x = ((x - y * DP1) - y * DP2) - y * DP3; */
  xmm1 = _ps_minus_cephes_DP1;
  xmm2 = _ps_minus_cephes_DP2;
  xmm3 = _ps_minus_cephes_DP3;
  xmm1 = _mm_mul_ps(y, xmm1);
  xmm2 = _mm_mul_ps(y, xmm2);
  xmm3 = _mm_mul_ps(y, xmm3);
  x = _mm_add_ps(x, xmm1);
  x = _mm_add_ps(x, xmm2);
  x = _mm_add_ps(x, xmm3);

  /* Evaluate the first polynom  (0 <= x <= Pi/4) */
  y = _ps_coscof_p0;
  float4 z = _mm_mul_ps(x, x);

  y = _mm_mul_ps(y, z);
  y = _mm_add_ps(y, _ps_coscof_p1);
  y = _mm_mul_ps(y, z);
  y = _mm_add_ps(y, _ps_coscof_p2);
  y = _mm_mul_ps(y, z);
  y = _mm_mul_ps(y, z);
  float4 tmp = _mm_mul_ps(z, _ps_0p5);
  y = _mm_sub_ps(y, tmp);
  y = _mm_add_ps(y, _ps_1);

  /* Evaluate the second polynom  (Pi/4 <= x <= 0) */
  float4 y2 = _ps_sincof_p0;
  y2 = _mm_mul_ps(y2, z);
  y2 = _mm_add_ps(y2, _ps_sincof_p1);
  y2 = _mm_mul_ps(y2, z);
  y2 = _mm_add_ps(y2, _ps_sincof_p2);
  y2 = _mm_mul_ps(y2, z);
  y2 = _mm_mul_ps(y2, x);
  y2 = _mm_add_ps(y2, x);

  /* select the correct result from the two polynoms */
  xmm3 = poly_mask;
  y2 = _mm_and_ps(xmm3, y2);  //, xmm3);
  y = _mm_andnot_ps(xmm3, y);
  y = _mm_add_ps(y, y2);
  /* update the sign */
  y = _mm_xor_ps(y, sign_bit);
  return y;
}

/* almost the same as sin_ps */
inline float4 vecCos(float4 x)
{
  float4 xmm1, xmm2 = _mm_setzero_ps(), xmm3, y;
  int4 emm0, emm2;

  /* take the absolute value */
  x = _mm_and_ps(x, _pi32_inv_sign_mask);

  /* scale by 4/Pi */
  y = _mm_mul_ps(x, _ps_cephes_FOPI);

  /* store the integer part of y in mm0 */
  emm2 = _mm_cvttps_epi32(y);
  /* j=(j+1) & (~1) (see the cephes sources) */
  emm2 = _mm_add_epi32(emm2, _pi32_1);
  emm2 = _mm_and_si128(emm2, _pi32_inv1);
  y = _mm_cvtepi32_ps(emm2);
  emm2 = _mm_sub_epi32(emm2, _pi32_2);

  /* get the swap sign flag */
  emm0 = _mm_andnot_si128(emm2, _pi32_4);
  emm0 = _mm_slli_epi32(emm0, 29);
  /* get the polynom selection mask */
  emm2 = _mm_and_si128(emm2, _pi32_2);
  emm2 = _mm_cmpeq_epi32(emm2, _mm_setzero_si128());

  float4 sign_bit = int4ToFloat4(emm0);
  float4 poly_mask = int4ToFloat4(emm2);

  /* The magic pass: "Extended precision modular arithmetic"
   x = ((x - y * DP1) - y * DP2) - y * DP3; */
  xmm1 = _ps_minus_cephes_DP1;
  xmm2 = _ps_minus_cephes_DP2;
  xmm3 = _ps_minus_cephes_DP3;
  xmm1 = _mm_mul_ps(y, xmm1);
  xmm2 = _mm_mul_ps(y, xmm2);
  xmm3 = _mm_mul_ps(y, xmm3);
  x = _mm_add_ps(x, xmm1);
  x = _mm_add_ps(x, xmm2);
  x = _mm_add_ps(x, xmm3);

  /* Evaluate the first polynom  (0 <= x <= Pi/4) */
  y = _ps_coscof_p0;
  float4 z = _mm_mul_ps(x, x);

  y = _mm_mul_ps(y, z);
  y = _mm_add_ps(y, _ps_coscof_p1);
  y = _mm_mul_ps(y, z);
  y = _mm_add_ps(y, _ps_coscof_p2);
  y = _mm_mul_ps(y, z);
  y = _mm_mul_ps(y, z);
  float4 tmp = _mm_mul_ps(z, _ps_0p5);
  y = _mm_sub_ps(y, tmp);
  y = _mm_add_ps(y, _ps_1);

  /* Evaluate the second polynom  (Pi/4 <= x <= 0) */
  float4 y2 = _ps_sincof_p0;
  y2 = _mm_mul_ps(y2, z);
  y2 = _mm_add_ps(y2, _ps_sincof_p1);
  y2 = _mm_mul_ps(y2, z);
  y2 = _mm_add_ps(y2, _ps_sincof_p2);
  y2 = _mm_mul_ps(y2, z);
  y2 = _mm_mul_ps(y2, x);
  y2 = _mm_add_ps(y2, x);

  /* select the correct result from the two polynoms */
  xmm3 = poly_mask;
  y2 = _mm_and_ps(xmm3, y2);  //, xmm3);
  y = _mm_andnot_ps(xmm3, y);
  y = _mm_add_ps(y, y2);
  /* update the sign */
  y = _mm_xor_ps(y, sign_bit);

  return y;
}

/* since sin_ps and cos_ps are almost identical, sincos_ps could replace both of
 them.. it is almost as fast, and gives you a free cosine with your sine */
inline void vecSinCos(float4 x, float4* s, float4* c)
{
  float4 xmm1, xmm2, xmm3 = _mm_setzero_ps(), sign_bit_sin, y;
  int4 emm0, emm2, emm4;

  sign_bit_sin = x;
  /* take the absolute value */
  x = _mm_and_ps(x, _pi32_inv_sign_mask);
  /* extract the sign bit (upper one) */
  sign_bit_sin = _mm_and_ps(sign_bit_sin, _pi32_sign_mask);

  /* scale by 4/Pi */
  y = _mm_mul_ps(x, _ps_cephes_FOPI);

  /* store the integer part of y in emm2 */
  emm2 = _mm_cvttps_epi32(y);

  /* j=(j+1) & (~1) (see the cephes sources) */
  emm2 = _mm_add_epi32(emm2, _pi32_1);
  emm2 = _mm_and_si128(emm2, _pi32_inv1);
  y = _mm_cvtepi32_ps(emm2);

  emm4 = emm2;

  /* get the swap sign flag for the sine */
  emm0 = _mm_and_si128(emm2, _pi32_4);
  emm0 = _mm_slli_epi32(emm0, 29);
  float4 swap_sign_bit_sin = int4ToFloat4(emm0);

  /* get the polynom selection mask for the sine*/
  emm2 = _mm_and_si128(emm2, _pi32_2);
  emm2 = _mm_cmpeq_epi32(emm2, _mm_setzero_si128());
  float4 poly_mask = int4ToFloat4(emm2);

  /* The magic pass: "Extended precision modular arithmetic"
   x = ((x - y * DP1) - y * DP2) - y * DP3; */
  xmm1 = _ps_minus_cephes_DP1;
  xmm2 = _ps_minus_cephes_DP2;
  xmm3 = _ps_minus_cephes_DP3;
  xmm1 = _mm_mul_ps(y, xmm1);
  xmm2 = _mm_mul_ps(y, xmm2);
  xmm3 = _mm_mul_ps(y, xmm3);
  x = _mm_add_ps(x, xmm1);
  x = _mm_add_ps(x, xmm2);
  x = _mm_add_ps(x, xmm3);

  emm4 = _mm_sub_epi32(emm4, _pi32_2);
  emm4 = _mm_andnot_si128(emm4, _pi32_4);
  emm4 = _mm_slli_epi32(emm4, 29);
  float4 sign_bit_cos = int4ToFloat4(emm4);

  sign_bit_sin = _mm_xor_ps(sign_bit_sin, swap_sign_bit_sin);

  /* Evaluate the first polynom  (0 <= x <= Pi/4) */
  float4 z = _mm_mul_ps(x, x);
  y = _ps_coscof_p0;

  y = _mm_mul_ps(y, z);
  y = _mm_add_ps(y, _ps_coscof_p1);
  y = _mm_mul_ps(y, z);
  y = _mm_add_ps(y, _ps_coscof_p2);
  y = _mm_mul_ps(y, z);
  y = _mm_mul_ps(y, z);
  float4 tmp = _mm_mul_ps(z, _ps_0p5);
  y = _mm_sub_ps(y, tmp);
  y = _mm_add_ps(y, _ps_1);

  /* Evaluate the second polynom  (Pi/4 <= x <= 0) */
  float4 y2 = _ps_sincof_p0;
  y2 = _mm_mul_ps(y2, z);
  y2 = _mm_add_ps(y2, _ps_sincof_p1);
  y2 = _mm_mul_ps(y2, z);
  y2 = _mm_add_ps(y2, _ps_sincof_p2);
  y2 = _mm_mul_ps(y2, z);
  y2 = _mm_mul_ps(y2, x);
  y2 = _mm_add_ps(y2, x);

  /* select the correct result from the two polynoms */
  xmm3 = poly_mask;
  float4 ysin2 = _mm_and_ps(xmm3, y2);
  float4 ysin1 = _mm_andnot_ps(xmm3, y);
  y2 = _mm_sub_ps(y2, ysin2);
  y = _mm_sub_ps(y, ysin1);

  xmm1 = _mm_add_ps(ysin1, ysin2);
  xmm2 = _mm_add_ps(y, y2);

  /* update the sign */
  *s = _mm_xor_ps(xmm1, sign_bit_sin);
  *c = _mm_xor_ps(xmm2, sign_bit_cos);
}

// fast polynomial approximations
// from scalar code by Jacques-Henri Jourdan <jourgun@gmail.com>
// sin and cos valid from -pi to pi
// exp and log polynomials generated using Sollya http://sollya.gforge.inria.fr/
// exp Generated in Sollya with:
// > f=remez(1-x*exp(-(x-1)*log(2)),
// [|1,(x-1)*(x-2), (x-1)*(x-2)*x, (x-1)*(x-2)*x*x|],
// [1,2], exp(-(x-1)*log(2)));
// > plot(exp((x-1)*log(2))/(f+x)-1, [1,2]);
// > f+x;
//
// log Generated in Sollya using :
// f = remez(log(x)-(x-1)*log(2),
// [|1,(x-1)*(x-2), (x-1)*(x-2)*x, (x-1)*(x-2)*x*x,
// (x-1)*(x-2)*x*x*x|], [1,2], 1, 1e-8);
// > plot(f+(x-1)*log(2)-log(x), [1,2]);
// > f+(x-1)*log(2)

constexpr float4 kSinC1Vec{0.99997937679290771484375f};
constexpr float4 kSinC2Vec{-0.166624367237091064453125f};
constexpr float4 kSinC3Vec{8.30897875130176544189453125e-3f};
constexpr float4 kSinC4Vec{-1.92649182281456887722015380859375e-4f};
constexpr float4 kSinC5Vec{2.147840177713078446686267852783203125e-6f};

inline __m128 vecSinApprox(__m128 x)
{
  __m128 x2 = _mm_mul_ps(x, x);
  return _mm_mul_ps(
      x, _mm_add_ps(
             kSinC1Vec,
             _mm_mul_ps(
                 x2,
                 _mm_add_ps(
                     kSinC2Vec,
                     _mm_mul_ps(
                         x2, _mm_add_ps(kSinC3Vec,
                                        _mm_mul_ps(x2, _mm_add_ps(kSinC4Vec,
                                                                  _mm_mul_ps(x2, kSinC5Vec)))))))));
}

constexpr float4 kCosC1Vec{0.999959766864776611328125f};
constexpr float4 kCosC2Vec{-0.4997930824756622314453125f};
constexpr float4 kCosC3Vec{4.1496001183986663818359375e-2f};
constexpr float4 kCosC4Vec{-1.33926304988563060760498046875e-3f};
constexpr float4 kCosC5Vec{1.8791708498611114919185638427734375e-5f};

inline float4 vecCosApprox(float4 x)
{
  float4 x2 = _mm_mul_ps(x, x);
  return _mm_add_ps(
      kCosC1Vec,
      _mm_mul_ps(
          x2,
          _mm_add_ps(
              kCosC2Vec,
              _mm_mul_ps(x2, _mm_add_ps(kCosC3Vec,
                                        _mm_mul_ps(x2, _mm_add_ps(kCosC4Vec,
                                                                  _mm_mul_ps(x2, kCosC5Vec))))))));
}

constexpr float4 kExpC1Vec{2139095040.f};
constexpr float4 kExpC2Vec{12102203.1615614f};
constexpr float4 kExpC3Vec{1065353216.f};
constexpr float4 kExpC4Vec{0.510397365625862338668154f};
constexpr float4 kExpC5Vec{0.310670891004095530771135f};
constexpr float4 kExpC6Vec{0.168143436463395944830000f};
constexpr float4 kExpC7Vec{-2.88093587581985443087955e-3f};
constexpr float4 kExpC8Vec{1.3671023382430374383648148e-2f};

inline float4 vecExpApprox(float4 x)
{
  const float4 kZeroVec = _mm_setzero_ps();

  float4 val2, val3, val4;
  int4 val4i;

  val2 = _mm_add_ps(_mm_mul_ps(x, kExpC2Vec), kExpC3Vec);
  val3 = _mm_min_ps(val2, kExpC1Vec);
  val4 = _mm_max_ps(val3, kZeroVec);
  val4i = _mm_cvttps_epi32(val4);

  float4 xu = _mm_and_ps(int4ToFloat4(val4i), int4ToFloat4(_mm_set1_epi32(0x7F800000)));
  float4 b = _mm_or_ps(_mm_and_ps(int4ToFloat4(val4i), int4ToFloat4(_mm_set1_epi32(0x7FFFFF))),
                                int4ToFloat4(_mm_set1_epi32(0x3F800000)));

  return _mm_mul_ps(
      xu,
      (_mm_add_ps(
          kExpC4Vec,
          _mm_mul_ps(
              b, _mm_add_ps(
                     kExpC5Vec,
                     _mm_mul_ps(
                         b, _mm_add_ps(kExpC6Vec,
                                       _mm_mul_ps(b, _mm_add_ps(kExpC7Vec,
                                                                _mm_mul_ps(b, kExpC8Vec))))))))));
}

constexpr float4 kLogC1Vec{-89.970756366f};
constexpr float4 kLogC2Vec{3.529304993f};
constexpr float4 kLogC3Vec{-2.461222105f};
constexpr float4 kLogC4Vec{1.130626167f};
constexpr float4 kLogC5Vec{-0.288739945f};
constexpr float4 kLogC6Vec{3.110401639e-2f};
constexpr float4 kLogC7Vec{0.69314718055995f};

inline float4 vecLogApprox(float4 val)
{
  int4 vZero = _mm_setzero_si128();
  int4 valAsInt = float4ToInt4(val);
  int4 expi = _mm_srli_epi32(valAsInt, 23);
  float4 addcst =
      vecSelectFFI(kLogC1Vec, _mm_set1_ps(FLT_MIN), float4ToInt4(_mm_cmpgt_ps(val, int4ToFloat4(vZero))));
  int4 valAsIntMasked =
      float4ToInt4(_mm_or_ps(_mm_and_ps(int4ToFloat4(valAsInt), int4ToFloat4(_mm_set1_epi32(0x7FFFFF))),
                       int4ToFloat4(_mm_set1_epi32(0x3F800000))));
  float4 x = int4ToFloat4(valAsIntMasked);

  float4 poly = _mm_mul_ps(
      x, _mm_add_ps(
             kLogC2Vec,
             _mm_mul_ps(
                 x, _mm_add_ps(
                        kLogC3Vec,
                        _mm_mul_ps(
                            x, _mm_add_ps(kLogC4Vec,
                                          _mm_mul_ps(x, _mm_add_ps(kLogC5Vec,
                                                                   _mm_mul_ps(x, kLogC6Vec)))))))));

  float4 addCstResult = _mm_add_ps(addcst, _mm_mul_ps(kLogC7Vec, _mm_cvtepi32_ps(expi)));
  return _mm_add_ps(poly, addCstResult);
}

// rough cubic tanh approx, valid in [-4, 4]

constexpr float4 k9Vec{9.0f};
constexpr float4 k27Vec{27.0f};

inline float4 vecTanhApprox(float4 x)
{
  float4 x2 = _mm_mul_ps(x, x);
  float4 denom = _mm_add_ps(k27Vec, _mm_mul_ps(k9Vec, x2));
  float4 frac = _mm_div_ps(_mm_add_ps(k27Vec, x2), (denom));
  return _mm_mul_ps(x, frac);
}

// conversions

inline float4 vecIntPart(float4 val)
{
  int4 vi = _mm_cvttps_epi32(val);  // convert with truncate
  return (_mm_cvtepi32_ps(vi));
}

inline float4 vecFracPart(float4 val)
{
  int4 vi = _mm_cvttps_epi32(val);  // convert with truncate
  float4 intPart = _mm_cvtepi32_ps(vi);
  return _mm_sub_ps(val, intPart);
}

// shuffles

// Given vectors [ ?, ?, ?, 3 ], [ 4, 5, 6, 7 ]
// Returns [ 3, 4, 5, 6 ]
inline float4 vecShuffleRight(float4 v1, float4 v2)
{
  return _mm_shuffle_ps(_mm_shuffle_ps(v2, v1, SHUFFLE(3, 3, 0, 0)), v2, SHUFFLE(2, 1, 0, 3));
}

// Given vectors [ 0, 1, 2, 3 ], [ 4, ?, ?, ? ]
// Returns [ 1, 2, 3, 4 ]
inline float4 vecShuffleLeft(float4 v1, float4 v2)
{
  return _mm_shuffle_ps(v1, _mm_shuffle_ps(v1, v2, SHUFFLE(0, 0, 3, 3)), SHUFFLE(3, 0, 2, 1));
}

// define infix operators for native SSE / MSVC.
#ifndef ML_SSE_TO_NEON
#ifdef WIN32

inline float4 operator*(const float4& a, const float4& b)
{
  return vecMul(a, b);
}

inline float4 operator+(const float4& a, const float4& b)
{
  return vecAdd(a, b);
}

inline float4 operator-(const float4& a, const float4& b)
{
  return vecSub(a, b);
}

inline float4 operator/(const float4& a, const float4& b)
{
  return vecDiv(a, b);
}


#endif
#endif
