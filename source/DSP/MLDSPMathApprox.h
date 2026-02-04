// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// MLDSPMathSSE.h
// SSE implementations of madronalib SIMD primitives

#pragma once

#include "MLDSPMath.h"

// cephes-derived approximate math functions adapted from code by Julien Pommier
// Copyright (C) 2007 Julien Pommier and licensed under the zlib license

// Float constants
static const float4 _ps_1(1.0f);
static const float4 _ps_0p5(0.5f);
static const float4 _ps_cephes_SQRTHF(0.707106781186547524f);
static const float4 _ps_cephes_log_p0(7.0376836292E-2f);
static const float4 _ps_cephes_log_p1(-1.1514610310E-1f);
static const float4 _ps_cephes_log_p2(1.1676998740E-1f);
static const float4 _ps_cephes_log_p3(-1.2420140846E-1f);
static const float4 _ps_cephes_log_p4(+1.4249322787E-1f);
static const float4 _ps_cephes_log_p5(-1.6668057665E-1f);
static const float4 _ps_cephes_log_p6(+2.0000714765E-1f);
static const float4 _ps_cephes_log_p7(-2.4999993993E-1f);
static const float4 _ps_cephes_log_p8(+3.3333331174E-1f);
static const float4 _ps_cephes_log_q1(-2.12194440e-4f);
static const float4 _ps_cephes_log_q2(0.693359375f);
static const float4 _ps_exp_hi(88.3762626647949f);
static const float4 _ps_exp_lo(-88.3762626647949f);
static const float4 _ps_cephes_LOG2EF(1.44269504088896341f);
static const float4 _ps_cephes_exp_C1(0.693359375f);
static const float4 _ps_cephes_exp_C2(-2.12194440e-4f);
static const float4 _ps_cephes_exp_p0(1.9875691500E-4f);
static const float4 _ps_cephes_exp_p1(1.3981999507E-3f);
static const float4 _ps_cephes_exp_p2(8.3334519073E-3f);
static const float4 _ps_cephes_exp_p3(4.1665795894E-2f);
static const float4 _ps_cephes_exp_p4(1.6666665459E-1f);
static const float4 _ps_cephes_exp_p5(5.0000001201E-1f);
static const float4 _ps_minus_cephes_DP1(-0.78515625f);
static const float4 _ps_minus_cephes_DP2(-2.4187564849853515625e-4f);
static const float4 _ps_minus_cephes_DP3(-3.77489497744594108e-8f);
static const float4 _ps_sincof_p0(-1.9515295891E-4f);
static const float4 _ps_sincof_p1(8.3321608736E-3f);
static const float4 _ps_sincof_p2(-1.6666654611E-1f);
static const float4 _ps_coscof_p0(2.443315711809948E-005f);
static const float4 _ps_coscof_p1(-1.388731625493765E-003f);
static const float4 _ps_coscof_p2(4.166664568298827E-002f);
static const float4 _ps_cephes_FOPI(1.27323954473516f);  // 4 / M_PI

// Integer constants
static const int4 _pi32_min_norm_pos(0x00800000);  // the smallest non denormalized float number
static const int4 _pi32_mant_mask(0x7f800000);
static const int4 _pi32_inv_mant_mask(~0x7f800000);
static const int4 _pi32_sign_mask((int32_t)0x80000000);
static const int4 _pi32_inv_sign_mask(~0x80000000);
static const int4 _pi32_1(1);
static const int4 _pi32_inv1(~1);
static const int4 _pi32_2(2);
static const int4 _pi32_4(4);
static const int4 _pi32_0x7f(0x7f);

// polynomial approximation constants

static const float4 kSinC1Vec(0.99997937679290771484375f);
static const float4 kSinC2Vec(-0.166624367237091064453125f);
static const float4 kSinC3Vec(8.30897875130176544189453125e-3f);
static const float4 kSinC4Vec(-1.92649182281456887722015380859375e-4f);
static const float4 kSinC5Vec(2.147840177713078446686267852783203125e-6f);

static const float4 kCosC1Vec(0.999959766864776611328125f);
static const float4 kCosC2Vec(-0.4997930824756622314453125f);
static const float4 kCosC3Vec(4.1496001183986663818359375e-2f);
static const float4 kCosC4Vec(-1.33926304988563060760498046875e-3f);
static const float4 kCosC5Vec(1.8791708498611114919185638427734375e-5f);

static const float4 kExpC1Vec(2139095040.f);
static const float4 kExpC2Vec(12102203.1615614f);
static const float4 kExpC3Vec(1065353216.f);
static const float4 kExpC4Vec(0.510397365625862338668154f);
static const float4 kExpC5Vec(0.310670891004095530771135f);
static const float4 kExpC6Vec(0.168143436463395944830000f);
static const float4 kExpC7Vec(-2.88093587581985443087955e-3f);
static const float4 kExpC8Vec(1.3671023382430374383648148e-2f);

static const float4 kLogC1Vec(-89.970756366f);
static const float4 kLogC2Vec(3.529304993f);
static const float4 kLogC3Vec(-2.461222105f);
static const float4 kLogC4Vec(1.130626167f);
static const float4 kLogC5Vec(-0.288739945f);
static const float4 kLogC6Vec(3.110401639e-2f);
static const float4 kLogC7Vec(0.69314718055995f);

static const float4 k9Vec(9.0f);
static const float4 k27Vec(27.0f);

// ----------------------------------------------------------------
// vecLog - natural logarithm

inline float4 vecLog(float4 x) {
  int4 emm0;
  float4 one = _ps_1;
  float4 invalid_mask = compareLessThanOrEqual(x, setZero());
  
  x = max(x, castIntToFloat(_pi32_min_norm_pos));
  
  emm0 = shiftRightElements(castFloatToInt(x), 23);
  
  x = andBits(x, castIntToFloat(_pi32_inv_mant_mask));
  x = orBits(x, _ps_0p5);
  
  emm0 = emm0 - _pi32_0x7f;
  float4 e = intToFloat(emm0);
  
  e = e + one;
  
  float4 mask = compareLessThan(x, _ps_cephes_SQRTHF);
  float4 tmp = andBits(x, mask);
  x = x - one;
  e = e - andBits(one, mask);
  x = x + tmp;
  
  float4 z = x * x;
  
  float4 y = _ps_cephes_log_p0;
  y = y * x;
  y = y + _ps_cephes_log_p1;
  y = y * x;
  y = y + _ps_cephes_log_p2;
  y = y * x;
  y = y + _ps_cephes_log_p3;
  y = y * x;
  y = y + _ps_cephes_log_p4;
  y = y * x;
  y = y + _ps_cephes_log_p5;
  y = y * x;
  y = y + _ps_cephes_log_p6;
  y = y * x;
  y = y + _ps_cephes_log_p7;
  y = y * x;
  y = y + _ps_cephes_log_p8;
  y = y * x;
  
  y = y * z;
  
  tmp = e * _ps_cephes_log_q1;
  y = y + tmp;
  
  tmp = z * _ps_0p5;
  y = y - tmp;
  
  tmp = e * _ps_cephes_log_q2;
  x = x + y;
  x = x + tmp;
  x = orBits(x, invalid_mask);
  return x;
}

// ----------------------------------------------------------------
// vecExp - exponential

inline float4 vecExp(float4 x) {
  float4 tmp = setZero(), fx;
  int4 emm0;
  float4 one = _ps_1;
  
  x = min(x, _ps_exp_hi);
  x = max(x, _ps_exp_lo);
  
  fx = x * _ps_cephes_LOG2EF;
  fx = fx + _ps_0p5;
  
  emm0 = floatToIntTruncate(fx);
  tmp = intToFloat(emm0);
  
  float4 mask = compareGreaterThan(tmp, fx);
  mask = andBits(mask, one);
  fx = tmp - mask;
  
  tmp = fx * _ps_cephes_exp_C1;
  float4 z = fx * _ps_cephes_exp_C2;
  x = x - tmp;
  x = x - z;
  z = x * x;
  
  float4 y = _ps_cephes_exp_p0;
  y = y * x;
  y = y + _ps_cephes_exp_p1;
  y = y * x;
  y = y + _ps_cephes_exp_p2;
  y = y * x;
  y = y + _ps_cephes_exp_p3;
  y = y * x;
  y = y + _ps_cephes_exp_p4;
  y = y * x;
  y = y + _ps_cephes_exp_p5;
  y = y * z;
  y = y + x;
  y = y + one;
  
  emm0 = floatToIntTruncate(fx);
  emm0 = emm0 + _pi32_0x7f;
  emm0 = shiftLeftElements(emm0, 23);
  float4 pow2n = castIntToFloat(emm0);
  
  y = y * pow2n;
  return y;
}

// ----------------------------------------------------------------
// vecSin - sine

inline float4 vecSin(float4 x) {
  float4 xmm1, xmm2 = setZero(), xmm3, sign_bit, y;
  int4 emm0, emm2;
  
  sign_bit = x;
  x = andBits(x, castIntToFloat(_pi32_inv_sign_mask));
  sign_bit = andBits(sign_bit, castIntToFloat(_pi32_sign_mask));
  
  y = x * _ps_cephes_FOPI;
  
  emm2 = floatToIntTruncate(y);
  emm2 = emm2 + _pi32_1;
  emm2 = andBits(emm2, _pi32_inv1);
  y = intToFloat(emm2);
  
  emm0 = andBits(emm2, _pi32_4);
  emm0 = shiftLeftElements(emm0, 29);
  
  emm2 = andBits(emm2, _pi32_2);
  emm2 = compareEqualInt(emm2, setZeroInt());
  
  float4 swap_sign_bit = castIntToFloat(emm0);
  float4 poly_mask = castIntToFloat(emm2);
  sign_bit = xorBits(sign_bit, swap_sign_bit);
  
  xmm1 = _ps_minus_cephes_DP1;
  xmm2 = _ps_minus_cephes_DP2;
  xmm3 = _ps_minus_cephes_DP3;
  xmm1 = y * xmm1;
  xmm2 = y * xmm2;
  xmm3 = y * xmm3;
  x = x + xmm1;
  x = x + xmm2;
  x = x + xmm3;
  
  y = _ps_coscof_p0;
  float4 z = x * x;
  
  y = y * z;
  y = y + _ps_coscof_p1;
  y = y * z;
  y = y + _ps_coscof_p2;
  y = y * z;
  y = y * z;
  float4 tmp = z * _ps_0p5;
  y = y - tmp;
  y = y + _ps_1;
  
  float4 y2 = _ps_sincof_p0;
  y2 = y2 * z;
  y2 = y2 + _ps_sincof_p1;
  y2 = y2 * z;
  y2 = y2 + _ps_sincof_p2;
  y2 = y2 * z;
  y2 = y2 * x;
  y2 = y2 + x;
  
  xmm3 = poly_mask;
  y2 = andBits(xmm3, y2);
  y = andNotBits(xmm3, y);
  y = y + y2;
  y = xorBits(y, sign_bit);
  return y;
}

// ----------------------------------------------------------------
// vecCos - cosine

inline float4 vecCos(float4 x) {
  float4 xmm1, xmm2 = setZero(), xmm3, y;
  int4 emm0, emm2;
  
  x = andBits(x, castIntToFloat(_pi32_inv_sign_mask));
  
  y = x * _ps_cephes_FOPI;
  
  emm2 = floatToIntTruncate(y);
  emm2 = emm2 + _pi32_1;
  emm2 = andBits(emm2, _pi32_inv1);
  y = intToFloat(emm2);
  emm2 = emm2 - _pi32_2;
  
  emm0 = andNotBits(emm2, _pi32_4);
  emm0 = shiftLeftElements(emm0, 29);
  
  emm2 = andBits(emm2, _pi32_2);
  emm2 = compareEqualInt(emm2, setZeroInt());
  
  float4 sign_bit = castIntToFloat(emm0);
  float4 poly_mask = castIntToFloat(emm2);
  
  xmm1 = _ps_minus_cephes_DP1;
  xmm2 = _ps_minus_cephes_DP2;
  xmm3 = _ps_minus_cephes_DP3;
  xmm1 = y * xmm1;
  xmm2 = y * xmm2;
  xmm3 = y * xmm3;
  x = x + xmm1;
  x = x + xmm2;
  x = x + xmm3;
  
  y = _ps_coscof_p0;
  float4 z = x * x;
  
  y = y * z;
  y = y + _ps_coscof_p1;
  y = y * z;
  y = y + _ps_coscof_p2;
  y = y * z;
  y = y * z;
  float4 tmp = z * _ps_0p5;
  y = y - tmp;
  y = y + _ps_1;
  
  float4 y2 = _ps_sincof_p0;
  y2 = y2 * z;
  y2 = y2 + _ps_sincof_p1;
  y2 = y2 * z;
  y2 = y2 + _ps_sincof_p2;
  y2 = y2 * z;
  y2 = y2 * x;
  y2 = y2 + x;
  
  xmm3 = poly_mask;
  y2 = andBits(xmm3, y2);
  y = andNotBits(xmm3, y);
  y = y + y2;
  y = xorBits(y, sign_bit);
  
  return y;
}

// ----------------------------------------------------------------
// vecSinCos - simultaneous sine and cosine

inline void vecSinCos(float4 x, float4* s, float4* c) {
  float4 xmm1, xmm2, xmm3 = setZero(), sign_bit_sin, y;
  int4 emm0, emm2, emm4;
  
  sign_bit_sin = x;
  x = andBits(x, castIntToFloat(_pi32_inv_sign_mask));
  sign_bit_sin = andBits(sign_bit_sin, castIntToFloat(_pi32_sign_mask));
  
  y = x * _ps_cephes_FOPI;
  
  emm2 = floatToIntTruncate(y);
  
  emm2 = emm2 + _pi32_1;
  emm2 = andBits(emm2, _pi32_inv1);
  y = intToFloat(emm2);
  
  emm4 = emm2;
  
  emm0 = andBits(emm2, _pi32_4);
  emm0 = shiftLeftElements(emm0, 29);
  float4 swap_sign_bit_sin = castIntToFloat(emm0);
  
  emm2 = andBits(emm2, _pi32_2);
  emm2 = compareEqualInt(emm2, setZeroInt());
  float4 poly_mask = castIntToFloat(emm2);
  
  xmm1 = _ps_minus_cephes_DP1;
  xmm2 = _ps_minus_cephes_DP2;
  xmm3 = _ps_minus_cephes_DP3;
  xmm1 = y * xmm1;
  xmm2 = y * xmm2;
  xmm3 = y * xmm3;
  x = x + xmm1;
  x = x + xmm2;
  x = x + xmm3;
  
  emm4 = emm4 - _pi32_2;
  emm4 = andNotBits(emm4, _pi32_4);
  emm4 = shiftLeftElements(emm4, 29);
  float4 sign_bit_cos = castIntToFloat(emm4);
  
  sign_bit_sin = xorBits(sign_bit_sin, swap_sign_bit_sin);
  
  float4 z = x * x;
  y = _ps_coscof_p0;
  
  y = y * z;
  y = y + _ps_coscof_p1;
  y = y * z;
  y = y + _ps_coscof_p2;
  y = y * z;
  y = y * z;
  float4 tmp = z * _ps_0p5;
  y = y - tmp;
  y = y + _ps_1;
  
  float4 y2 = _ps_sincof_p0;
  y2 = y2 * z;
  y2 = y2 + _ps_sincof_p1;
  y2 = y2 * z;
  y2 = y2 + _ps_sincof_p2;
  y2 = y2 * z;
  y2 = y2 * x;
  y2 = y2 + x;
  
  xmm3 = poly_mask;
  float4 ysin2 = andBits(xmm3, y2);
  float4 ysin1 = andNotBits(xmm3, y);
  y2 = y2 - ysin2;
  y = y - ysin1;
  
  xmm1 = ysin1 + ysin2;
  xmm2 = y + y2;
  
  *s = xorBits(xmm1, sign_bit_sin);
  *c = xorBits(xmm2, sign_bit_cos);
}

// ----------------------------------------------------------------
// Fast polynomial approximations

inline float4 vecSinApprox(float4 x) {
  float4 x2 = x * x;
  return x * (kSinC1Vec + (x2 * (kSinC2Vec + (x2 * (kSinC3Vec + (x2 * (kSinC4Vec + (x2 * kSinC5Vec))))))));
}

inline float4 vecCosApprox(float4 x) {
  float4 x2 = x * x;
  return (kCosC1Vec + (x2 * (kCosC2Vec + (x2 * (kCosC3Vec + (x2 * (kCosC4Vec + (x2 * kCosC5Vec))))))));
}

inline float4 vecExpApprox(float4 x) {
  const float4 kZeroVec = setZero();
  
  float4 val2, val3, val4;
  int4 val4i;
  
  val2 = (x * kExpC2Vec) + kExpC3Vec;
  val3 = min(val2, kExpC1Vec);
  val4 = max(val3, kZeroVec);
  val4i = floatToIntTruncate(val4);
  
  float4 xu = andBits(castIntToFloat(val4i), castIntToFloat(set1Int(0x7F800000)));
  float4 b = orBits(andBits(castIntToFloat(val4i), castIntToFloat(set1Int(0x7FFFFF))),
                    castIntToFloat(set1Int(0x3F800000)));
  
  return xu * (kExpC4Vec + (b * (kExpC5Vec + (b * (kExpC6Vec + (b * (kExpC7Vec + (b * kExpC8Vec))))))));
}

inline float4 vecLogApprox(float4 val) {
  int4 vZero = setZeroInt();
  int4 valAsInt = castFloatToInt(val);
  int4 expi = shiftRightElements(valAsInt, 23);
  float4 addcst = vecSelectFFI(kLogC1Vec, set1Float(FLT_MIN),
                               castFloatToInt(compareGreaterThan(val, castIntToFloat(vZero))));
  int4 valAsIntMasked =
  castFloatToInt(orBits(andBits(castIntToFloat(valAsInt), castIntToFloat(set1Int(0x7FFFFF))),
                        castIntToFloat(set1Int(0x3F800000))));
  float4 x = castIntToFloat(valAsIntMasked);
  
  float4 poly = x * (kLogC2Vec + (x * (kLogC3Vec + (x * (kLogC4Vec + (x * (kLogC5Vec + (x * kLogC6Vec))))))));
  
  float4 addCstResult = addcst + (kLogC7Vec * intToFloat(expi));
  return poly + addCstResult;
}

inline float4 vecTanhApprox(float4 x) {
  float4 x2 = x * x;
  float4 denom = k27Vec + (k9Vec * x2);
  float4 frac = (k27Vec + x2) / denom;
  return x * frac;
}
