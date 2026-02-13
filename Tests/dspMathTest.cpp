// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// Unit tests for MLDSPMath - low-level SIMD primitives

#include "catch.hpp"
#include "MLDSPMath.h"
#include "MLDSPMathApprox.h"

//#include <cmath>
//#include <limits>

using namespace ml;

namespace {

// Equality check for float4 (for use in REQUIRE)
bool eq(float4 a, float4 b) {
  for (int i = 0; i < 4; ++i) {
    if (getFloat4Lane(a, i) != getFloat4Lane(b, i)) return false;
  }
  return true;
}

// Equality check for int4 (for use in REQUIRE)
bool eq(int4 a, int4 b) {
  alignas(16) int32_t va[4], vb[4];
  storeInt4(va, a);
  storeInt4(vb, b);
  return va[0] == vb[0] && va[1] == vb[1] && va[2] == vb[2] && va[3] == vb[3];
}

// Helper to check if two float4s are nearly equal
bool nearlyEqual(float4 a, float4 b, float eps = 1e-6f) {
  for (int i = 0; i < 4; ++i) {
    if (std::fabs(getFloat4Lane(a, i) - getFloat4Lane(b, i)) >= eps) return false;
  }
  return true;
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
    (void)v; // Just verify it doesn't crash
  }

  SECTION("broadcast construction")
  {
    REQUIRE(eq(float4(3.14f), float4(3.14f, 3.14f, 3.14f, 3.14f)));
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
  alignas(16) float src[4] = {1.0f, 2.0f, 3.0f, 4.0f};
  alignas(16) float dst[4] = {0};

  float4 v = loadFloat4(src);
  storeFloat4(dst, v);

  REQUIRE(eq(v, float4(1.0f, 2.0f, 3.0f, 4.0f)));
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
    REQUIRE(eq(v, float4(1.0f, 2.0f, 3.0f, 4.0f)));
  }
}

// ================================================================
// int4 type tests
// ================================================================

TEST_CASE("madronalib/dsp_math/int4_construction", "[dsp_math]")
{
  SECTION("broadcast construction")
  {
    REQUIRE(eq(int4(42), int4(42, 42, 42, 42)));
  }

  SECTION("element-wise construction")
  {
    REQUIRE(eq(int4(1, 2, 3, 4), int4(1, 2, 3, 4)));
  }
}

TEST_CASE("madronalib/dsp_math/int4_load_store", "[dsp_math]")
{
  alignas(16) int32_t src[4] = {10, 20, 30, 40};
  alignas(16) int32_t dst[4] = {0};

  int4 v = loadInt4(src);
  storeInt4(dst, v);

  REQUIRE(eq(v, int4(10, 20, 30, 40)));
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
    REQUIRE(eq(a + b, float4(6.0f, 8.0f, 10.0f, 12.0f)));
  }

  SECTION("subtraction")
  {
    REQUIRE(eq(b - a, float4(4.0f, 4.0f, 4.0f, 4.0f)));
  }

  SECTION("multiplication")
  {
    REQUIRE(eq(a * b, float4(5.0f, 12.0f, 21.0f, 32.0f)));
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
    REQUIRE(eq(-a, float4(-1.0f, -2.0f, -3.0f, -4.0f)));
  }

  SECTION("compound assignment +=")
  {
    float4 c = a; c += b;
    REQUIRE(eq(c, float4(6.0f, 8.0f, 10.0f, 12.0f)));
  }

  SECTION("compound assignment -=")
  {
    float4 c = b; c -= a;
    REQUIRE(eq(c, float4(4.0f, 4.0f, 4.0f, 4.0f)));
  }

  SECTION("compound assignment *=")
  {
    float4 c = a; c *= b;
    REQUIRE(eq(c, float4(5.0f, 12.0f, 21.0f, 32.0f)));
  }

  SECTION("compound assignment /=")
  {
    float4 c = b; c /= a;
    REQUIRE(getFloat4Lane(c, 2) == Approx(7.0f / 3.0f));
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
    REQUIRE(eq(a + b, int4(11, 22, 33, 44)));
  }

  SECTION("subtraction")
  {
    REQUIRE(eq(b - a, int4(9, 18, 27, 36)));
  }

  SECTION("negation")
  {
    REQUIRE(eq(-a, int4(-1, -2, -3, -4)));
  }

  SECTION("multiply unsigned")
  {
    REQUIRE(eq(multiplyUnsigned(a, b), int4(10, 40, 90, 160)));
  }
}

// ================================================================
// Math functions tests
// ================================================================

TEST_CASE("madronalib/dsp_math/math_functions", "[dsp_math]")
{
  SECTION("min")
  {
    REQUIRE(eq(min(float4(1.0f, 5.0f, 3.0f, 8.0f), float4(2.0f, 4.0f, 6.0f, 7.0f)),
               float4(1.0f, 4.0f, 3.0f, 7.0f)));
  }

  SECTION("max")
  {
    REQUIRE(eq(max(float4(1.0f, 5.0f, 3.0f, 8.0f), float4(2.0f, 4.0f, 6.0f, 7.0f)),
               float4(2.0f, 5.0f, 6.0f, 8.0f)));
  }

  SECTION("clamp")
  {
    REQUIRE(eq(clamp(float4(-1.0f, 0.5f, 1.5f, 3.0f), float4(0.0f), float4(1.0f)),
               float4(0.0f, 0.5f, 1.0f, 1.0f)));
  }

  SECTION("sqrt")
  {
    REQUIRE(eq(sqrt(float4(1.0f, 4.0f, 9.0f, 16.0f)), float4(1.0f, 2.0f, 3.0f, 4.0f)));
  }

  SECTION("rsqrt")
  {
    REQUIRE(nearlyEqual(rsqrt(float4(1.0f, 4.0f, 16.0f, 64.0f)),
                        float4(1.0f, 0.5f, 0.25f, 0.125f), 0.01f));
  }

  SECTION("rcp")
  {
    REQUIRE(nearlyEqual(rcp(float4(1.0f, 2.0f, 4.0f, 8.0f)),
                        float4(1.0f, 0.5f, 0.25f, 0.125f), 0.01f));
  }

  SECTION("multiplyAdd")
  {
    REQUIRE(eq(multiplyAdd(float4(1.0f, 2.0f, 3.0f, 4.0f),
                           float4(2.0f, 3.0f, 4.0f, 5.0f),
                           float4(10.0f, 20.0f, 30.0f, 40.0f)),
               float4(12.0f, 26.0f, 42.0f, 60.0f)));
  }
}

// ================================================================
// Logical operations tests
// ================================================================

TEST_CASE("madronalib/dsp_math/float4_logical", "[dsp_math]")
{
  int4 ia(0x0F0F0F0F);
  int4 ib(0x00FF00FF);
  float4 a = reinterpretIntAsFloat(ia);
  float4 b = reinterpretIntAsFloat(ib);

  SECTION("andBits")    { REQUIRE(eq(reinterpretFloatAsInt(andBits(a, b)), int4(0x000F000F))); }
  SECTION("orBits")     { REQUIRE(eq(reinterpretFloatAsInt(orBits(a, b)), int4(0x0FFF0FFF))); }
  SECTION("xorBits")    { REQUIRE(eq(reinterpretFloatAsInt(xorBits(a, b)), int4(0x0FF00FF0))); }
  SECTION("andNotBits") { REQUIRE(eq(reinterpretFloatAsInt(andNotBits(a, b)), int4(0x00F000F0))); }
}

TEST_CASE("madronalib/dsp_math/int4_logical", "[dsp_math]")
{
  int4 a(0x0F0F0F0F);
  int4 b(0x00FF00FF);

  SECTION("andBits")    { REQUIRE(eq(andBits(a, b), int4(0x000F000F))); }
  SECTION("orBits")     { REQUIRE(eq(orBits(a, b), int4(0x0FFF0FFF))); }
  SECTION("xorBits")    { REQUIRE(eq(xorBits(a, b), int4(0x0FF00FF0))); }
  SECTION("andNotBits") { REQUIRE(eq(andNotBits(a, b), int4(0x00F000F0))); }
}

// ================================================================
// Comparison tests
// ================================================================

TEST_CASE("madronalib/dsp_math/float4_comparisons", "[dsp_math]")
{
  float4 a(1.0f, 2.0f, 3.0f, 4.0f);
  float4 b(2.0f, 2.0f, 2.0f, 2.0f);

  SECTION("compareEqual")              { REQUIRE(eq(reinterpretFloatAsInt((a == b)), int4(0, -1, 0, 0))); }
  SECTION("compareNotEqual")           { REQUIRE(eq(reinterpretFloatAsInt((a != b)), int4(-1, 0, -1, -1))); }
  SECTION("compareLessThan")           { REQUIRE(eq(reinterpretFloatAsInt((a < b)), int4(-1, 0, 0, 0))); }
  SECTION("compareLessThanOrEqual")    { REQUIRE(eq(reinterpretFloatAsInt((a <= b)), int4(-1, -1, 0, 0))); }
  SECTION("compareGreaterThan")        { REQUIRE(eq(reinterpretFloatAsInt((a > b)), int4(0, 0, -1, -1))); }
  SECTION("compareGreaterThanOrEqual") { REQUIRE(eq(reinterpretFloatAsInt((a >= b)), int4(0, -1, -1, -1))); }
}

TEST_CASE("madronalib/dsp_math/int4_comparisons", "[dsp_math]")
{
  REQUIRE(eq(compareEqualInt(int4(1, 2, 3, 4), int4(2, 2, 2, 2)), int4(0, -1, 0, 0)));
}

// ================================================================
// Shuffle and move tests
// ================================================================

TEST_CASE("madronalib/dsp_math/float4_shuffle", "[dsp_math]")
{
  float4 a(1.0f, 2.0f, 3.0f, 4.0f);
  float4 b(5.0f, 6.0f, 7.0f, 8.0f);

  SECTION("shuffle basic")   { REQUIRE(eq(shuffle<0, 1, 0, 1>(a, b), float4(1.0f, 2.0f, 5.0f, 6.0f))); }
  SECTION("shuffle reverse") { REQUIRE(eq(shuffle<3, 2, 1, 0>(a, a), float4(4.0f, 3.0f, 2.0f, 1.0f))); }
  SECTION("unpackLo")        { REQUIRE(eq(unpackLo(a, b), float4(1.0f, 5.0f, 2.0f, 6.0f))); }
  SECTION("unpackHi")        { REQUIRE(eq(unpackHi(a, b), float4(3.0f, 7.0f, 4.0f, 8.0f))); }
  SECTION("moveLH")          { REQUIRE(eq(moveLH(a, b), float4(1.0f, 2.0f, 5.0f, 6.0f))); }
  SECTION("moveHL")          { REQUIRE(eq(moveHL(a, b), float4(7.0f, 8.0f, 3.0f, 4.0f))); }
}

TEST_CASE("madronalib/dsp_math/transpose4x4", "[dsp_math]")
{
  float4 r[4];
  
  r[0] = float4(1.0f, 2.0f, 3.0f, 4.0f);
  r[1] = float4(5.0f, 6.0f, 7.0f, 8.0f);
  r[2] = float4(9.0f, 10.0f, 11.0f, 12.0f);
  r[3] = float4(13.0f, 14.0f, 15.0f, 16.0f);

  transpose4x4InPlace(r);

  REQUIRE(eq(r[0], float4(1.0f, 5.0f, 9.0f, 13.0f)));
  REQUIRE(eq(r[1], float4(2.0f, 6.0f, 10.0f, 14.0f)));
  REQUIRE(eq(r[2], float4(3.0f, 7.0f, 11.0f, 15.0f)));
  REQUIRE(eq(r[3], float4(4.0f, 8.0f, 12.0f, 16.0f)));
}

// ================================================================
// Scalar operations tests
// ================================================================

TEST_CASE("madronalib/dsp_math/scalar_operations", "[dsp_math]")
{
  float4 a(1.0f, 2.0f, 3.0f, 4.0f);
  float4 b(10.0f, 20.0f, 30.0f, 40.0f);

  SECTION("addScalar")     { REQUIRE(eq(addScalar(a, b), float4(11.0f, 2.0f, 3.0f, 4.0f))); }
  SECTION("maxScalar")     { REQUIRE(getFloat4Lane(maxScalar(a, b), 0) == 10.0f); }
  SECTION("minScalar")     { REQUIRE(getFloat4Lane(minScalar(a, b), 0) == 1.0f); }
  SECTION("extractScalar") { REQUIRE(extractScalar(a) == 1.0f); }
}

// ================================================================
// Conversion tests
// ================================================================

TEST_CASE("madronalib/dsp_math/conversions", "[dsp_math]")
{
  SECTION("floatToIntRound")
  {
    REQUIRE(eq(floatToIntRound(float4(1.4f, 1.6f, -1.4f, -1.6f)), int4(1, 2, -1, -2)));
  }

  SECTION("floatToIntTruncate")
  {
    REQUIRE(eq(floatToIntTruncate(float4(1.9f, -1.9f, 2.1f, -2.1f)), int4(1, -1, 2, -2)));
  }

  SECTION("intToFloat")
  {
    REQUIRE(eq(intToFloat(int4(1, -2, 3, -4)), float4(1.0f, -2.0f, 3.0f, -4.0f)));
  }

  SECTION("reinterpretFloatAsInt and reinterpretIntAsFloat")
  {
    float4 a(1.0f, 2.0f, 3.0f, 4.0f);
    REQUIRE(eq(reinterpretIntAsFloat(reinterpretFloatAsInt(a)), a));
  }

  SECTION("intPart")
  {
    REQUIRE(eq(intPart(float4(1.7f, -1.7f, 2.3f, -2.3f)), float4(1.0f, -1.0f, 2.0f, -2.0f)));
  }

  SECTION("vecFracPart")
  {
    REQUIRE(nearlyEqual(fracPart(float4(1.25f, -1.25f, 2.75f, -2.75f)),
                        float4(0.25f, -0.25f, 0.75f, -0.75f)));
  }
}

// ================================================================
// Integer shift tests
// ================================================================

TEST_CASE("madronalib/dsp_math/int4_shifts", "[dsp_math]")
{
  SECTION("shiftLeftElements")
  {
    REQUIRE(eq(shiftLeftElements(int4(1, 2, 4, 8), 2), int4(4, 8, 16, 32)));
  }

  SECTION("shiftRightElements")
  {
    REQUIRE(eq(shiftRightElements(int4(4, 8, 16, 32), 2), int4(1, 2, 4, 8)));
  }

  SECTION("shiftLeftBytes")
  {
    REQUIRE(eq(shiftLeftBytes<4>(int4(0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10)),
               int4(0, 0x01020304, 0x05060708, 0x090A0B0C)));
  }

  SECTION("shiftRightBytes")
  {
    REQUIRE(eq(shiftRightBytes<4>(int4(0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10)),
               int4(0x05060708, 0x090A0B0C, 0x0D0E0F10, 0)));
  }
}

// ================================================================
// Select functions tests
// ================================================================

TEST_CASE("madronalib/dsp_math/select_functions", "[dsp_math]")
{
  float4 a(1.0f, 2.0f, 3.0f, 4.0f);
  float4 b(10.0f, 20.0f, 30.0f, 40.0f);
  int4 mask(0, -1, 0, -1);

  SECTION("select") { REQUIRE(eq(select(a, b, reinterpretIntAsFloat(mask)), float4(10.0f, 2.0f, 30.0f, 4.0f))); }
}

// ================================================================
// Horizontal operations tests
// ================================================================

TEST_CASE("madronalib/dsp_math/horizontal_operations", "[dsp_math]")
{
  SECTION("vecSumH")
  {
    REQUIRE(vecSumH(float4(1.0f, 2.0f, 3.0f, 4.0f)) == 10.0f);
    REQUIRE(vecSumH(float4(-1.0f, 2.0f, -3.0f, 4.0f)) == 2.0f);
  }

  SECTION("vecMaxH")
  {
    REQUIRE(vecMaxH(float4(3.0f, 1.0f, 4.0f, 2.0f)) == 4.0f);
    REQUIRE(vecMaxH(float4(-3.0f, -1.0f, -4.0f, -2.0f)) == -1.0f);
  }

  SECTION("vecMinH")
  {
    REQUIRE(vecMinH(float4(3.0f, 1.0f, 4.0f, 2.0f)) == 1.0f);
    REQUIRE(vecMinH(float4(-3.0f, -1.0f, -4.0f, -2.0f)) == -4.0f);
  }
}

// ================================================================
// Shuffle helpers from MLDSPMath.h
// ================================================================

TEST_CASE("madronalib/dsp_math/vec_shuffle_helpers", "[dsp_math]")
{
  float4 v1(1.0f, 2.0f, 3.0f, 4.0f);
  float4 v2(5.0f, 6.0f, 7.0f, 8.0f);

  SECTION("vecShuffleRight") { REQUIRE(eq(vecShuffleRight(v1, v2), float4(4.0f, 5.0f, 6.0f, 7.0f))); }
  SECTION("vecShuffleLeft")  { REQUIRE(eq(vecShuffleLeft(v1, v2), float4(2.0f, 3.0f, 4.0f, 5.0f))); }
}

// ================================================================
// Approximate math functions (from MLDSPMathApprox.h)
// ================================================================

TEST_CASE("madronalib/dsp_math/approx_functions", "[dsp_math]")
{
  SECTION("vecLog")
  {
    REQUIRE(nearlyEqual(log(float4(1.0f, 2.0f, 4.0f, 8.0f)),
            float4(std::log(1.0f), std::log(2.0f), std::log(4.0f), std::log(8.0f)), 1e-5f));
  }

  SECTION("exp")
  {
    REQUIRE(nearlyEqual(exp(float4(0.0f, 1.0f, 2.0f, -1.0f)),
            float4(std::exp(0.0f), std::exp(1.0f), std::exp(2.0f), std::exp(-1.0f)), 1e-4f));
  }

  SECTION("vecSin")
  {
    REQUIRE(nearlyEqual(sin(float4(0.0f, 0.5f, 1.0f, 1.5f)),
            float4(std::sin(0.0f), std::sin(0.5f), std::sin(1.0f), std::sin(1.5f)), 1e-5f));
  }

  SECTION("cos")
  {
    REQUIRE(nearlyEqual(cos(float4(0.0f, 0.5f, 1.0f, 1.5f)),
            float4(std::cos(0.0f), std::cos(0.5f), std::cos(1.0f), std::cos(1.5f)), 1e-5f));
  }

  SECTION("vecSinCos")
  {
    float4 a(0.0f, 0.5f, 1.0f, 1.5f);
    float4 s, c;
    auto sincospair = sincos(a);
    REQUIRE(nearlyEqual(sincospair.first, float4(std::sin(0.0f), std::sin(0.5f), std::sin(1.0f), std::sin(1.5f)), 1e-5f));
    REQUIRE(nearlyEqual(sincospair.second, float4(std::cos(0.0f), std::cos(0.5f), std::cos(1.0f), std::cos(1.5f)), 1e-5f));
  }
}

TEST_CASE("madronalib/dsp_math/fast_approx_functions", "[dsp_math]")
{
  SECTION("sinApprox")
  {
    REQUIRE(nearlyEqual(sinApprox(float4(0.0f, 0.5f, 1.0f, -0.5f)),
            float4(std::sin(0.0f), std::sin(0.5f), std::sin(1.0f), std::sin(-0.5f)), 1e-4f));
  }

  SECTION("cosApprox")
  {
    REQUIRE(nearlyEqual(cosApprox(float4(0.0f, 0.5f, 1.0f, -0.5f)),
            float4(std::cos(0.0f), std::cos(0.5f), std::cos(1.0f), std::cos(-0.5f)), 1e-4f));
  }

  SECTION("expApprox")
  {
    REQUIRE(nearlyEqual(expApprox(float4(0.0f, 1.0f, 2.0f, -1.0f)),
            float4(std::exp(0.0f), std::exp(1.0f), std::exp(2.0f), std::exp(-1.0f)), 0.15f));
  }

  SECTION("logApprox")
  {
    REQUIRE(nearlyEqual(logApprox(float4(1.0f, 2.0f, 4.0f, 8.0f)),
            float4(std::log(1.0f), std::log(2.0f), std::log(4.0f), std::log(8.0f)), 0.01f));
  }

  SECTION("tanhApprox")
  {
    REQUIRE(nearlyEqual(tanhApprox(float4(0.0f, 0.5f, 1.0f, -0.5f)),
            float4(std::tanh(0.0f), std::tanh(0.5f), std::tanh(1.0f), std::tanh(-0.5f)), 0.02f));
  }
}

// ================================================================
// Special value tests
// ================================================================

TEST_CASE("madronalib/dsp_math/special_values", "[dsp_math]")
{
  SECTION("setZero")    { REQUIRE(eq(setZero(), float4(0.0f, 0.0f, 0.0f, 0.0f))); }
  SECTION("setZeroInt") { REQUIRE(eq(setZeroInt(), int4(0, 0, 0, 0))); }
  SECTION("set1Float")  { REQUIRE(eq(set1Float(3.14f), float4(3.14f, 3.14f, 3.14f, 3.14f))); }
  SECTION("set1Int")    { REQUIRE(eq(set1Int(42), int4(42, 42, 42, 42))); }
  SECTION("setrFloat")  { REQUIRE(eq(setrFloat(1.0f, 2.0f, 3.0f, 4.0f), float4(1.0f, 2.0f, 3.0f, 4.0f))); }
  SECTION("setrInt")    { REQUIRE(eq(setrInt(1, 2, 3, 4), int4(1, 2, 3, 4))); }
}
