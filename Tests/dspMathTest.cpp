// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// Unit tests for MLDSPMath - low-level SIMD primitives

#include "catch.hpp"
#include "MLDSPMath.h"
#include "MLDSPMathApprox.h"

#include <cmath>
#include <limits>

namespace {

// Helper to check if two floats are nearly equal
bool nearlyEqual(float a, float b, float eps = 1e-6f) {
  return std::fabs(a - b) < eps;
}

// Helper to check if two float4s are nearly equal
bool nearlyEqual(float4 a, float4 b, float eps = 1e-6f) {
  for (int i = 0; i < 4; ++i) {
    if (!nearlyEqual(getFloat4Lane(a, i), getFloat4Lane(b, i), eps)) {
      return false;
    }
  }
  return true;
}

// Helper to check exact equality of float4
bool exactlyEqual(float4 a, float4 b) {
  for (int i = 0; i < 4; ++i) {
    if (getFloat4Lane(a, i) != getFloat4Lane(b, i)) {
      return false;
    }
  }
  return true;
}

// Helper to check exact equality of int4
bool exactlyEqual(int4 a, int4 b) {
  alignas(16) int32_t va[4], vb[4];
  storeInt4(va, a);
  storeInt4(vb, b);
  return va[0] == vb[0] && va[1] == vb[1] && va[2] == vb[2] && va[3] == vb[3];
}

} // anonymous namespace

// ================================================================
// float4 type tests
// ================================================================

TEST_CASE("madronalib/dsp_math/float4_construction", "[dsp_math]")
{
  SECTION("default construction")
  {
    float4 v;
    // Just verify it doesn't crash - values are undefined
    (void)v;
  }

  SECTION("broadcast construction")
  {
    float4 v(3.14f);
    REQUIRE(getFloat4Lane(v, 0) == 3.14f);
    REQUIRE(getFloat4Lane(v, 1) == 3.14f);
    REQUIRE(getFloat4Lane(v, 2) == 3.14f);
    REQUIRE(getFloat4Lane(v, 3) == 3.14f);
  }

  SECTION("element-wise construction")
  {
    float4 v(1.0f, 2.0f, 3.0f, 4.0f);
    REQUIRE(getFloat4Lane(v, 0) == 1.0f);
    REQUIRE(getFloat4Lane(v, 1) == 2.0f);
    REQUIRE(getFloat4Lane(v, 2) == 3.0f);
    REQUIRE(getFloat4Lane(v, 3) == 4.0f);
  }
}

TEST_CASE("madronalib/dsp_math/float4_load_store", "[dsp_math]")
{
  SECTION("aligned load/store")
  {
    alignas(16) float src[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    alignas(16) float dst[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    float4 v = loadFloat4(src);
    storeFloat4(dst, v);

    REQUIRE(dst[0] == 1.0f);
    REQUIRE(dst[1] == 2.0f);
    REQUIRE(dst[2] == 3.0f);
    REQUIRE(dst[3] == 4.0f);
  }
}

TEST_CASE("madronalib/dsp_math/float4_lane_access", "[dsp_math]")
{
  SECTION("get lane")
  {
    float4 v(10.0f, 20.0f, 30.0f, 40.0f);
    REQUIRE(getFloat4Lane(v, 0) == 10.0f);
    REQUIRE(getFloat4Lane(v, 1) == 20.0f);
    REQUIRE(getFloat4Lane(v, 2) == 30.0f);
    REQUIRE(getFloat4Lane(v, 3) == 40.0f);
  }

  SECTION("set lane")
  {
    float4 v(0.0f);
    setFloat4Lane(v, 0, 1.0f);
    setFloat4Lane(v, 1, 2.0f);
    setFloat4Lane(v, 2, 3.0f);
    setFloat4Lane(v, 3, 4.0f);

    REQUIRE(getFloat4Lane(v, 0) == 1.0f);
    REQUIRE(getFloat4Lane(v, 1) == 2.0f);
    REQUIRE(getFloat4Lane(v, 2) == 3.0f);
    REQUIRE(getFloat4Lane(v, 3) == 4.0f);
  }
}

// ================================================================
// int4 type tests
// ================================================================

TEST_CASE("madronalib/dsp_math/int4_construction", "[dsp_math]")
{
  SECTION("broadcast construction")
  {
    int4 v(42);
    alignas(16) int32_t dst[4];
    storeInt4(dst, v);
    REQUIRE(dst[0] == 42);
    REQUIRE(dst[1] == 42);
    REQUIRE(dst[2] == 42);
    REQUIRE(dst[3] == 42);
  }

  SECTION("element-wise construction")
  {
    int4 v(1, 2, 3, 4);
    alignas(16) int32_t dst[4];
    storeInt4(dst, v);
    REQUIRE(dst[0] == 1);
    REQUIRE(dst[1] == 2);
    REQUIRE(dst[2] == 3);
    REQUIRE(dst[3] == 4);
  }
}

TEST_CASE("madronalib/dsp_math/int4_load_store", "[dsp_math]")
{
  alignas(16) int32_t src[4] = {10, 20, 30, 40};
  alignas(16) int32_t dst[4] = {0, 0, 0, 0};

  int4 v = loadInt4(src);
  storeInt4(dst, v);

  REQUIRE(dst[0] == 10);
  REQUIRE(dst[1] == 20);
  REQUIRE(dst[2] == 30);
  REQUIRE(dst[3] == 40);
}

// ================================================================
// float4 arithmetic tests
// ================================================================

TEST_CASE("madronalib/dsp_math/float4_arithmetic", "[dsp_math]")
{
  float4 a(1.0f, 2.0f, 3.0f, 4.0f);
  float4 b(5.0f, 6.0f, 7.0f, 8.0f);

  SECTION("addition")
  {
    float4 c = a + b;
    REQUIRE(getFloat4Lane(c, 0) == 6.0f);
    REQUIRE(getFloat4Lane(c, 1) == 8.0f);
    REQUIRE(getFloat4Lane(c, 2) == 10.0f);
    REQUIRE(getFloat4Lane(c, 3) == 12.0f);
  }

  SECTION("subtraction")
  {
    float4 c = b - a;
    REQUIRE(getFloat4Lane(c, 0) == 4.0f);
    REQUIRE(getFloat4Lane(c, 1) == 4.0f);
    REQUIRE(getFloat4Lane(c, 2) == 4.0f);
    REQUIRE(getFloat4Lane(c, 3) == 4.0f);
  }

  SECTION("multiplication")
  {
    float4 c = a * b;
    REQUIRE(getFloat4Lane(c, 0) == 5.0f);
    REQUIRE(getFloat4Lane(c, 1) == 12.0f);
    REQUIRE(getFloat4Lane(c, 2) == 21.0f);
    REQUIRE(getFloat4Lane(c, 3) == 32.0f);
  }

  SECTION("division")
  {
    float4 c = b / a;
    REQUIRE(getFloat4Lane(c, 0) == 5.0f);
    REQUIRE(getFloat4Lane(c, 1) == 3.0f);
    REQUIRE(getFloat4Lane(c, 2) == Approx(7.0f / 3.0f));
    REQUIRE(getFloat4Lane(c, 3) == 2.0f);
  }

  SECTION("negation")
  {
    float4 c = -a;
    REQUIRE(getFloat4Lane(c, 0) == -1.0f);
    REQUIRE(getFloat4Lane(c, 1) == -2.0f);
    REQUIRE(getFloat4Lane(c, 2) == -3.0f);
    REQUIRE(getFloat4Lane(c, 3) == -4.0f);
  }

  SECTION("compound assignment +=")
  {
    float4 c = a;
    c += b;
    REQUIRE(getFloat4Lane(c, 0) == 6.0f);
    REQUIRE(getFloat4Lane(c, 1) == 8.0f);
    REQUIRE(getFloat4Lane(c, 2) == 10.0f);
    REQUIRE(getFloat4Lane(c, 3) == 12.0f);
  }

  SECTION("compound assignment -=")
  {
    float4 c = b;
    c -= a;
    REQUIRE(getFloat4Lane(c, 0) == 4.0f);
    REQUIRE(getFloat4Lane(c, 1) == 4.0f);
    REQUIRE(getFloat4Lane(c, 2) == 4.0f);
    REQUIRE(getFloat4Lane(c, 3) == 4.0f);
  }

  SECTION("compound assignment *=")
  {
    float4 c = a;
    c *= b;
    REQUIRE(getFloat4Lane(c, 0) == 5.0f);
    REQUIRE(getFloat4Lane(c, 1) == 12.0f);
    REQUIRE(getFloat4Lane(c, 2) == 21.0f);
    REQUIRE(getFloat4Lane(c, 3) == 32.0f);
  }

  SECTION("compound assignment /=")
  {
    float4 c = b;
    c /= a;
    REQUIRE(getFloat4Lane(c, 0) == 5.0f);
    REQUIRE(getFloat4Lane(c, 1) == 3.0f);
    REQUIRE(getFloat4Lane(c, 2) == Approx(7.0f / 3.0f));
    REQUIRE(getFloat4Lane(c, 3) == 2.0f);
  }
}

// ================================================================
// int4 arithmetic tests
// ================================================================

TEST_CASE("madronalib/dsp_math/int4_arithmetic", "[dsp_math]")
{
  int4 a(1, 2, 3, 4);
  int4 b(10, 20, 30, 40);

  SECTION("addition")
  {
    int4 c = a + b;
    alignas(16) int32_t dst[4];
    storeInt4(dst, c);
    REQUIRE(dst[0] == 11);
    REQUIRE(dst[1] == 22);
    REQUIRE(dst[2] == 33);
    REQUIRE(dst[3] == 44);
  }

  SECTION("subtraction")
  {
    int4 c = b - a;
    alignas(16) int32_t dst[4];
    storeInt4(dst, c);
    REQUIRE(dst[0] == 9);
    REQUIRE(dst[1] == 18);
    REQUIRE(dst[2] == 27);
    REQUIRE(dst[3] == 36);
  }

  SECTION("negation")
  {
    int4 c = -a;
    alignas(16) int32_t dst[4];
    storeInt4(dst, c);
    REQUIRE(dst[0] == -1);
    REQUIRE(dst[1] == -2);
    REQUIRE(dst[2] == -3);
    REQUIRE(dst[3] == -4);
  }

  SECTION("multiply unsigned")
  {
    int4 c = multiplyUnsigned(a, b);
    alignas(16) int32_t dst[4];
    storeInt4(dst, c);
    REQUIRE(dst[0] == 10);
    REQUIRE(dst[1] == 40);
    REQUIRE(dst[2] == 90);
    REQUIRE(dst[3] == 160);
  }
}

// ================================================================
// Math functions tests
// ================================================================

TEST_CASE("madronalib/dsp_math/math_functions", "[dsp_math]")
{
  SECTION("min")
  {
    float4 a(1.0f, 5.0f, 3.0f, 8.0f);
    float4 b(2.0f, 4.0f, 6.0f, 7.0f);
    float4 c = min(a, b);
    REQUIRE(getFloat4Lane(c, 0) == 1.0f);
    REQUIRE(getFloat4Lane(c, 1) == 4.0f);
    REQUIRE(getFloat4Lane(c, 2) == 3.0f);
    REQUIRE(getFloat4Lane(c, 3) == 7.0f);
  }

  SECTION("max")
  {
    float4 a(1.0f, 5.0f, 3.0f, 8.0f);
    float4 b(2.0f, 4.0f, 6.0f, 7.0f);
    float4 c = max(a, b);
    REQUIRE(getFloat4Lane(c, 0) == 2.0f);
    REQUIRE(getFloat4Lane(c, 1) == 5.0f);
    REQUIRE(getFloat4Lane(c, 2) == 6.0f);
    REQUIRE(getFloat4Lane(c, 3) == 8.0f);
  }

  SECTION("clamp")
  {
    float4 val(-1.0f, 0.5f, 1.5f, 3.0f);
    float4 lo(0.0f);
    float4 hi(1.0f);
    float4 c = clamp(val, lo, hi);
    REQUIRE(getFloat4Lane(c, 0) == 0.0f);
    REQUIRE(getFloat4Lane(c, 1) == 0.5f);
    REQUIRE(getFloat4Lane(c, 2) == 1.0f);
    REQUIRE(getFloat4Lane(c, 3) == 1.0f);
  }

  SECTION("sqrt")
  {
    float4 a(1.0f, 4.0f, 9.0f, 16.0f);
    float4 b = sqrt(a);
    REQUIRE(getFloat4Lane(b, 0) == 1.0f);
    REQUIRE(getFloat4Lane(b, 1) == 2.0f);
    REQUIRE(getFloat4Lane(b, 2) == 3.0f);
    REQUIRE(getFloat4Lane(b, 3) == 4.0f);
  }

  SECTION("rsqrt")
  {
    float4 a(1.0f, 4.0f, 16.0f, 64.0f);
    float4 b = rsqrt(a);
    // rsqrt is approximate, allow some tolerance
    REQUIRE(nearlyEqual(getFloat4Lane(b, 0), 1.0f, 0.01f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 1), 0.5f, 0.01f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 2), 0.25f, 0.01f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 3), 0.125f, 0.01f));
  }

  SECTION("rcp")
  {
    float4 a(1.0f, 2.0f, 4.0f, 8.0f);
    float4 b = rcp(a);
    // rcp is approximate, allow some tolerance
    REQUIRE(nearlyEqual(getFloat4Lane(b, 0), 1.0f, 0.01f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 1), 0.5f, 0.01f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 2), 0.25f, 0.01f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 3), 0.125f, 0.01f));
  }

  SECTION("multiplyAdd")
  {
    float4 a(1.0f, 2.0f, 3.0f, 4.0f);
    float4 b(2.0f, 3.0f, 4.0f, 5.0f);
    float4 c(10.0f, 20.0f, 30.0f, 40.0f);
    // result = a*b + c
    float4 d = multiplyAdd(a, b, c);
    REQUIRE(getFloat4Lane(d, 0) == 12.0f);  // 1*2 + 10
    REQUIRE(getFloat4Lane(d, 1) == 26.0f);  // 2*3 + 20
    REQUIRE(getFloat4Lane(d, 2) == 42.0f);  // 3*4 + 30
    REQUIRE(getFloat4Lane(d, 3) == 60.0f);  // 4*5 + 40
  }
}

// ================================================================
// Logical operations tests
// ================================================================

TEST_CASE("madronalib/dsp_math/float4_logical", "[dsp_math]")
{
  // Use bit patterns that are easy to verify
  int4 ia(0x0F0F0F0F);
  int4 ib(0x00FF00FF);
  float4 a = castIntToFloat(ia);
  float4 b = castIntToFloat(ib);

  SECTION("andBits")
  {
    float4 c = andBits(a, b);
    int4 ic = castFloatToInt(c);
    alignas(16) int32_t dst[4];
    storeInt4(dst, ic);
    REQUIRE(dst[0] == 0x000F000F);
    REQUIRE(dst[1] == 0x000F000F);
    REQUIRE(dst[2] == 0x000F000F);
    REQUIRE(dst[3] == 0x000F000F);
  }

  SECTION("orBits")
  {
    float4 c = orBits(a, b);
    int4 ic = castFloatToInt(c);
    alignas(16) int32_t dst[4];
    storeInt4(dst, ic);
    REQUIRE(dst[0] == 0x0FFF0FFF);
    REQUIRE(dst[1] == 0x0FFF0FFF);
    REQUIRE(dst[2] == 0x0FFF0FFF);
    REQUIRE(dst[3] == 0x0FFF0FFF);
  }

  SECTION("xorBits")
  {
    float4 c = xorBits(a, b);
    int4 ic = castFloatToInt(c);
    alignas(16) int32_t dst[4];
    storeInt4(dst, ic);
    REQUIRE(dst[0] == 0x0FF00FF0);
    REQUIRE(dst[1] == 0x0FF00FF0);
    REQUIRE(dst[2] == 0x0FF00FF0);
    REQUIRE(dst[3] == 0x0FF00FF0);
  }

  SECTION("andNotBits")
  {
    // andNotBits(a, b) = (~a) & b
    float4 c = andNotBits(a, b);
    int4 ic = castFloatToInt(c);
    alignas(16) int32_t dst[4];
    storeInt4(dst, ic);
    REQUIRE(dst[0] == 0x00F000F0);
    REQUIRE(dst[1] == 0x00F000F0);
    REQUIRE(dst[2] == 0x00F000F0);
    REQUIRE(dst[3] == 0x00F000F0);
  }
}

TEST_CASE("madronalib/dsp_math/int4_logical", "[dsp_math]")
{
  int4 a(0x0F0F0F0F);
  int4 b(0x00FF00FF);

  SECTION("andBits")
  {
    int4 c = andBits(a, b);
    alignas(16) int32_t dst[4];
    storeInt4(dst, c);
    REQUIRE(dst[0] == 0x000F000F);
  }

  SECTION("orBits")
  {
    int4 c = orBits(a, b);
    alignas(16) int32_t dst[4];
    storeInt4(dst, c);
    REQUIRE(dst[0] == 0x0FFF0FFF);
  }

  SECTION("xorBits")
  {
    int4 c = xorBits(a, b);
    alignas(16) int32_t dst[4];
    storeInt4(dst, c);
    REQUIRE(dst[0] == 0x0FF00FF0);
  }

  SECTION("andNotBits")
  {
    int4 c = andNotBits(a, b);
    alignas(16) int32_t dst[4];
    storeInt4(dst, c);
    REQUIRE(dst[0] == 0x00F000F0);
  }
}

// ================================================================
// Comparison tests
// ================================================================

TEST_CASE("madronalib/dsp_math/float4_comparisons", "[dsp_math]")
{
  float4 a(1.0f, 2.0f, 3.0f, 4.0f);
  float4 b(2.0f, 2.0f, 2.0f, 2.0f);

  SECTION("compareEqual")
  {
    float4 mask = compareEqual(a, b);
    int4 imask = castFloatToInt(mask);
    alignas(16) int32_t dst[4];
    storeInt4(dst, imask);
    REQUIRE(dst[0] == 0);           // 1 != 2
    REQUIRE(dst[1] == -1);          // 2 == 2 (all bits set)
    REQUIRE(dst[2] == 0);           // 3 != 2
    REQUIRE(dst[3] == 0);           // 4 != 2
  }

  SECTION("compareNotEqual")
  {
    float4 mask = compareNotEqual(a, b);
    int4 imask = castFloatToInt(mask);
    alignas(16) int32_t dst[4];
    storeInt4(dst, imask);
    REQUIRE(dst[0] == -1);          // 1 != 2
    REQUIRE(dst[1] == 0);           // 2 == 2
    REQUIRE(dst[2] == -1);          // 3 != 2
    REQUIRE(dst[3] == -1);          // 4 != 2
  }

  SECTION("compareLessThan")
  {
    float4 mask = compareLessThan(a, b);
    int4 imask = castFloatToInt(mask);
    alignas(16) int32_t dst[4];
    storeInt4(dst, imask);
    REQUIRE(dst[0] == -1);          // 1 < 2
    REQUIRE(dst[1] == 0);           // 2 < 2 is false
    REQUIRE(dst[2] == 0);           // 3 < 2 is false
    REQUIRE(dst[3] == 0);           // 4 < 2 is false
  }

  SECTION("compareLessThanOrEqual")
  {
    float4 mask = compareLessThanOrEqual(a, b);
    int4 imask = castFloatToInt(mask);
    alignas(16) int32_t dst[4];
    storeInt4(dst, imask);
    REQUIRE(dst[0] == -1);          // 1 <= 2
    REQUIRE(dst[1] == -1);          // 2 <= 2
    REQUIRE(dst[2] == 0);           // 3 <= 2 is false
    REQUIRE(dst[3] == 0);           // 4 <= 2 is false
  }

  SECTION("compareGreaterThan")
  {
    float4 mask = compareGreaterThan(a, b);
    int4 imask = castFloatToInt(mask);
    alignas(16) int32_t dst[4];
    storeInt4(dst, imask);
    REQUIRE(dst[0] == 0);           // 1 > 2 is false
    REQUIRE(dst[1] == 0);           // 2 > 2 is false
    REQUIRE(dst[2] == -1);          // 3 > 2
    REQUIRE(dst[3] == -1);          // 4 > 2
  }

  SECTION("compareGreaterThanOrEqual")
  {
    float4 mask = compareGreaterThanOrEqual(a, b);
    int4 imask = castFloatToInt(mask);
    alignas(16) int32_t dst[4];
    storeInt4(dst, imask);
    REQUIRE(dst[0] == 0);           // 1 >= 2 is false
    REQUIRE(dst[1] == -1);          // 2 >= 2
    REQUIRE(dst[2] == -1);          // 3 >= 2
    REQUIRE(dst[3] == -1);          // 4 >= 2
  }
}

TEST_CASE("madronalib/dsp_math/int4_comparisons", "[dsp_math]")
{
  int4 a(1, 2, 3, 4);
  int4 b(2, 2, 2, 2);

  SECTION("compareEqualInt")
  {
    int4 mask = compareEqualInt(a, b);
    alignas(16) int32_t dst[4];
    storeInt4(dst, mask);
    REQUIRE(dst[0] == 0);
    REQUIRE(dst[1] == -1);
    REQUIRE(dst[2] == 0);
    REQUIRE(dst[3] == 0);
  }
}

// ================================================================
// Shuffle and move tests
// ================================================================

TEST_CASE("madronalib/dsp_math/float4_shuffle", "[dsp_math]")
{
  float4 a(1.0f, 2.0f, 3.0f, 4.0f);
  float4 b(5.0f, 6.0f, 7.0f, 8.0f);

  SECTION("shuffle basic")
  {
    // shuffle<i0,i1,i2,i3>(a,b) = [a[i0], a[i1], b[i2], b[i3]]
    float4 c = shuffle<0, 1, 0, 1>(a, b);
    REQUIRE(getFloat4Lane(c, 0) == 1.0f);
    REQUIRE(getFloat4Lane(c, 1) == 2.0f);
    REQUIRE(getFloat4Lane(c, 2) == 5.0f);
    REQUIRE(getFloat4Lane(c, 3) == 6.0f);
  }

  SECTION("shuffle reverse")
  {
    float4 c = shuffle<3, 2, 1, 0>(a, a);
    REQUIRE(getFloat4Lane(c, 0) == 4.0f);
    REQUIRE(getFloat4Lane(c, 1) == 3.0f);
    REQUIRE(getFloat4Lane(c, 2) == 2.0f);
    REQUIRE(getFloat4Lane(c, 3) == 1.0f);
  }

  SECTION("unpackLo")
  {
    // interleave low halves: [a0, b0, a1, b1]
    float4 c = unpackLo(a, b);
    REQUIRE(getFloat4Lane(c, 0) == 1.0f);
    REQUIRE(getFloat4Lane(c, 1) == 5.0f);
    REQUIRE(getFloat4Lane(c, 2) == 2.0f);
    REQUIRE(getFloat4Lane(c, 3) == 6.0f);
  }

  SECTION("unpackHi")
  {
    // interleave high halves: [a2, b2, a3, b3]
    float4 c = unpackHi(a, b);
    REQUIRE(getFloat4Lane(c, 0) == 3.0f);
    REQUIRE(getFloat4Lane(c, 1) == 7.0f);
    REQUIRE(getFloat4Lane(c, 2) == 4.0f);
    REQUIRE(getFloat4Lane(c, 3) == 8.0f);
  }

  SECTION("moveLH")
  {
    // [a0, a1, b0, b1]
    float4 c = moveLH(a, b);
    REQUIRE(getFloat4Lane(c, 0) == 1.0f);
    REQUIRE(getFloat4Lane(c, 1) == 2.0f);
    REQUIRE(getFloat4Lane(c, 2) == 5.0f);
    REQUIRE(getFloat4Lane(c, 3) == 6.0f);
  }

  SECTION("moveHL")
  {
    // [b2, b3, a2, a3]
    float4 c = moveHL(a, b);
    REQUIRE(getFloat4Lane(c, 0) == 7.0f);
    REQUIRE(getFloat4Lane(c, 1) == 8.0f);
    REQUIRE(getFloat4Lane(c, 2) == 3.0f);
    REQUIRE(getFloat4Lane(c, 3) == 4.0f);
  }
}

TEST_CASE("madronalib/dsp_math/transpose4x4", "[dsp_math]")
{
  float4 r0(1.0f, 2.0f, 3.0f, 4.0f);
  float4 r1(5.0f, 6.0f, 7.0f, 8.0f);
  float4 r2(9.0f, 10.0f, 11.0f, 12.0f);
  float4 r3(13.0f, 14.0f, 15.0f, 16.0f);

  transpose4x4(r0, r1, r2, r3);

  // After transpose:
  // r0 = column 0: [1, 5, 9, 13]
  // r1 = column 1: [2, 6, 10, 14]
  // r2 = column 2: [3, 7, 11, 15]
  // r3 = column 3: [4, 8, 12, 16]

  REQUIRE(getFloat4Lane(r0, 0) == 1.0f);
  REQUIRE(getFloat4Lane(r0, 1) == 5.0f);
  REQUIRE(getFloat4Lane(r0, 2) == 9.0f);
  REQUIRE(getFloat4Lane(r0, 3) == 13.0f);

  REQUIRE(getFloat4Lane(r1, 0) == 2.0f);
  REQUIRE(getFloat4Lane(r1, 1) == 6.0f);
  REQUIRE(getFloat4Lane(r1, 2) == 10.0f);
  REQUIRE(getFloat4Lane(r1, 3) == 14.0f);

  REQUIRE(getFloat4Lane(r2, 0) == 3.0f);
  REQUIRE(getFloat4Lane(r2, 1) == 7.0f);
  REQUIRE(getFloat4Lane(r2, 2) == 11.0f);
  REQUIRE(getFloat4Lane(r2, 3) == 15.0f);

  REQUIRE(getFloat4Lane(r3, 0) == 4.0f);
  REQUIRE(getFloat4Lane(r3, 1) == 8.0f);
  REQUIRE(getFloat4Lane(r3, 2) == 12.0f);
  REQUIRE(getFloat4Lane(r3, 3) == 16.0f);
}

// ================================================================
// Scalar operations tests
// ================================================================

TEST_CASE("madronalib/dsp_math/scalar_operations", "[dsp_math]")
{
  float4 a(1.0f, 2.0f, 3.0f, 4.0f);
  float4 b(10.0f, 20.0f, 30.0f, 40.0f);

  SECTION("addScalar")
  {
    float4 c = addScalar(a, b);
    REQUIRE(getFloat4Lane(c, 0) == 11.0f);  // 1 + 10
    // Upper lanes come from first operand
    REQUIRE(getFloat4Lane(c, 1) == 2.0f);
    REQUIRE(getFloat4Lane(c, 2) == 3.0f);
    REQUIRE(getFloat4Lane(c, 3) == 4.0f);
  }

  SECTION("maxScalar")
  {
    float4 c = maxScalar(a, b);
    REQUIRE(getFloat4Lane(c, 0) == 10.0f);  // max(1, 10)
  }

  SECTION("minScalar")
  {
    float4 c = minScalar(a, b);
    REQUIRE(getFloat4Lane(c, 0) == 1.0f);   // min(1, 10)
  }

  SECTION("extractScalar")
  {
    float s = extractScalar(a);
    REQUIRE(s == 1.0f);
  }
}

// ================================================================
// Conversion tests
// ================================================================

TEST_CASE("madronalib/dsp_math/conversions", "[dsp_math]")
{
  SECTION("floatToIntRound")
  {
    float4 a(1.4f, 1.6f, -1.4f, -1.6f);
    int4 b = floatToIntRound(a);
    alignas(16) int32_t dst[4];
    storeInt4(dst, b);
    REQUIRE(dst[0] == 1);
    REQUIRE(dst[1] == 2);
    REQUIRE(dst[2] == -1);
    REQUIRE(dst[3] == -2);
  }

  SECTION("floatToIntTruncate")
  {
    float4 a(1.9f, -1.9f, 2.1f, -2.1f);
    int4 b = floatToIntTruncate(a);
    alignas(16) int32_t dst[4];
    storeInt4(dst, b);
    REQUIRE(dst[0] == 1);
    REQUIRE(dst[1] == -1);
    REQUIRE(dst[2] == 2);
    REQUIRE(dst[3] == -2);
  }

  SECTION("intToFloat")
  {
    int4 a(1, -2, 3, -4);
    float4 b = intToFloat(a);
    REQUIRE(getFloat4Lane(b, 0) == 1.0f);
    REQUIRE(getFloat4Lane(b, 1) == -2.0f);
    REQUIRE(getFloat4Lane(b, 2) == 3.0f);
    REQUIRE(getFloat4Lane(b, 3) == -4.0f);
  }

  SECTION("castFloatToInt and castIntToFloat")
  {
    // Test that cast preserves bit patterns
    float4 a(1.0f, 2.0f, 3.0f, 4.0f);
    int4 b = castFloatToInt(a);
    float4 c = castIntToFloat(b);
    REQUIRE(exactlyEqual(a, c));
  }

  SECTION("vecIntPart")
  {
    float4 a(1.7f, -1.7f, 2.3f, -2.3f);
    float4 b = vecIntPart(a);
    REQUIRE(getFloat4Lane(b, 0) == 1.0f);
    REQUIRE(getFloat4Lane(b, 1) == -1.0f);
    REQUIRE(getFloat4Lane(b, 2) == 2.0f);
    REQUIRE(getFloat4Lane(b, 3) == -2.0f);
  }

  SECTION("vecFracPart")
  {
    float4 a(1.25f, -1.25f, 2.75f, -2.75f);
    float4 b = vecFracPart(a);
    REQUIRE(nearlyEqual(getFloat4Lane(b, 0), 0.25f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 1), -0.25f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 2), 0.75f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 3), -0.75f));
  }
}

// ================================================================
// Integer shift tests
// ================================================================

TEST_CASE("madronalib/dsp_math/int4_shifts", "[dsp_math]")
{
  SECTION("shiftLeftElements")
  {
    int4 a(1, 2, 4, 8);
    int4 b = shiftLeftElements(a, 2);  // shift left by 2 bits
    alignas(16) int32_t dst[4];
    storeInt4(dst, b);
    REQUIRE(dst[0] == 4);
    REQUIRE(dst[1] == 8);
    REQUIRE(dst[2] == 16);
    REQUIRE(dst[3] == 32);
  }

  SECTION("shiftRightElements")
  {
    int4 a(4, 8, 16, 32);
    int4 b = shiftRightElements(a, 2);  // shift right by 2 bits
    alignas(16) int32_t dst[4];
    storeInt4(dst, b);
    REQUIRE(dst[0] == 1);
    REQUIRE(dst[1] == 2);
    REQUIRE(dst[2] == 4);
    REQUIRE(dst[3] == 8);
  }

  SECTION("shiftLeftBytes")
  {
    int4 a(0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10);
    int4 b = shiftLeftBytes<4>(a);  // shift left by 4 bytes (one element)
    alignas(16) int32_t dst[4];
    storeInt4(dst, b);
    REQUIRE(dst[0] == 0);
    REQUIRE(dst[1] == 0x01020304);
    REQUIRE(dst[2] == 0x05060708);
    REQUIRE(dst[3] == 0x090A0B0C);
  }

  SECTION("shiftRightBytes")
  {
    int4 a(0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10);
    int4 b = shiftRightBytes<4>(a);  // shift right by 4 bytes (one element)
    alignas(16) int32_t dst[4];
    storeInt4(dst, b);
    REQUIRE(dst[0] == 0x05060708);
    REQUIRE(dst[1] == 0x090A0B0C);
    REQUIRE(dst[2] == 0x0D0E0F10);
    REQUIRE(dst[3] == 0);
  }
}

// ================================================================
// Select functions tests
// ================================================================

TEST_CASE("madronalib/dsp_math/select_functions", "[dsp_math]")
{
  float4 a(1.0f, 2.0f, 3.0f, 4.0f);
  float4 b(10.0f, 20.0f, 30.0f, 40.0f);

  SECTION("vecSelectFFI")
  {
    // mask: select a where mask is all-ones, b where mask is zero
    int4 mask(0, -1, 0, -1);  // select b, a, b, a
    float4 c = vecSelectFFI(a, b, mask);
    REQUIRE(getFloat4Lane(c, 0) == 10.0f);
    REQUIRE(getFloat4Lane(c, 1) == 2.0f);
    REQUIRE(getFloat4Lane(c, 2) == 30.0f);
    REQUIRE(getFloat4Lane(c, 3) == 4.0f);
  }

  SECTION("vecSelectFFF")
  {
    float4 mask = castIntToFloat(int4(0, -1, 0, -1));
    float4 c = vecSelectFFF(a, b, mask);
    REQUIRE(getFloat4Lane(c, 0) == 10.0f);
    REQUIRE(getFloat4Lane(c, 1) == 2.0f);
    REQUIRE(getFloat4Lane(c, 2) == 30.0f);
    REQUIRE(getFloat4Lane(c, 3) == 4.0f);
  }

  SECTION("vecSelectIII")
  {
    int4 ia(1, 2, 3, 4);
    int4 ib(10, 20, 30, 40);
    int4 mask(0, -1, 0, -1);
    int4 c = vecSelectIII(ia, ib, mask);
    alignas(16) int32_t dst[4];
    storeInt4(dst, c);
    REQUIRE(dst[0] == 10);
    REQUIRE(dst[1] == 2);
    REQUIRE(dst[2] == 30);
    REQUIRE(dst[3] == 4);
  }
}

// ================================================================
// Horizontal operations tests
// ================================================================

TEST_CASE("madronalib/dsp_math/horizontal_operations", "[dsp_math]")
{
  SECTION("vecSumH")
  {
    float4 a(1.0f, 2.0f, 3.0f, 4.0f);
    float sum = vecSumH(a);
    REQUIRE(sum == 10.0f);
  }

  SECTION("vecMaxH")
  {
    float4 a(3.0f, 1.0f, 4.0f, 2.0f);
    float m = vecMaxH(a);
    REQUIRE(m == 4.0f);
  }

  SECTION("vecMinH")
  {
    float4 a(3.0f, 1.0f, 4.0f, 2.0f);
    float m = vecMinH(a);
    REQUIRE(m == 1.0f);
  }

  SECTION("vecSumH with negatives")
  {
    float4 a(-1.0f, 2.0f, -3.0f, 4.0f);
    float sum = vecSumH(a);
    REQUIRE(sum == 2.0f);
  }

  SECTION("vecMaxH with negatives")
  {
    float4 a(-3.0f, -1.0f, -4.0f, -2.0f);
    float m = vecMaxH(a);
    REQUIRE(m == -1.0f);
  }

  SECTION("vecMinH with negatives")
  {
    float4 a(-3.0f, -1.0f, -4.0f, -2.0f);
    float m = vecMinH(a);
    REQUIRE(m == -4.0f);
  }
}

// ================================================================
// Shuffle helpers from MLDSPMath.h
// ================================================================

TEST_CASE("madronalib/dsp_math/vec_shuffle_helpers", "[dsp_math]")
{
  float4 v1(1.0f, 2.0f, 3.0f, 4.0f);
  float4 v2(5.0f, 6.0f, 7.0f, 8.0f);

  SECTION("vecShuffleRight")
  {
    // Shifts window right through stream where v1 precedes v2
    // Brings last element of v1 and first 3 elements of v2
    // Result: [v1[3], v2[0], v2[1], v2[2]] = [4, 5, 6, 7]
    float4 r = vecShuffleRight(v1, v2);
    REQUIRE(getFloat4Lane(r, 0) == 4.0f);
    REQUIRE(getFloat4Lane(r, 1) == 5.0f);
    REQUIRE(getFloat4Lane(r, 2) == 6.0f);
    REQUIRE(getFloat4Lane(r, 3) == 7.0f);
  }

  SECTION("vecShuffleLeft")
  {
    // Shifts elements left, bringing in element from v2
    // Result should be: [v1[1], v1[2], v1[3], v2[0]] = [2, 3, 4, 5]
    float4 r = vecShuffleLeft(v1, v2);
    REQUIRE(getFloat4Lane(r, 0) == 2.0f);
    REQUIRE(getFloat4Lane(r, 1) == 3.0f);
    REQUIRE(getFloat4Lane(r, 2) == 4.0f);
    REQUIRE(getFloat4Lane(r, 3) == 5.0f);
  }
}

// ================================================================
// Approximate math functions (from MLDSPMathApprox.h)
// ================================================================

TEST_CASE("madronalib/dsp_math/approx_functions", "[dsp_math]")
{
  // Test approximate functions against standard library
  // Allow reasonable tolerance for approximations

  SECTION("vecLog")
  {
    float4 a(1.0f, 2.0f, 4.0f, 8.0f);
    float4 b = vecLog(a);
    REQUIRE(nearlyEqual(getFloat4Lane(b, 0), std::log(1.0f), 1e-5f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 1), std::log(2.0f), 1e-5f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 2), std::log(4.0f), 1e-5f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 3), std::log(8.0f), 1e-5f));
  }

  SECTION("vecExp")
  {
    float4 a(0.0f, 1.0f, 2.0f, -1.0f);
    float4 b = vecExp(a);
    REQUIRE(nearlyEqual(getFloat4Lane(b, 0), std::exp(0.0f), 1e-5f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 1), std::exp(1.0f), 1e-5f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 2), std::exp(2.0f), 1e-4f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 3), std::exp(-1.0f), 1e-5f));
  }

  SECTION("vecSin")
  {
    float4 a(0.0f, 0.5f, 1.0f, 1.5f);
    float4 b = vecSin(a);
    REQUIRE(nearlyEqual(getFloat4Lane(b, 0), std::sin(0.0f), 1e-5f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 1), std::sin(0.5f), 1e-5f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 2), std::sin(1.0f), 1e-5f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 3), std::sin(1.5f), 1e-5f));
  }

  SECTION("vecCos")
  {
    float4 a(0.0f, 0.5f, 1.0f, 1.5f);
    float4 b = vecCos(a);
    REQUIRE(nearlyEqual(getFloat4Lane(b, 0), std::cos(0.0f), 1e-5f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 1), std::cos(0.5f), 1e-5f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 2), std::cos(1.0f), 1e-5f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 3), std::cos(1.5f), 1e-5f));
  }

  SECTION("vecSinCos")
  {
    float4 a(0.0f, 0.5f, 1.0f, 1.5f);
    float4 s, c;
    vecSinCos(a, &s, &c);

    REQUIRE(nearlyEqual(getFloat4Lane(s, 0), std::sin(0.0f), 1e-5f));
    REQUIRE(nearlyEqual(getFloat4Lane(s, 1), std::sin(0.5f), 1e-5f));
    REQUIRE(nearlyEqual(getFloat4Lane(s, 2), std::sin(1.0f), 1e-5f));
    REQUIRE(nearlyEqual(getFloat4Lane(s, 3), std::sin(1.5f), 1e-5f));

    REQUIRE(nearlyEqual(getFloat4Lane(c, 0), std::cos(0.0f), 1e-5f));
    REQUIRE(nearlyEqual(getFloat4Lane(c, 1), std::cos(0.5f), 1e-5f));
    REQUIRE(nearlyEqual(getFloat4Lane(c, 2), std::cos(1.0f), 1e-5f));
    REQUIRE(nearlyEqual(getFloat4Lane(c, 3), std::cos(1.5f), 1e-5f));
  }
}

TEST_CASE("madronalib/dsp_math/fast_approx_functions", "[dsp_math]")
{
  // Fast approximations have lower precision

  SECTION("vecSinApprox")
  {
    // Valid in range [-pi/2, pi/2]
    float4 a(0.0f, 0.5f, 1.0f, -0.5f);
    float4 b = vecSinApprox(a);
    REQUIRE(nearlyEqual(getFloat4Lane(b, 0), std::sin(0.0f), 1e-4f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 1), std::sin(0.5f), 1e-4f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 2), std::sin(1.0f), 1e-4f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 3), std::sin(-0.5f), 1e-4f));
  }

  SECTION("vecCosApprox")
  {
    // Valid in range [-pi/2, pi/2]
    float4 a(0.0f, 0.5f, 1.0f, -0.5f);
    float4 b = vecCosApprox(a);
    REQUIRE(nearlyEqual(getFloat4Lane(b, 0), std::cos(0.0f), 1e-4f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 1), std::cos(0.5f), 1e-4f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 2), std::cos(1.0f), 1e-4f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 3), std::cos(-0.5f), 1e-4f));
  }

  SECTION("vecExpApprox")
  {
    float4 a(0.0f, 1.0f, 2.0f, -1.0f);
    float4 b = vecExpApprox(a);
    // Lower precision for fast approximation
    REQUIRE(nearlyEqual(getFloat4Lane(b, 0), std::exp(0.0f), 0.01f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 1), std::exp(1.0f), 0.05f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 2), std::exp(2.0f), 0.15f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 3), std::exp(-1.0f), 0.01f));
  }

  SECTION("vecLogApprox")
  {
    float4 a(1.0f, 2.0f, 4.0f, 8.0f);
    float4 b = vecLogApprox(a);
    REQUIRE(nearlyEqual(getFloat4Lane(b, 0), std::log(1.0f), 0.01f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 1), std::log(2.0f), 0.01f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 2), std::log(4.0f), 0.01f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 3), std::log(8.0f), 0.01f));
  }

  SECTION("vecTanhApprox")
  {
    float4 a(0.0f, 0.5f, 1.0f, -0.5f);
    float4 b = vecTanhApprox(a);
    REQUIRE(nearlyEqual(getFloat4Lane(b, 0), std::tanh(0.0f), 0.01f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 1), std::tanh(0.5f), 0.01f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 2), std::tanh(1.0f), 0.02f));
    REQUIRE(nearlyEqual(getFloat4Lane(b, 3), std::tanh(-0.5f), 0.01f));
  }
}

// ================================================================
// Special value tests
// ================================================================

TEST_CASE("madronalib/dsp_math/special_values", "[dsp_math]")
{
  SECTION("setZero")
  {
    float4 z = setZero();
    REQUIRE(getFloat4Lane(z, 0) == 0.0f);
    REQUIRE(getFloat4Lane(z, 1) == 0.0f);
    REQUIRE(getFloat4Lane(z, 2) == 0.0f);
    REQUIRE(getFloat4Lane(z, 3) == 0.0f);
  }

  SECTION("setZeroInt")
  {
    int4 z = setZeroInt();
    alignas(16) int32_t dst[4];
    storeInt4(dst, z);
    REQUIRE(dst[0] == 0);
    REQUIRE(dst[1] == 0);
    REQUIRE(dst[2] == 0);
    REQUIRE(dst[3] == 0);
  }

  SECTION("set1Float")
  {
    float4 v = set1Float(3.14f);
    REQUIRE(getFloat4Lane(v, 0) == 3.14f);
    REQUIRE(getFloat4Lane(v, 1) == 3.14f);
    REQUIRE(getFloat4Lane(v, 2) == 3.14f);
    REQUIRE(getFloat4Lane(v, 3) == 3.14f);
  }

  SECTION("set1Int")
  {
    int4 v = set1Int(42);
    alignas(16) int32_t dst[4];
    storeInt4(dst, v);
    REQUIRE(dst[0] == 42);
    REQUIRE(dst[1] == 42);
    REQUIRE(dst[2] == 42);
    REQUIRE(dst[3] == 42);
  }

  SECTION("setrFloat")
  {
    float4 v = setrFloat(1.0f, 2.0f, 3.0f, 4.0f);
    REQUIRE(getFloat4Lane(v, 0) == 1.0f);
    REQUIRE(getFloat4Lane(v, 1) == 2.0f);
    REQUIRE(getFloat4Lane(v, 2) == 3.0f);
    REQUIRE(getFloat4Lane(v, 3) == 4.0f);
  }

  SECTION("setrInt")
  {
    int4 v = setrInt(1, 2, 3, 4);
    alignas(16) int32_t dst[4];
    storeInt4(dst, v);
    REQUIRE(dst[0] == 1);
    REQUIRE(dst[1] == 2);
    REQUIRE(dst[2] == 3);
    REQUIRE(dst[3] == 4);
  }
}
