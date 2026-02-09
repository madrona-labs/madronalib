// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#include "catch.hpp"
#include "MLTestUtils.h"
#include "MLDSPOps.h"
#include "MLDSPFunctional.h"
#include "MLDSPUtils.h"
#include "MLDSPRouting.h"
#include "MLDSPGens.h"

#include <iostream>
#include <iomanip>

#define DO_TIME_TESTS 0

using namespace ml;
using namespace testUtils;

// Helper functions for comparisons
namespace {

// Equality check for SignalBlock
bool eq(const SignalBlock& a, const SignalBlock& b) {
  for (size_t i = 0; i < kFramesPerBlock; ++i) {
    if (a[i] != b[i]) return false;
  }
  return true;
}

// Equality check for float4
bool eq(float4 a, float4 b) {
  for (int i = 0; i < 4; ++i) {
    if (getFloat4Lane(a, i) != getFloat4Lane(b, i)) return false;
  }
  return true;
}

// Nearly equal check for float4
bool nearlyEqual(float4 a, float4 b, float eps = 1e-6f) {
  for (int i = 0; i < 4; ++i) {
    if (std::fabs(getFloat4Lane(a, i) - getFloat4Lane(b, i)) >= eps) return false;
  }
  return true;
}

// Nearly equal check for SignalBlock
bool nearlyEqual(const SignalBlock& a, const SignalBlock& b, float eps = 1e-6f) {
  for (size_t i = 0; i < kFramesPerBlock; ++i) {
    if (std::fabs(a[i] - b[i]) >= eps) return false;
  }
  return true;
}

// Check if all elements equal a constant
bool allEqual(const SignalBlock& a, float val, float eps = 1e-6f) {
  for (size_t i = 0; i < kFramesPerBlock; ++i) {
    if (std::fabs(a[i] - val) >= eps) return false;
  }
  return true;
}

} // anonymous namespace


TEST_CASE("madronalib/core/dsp_ops", "[dsp_ops]")
{
  SECTION("construction")
  {
    SignalBlock a(3.0f);
    REQUIRE(allEqual(a, 3.0f));
    
    SignalBlock b(rangeClosed(0.0f, 1.0f));
    REQUIRE(b[0] == Approx(0.0f));
    REQUIRE(b[kFramesPerBlock - 1] == Approx(1.0f));
    
    SignalBlockInt c(42);
    REQUIRE(c[0] == 42);
    REQUIRE(c[kFramesPerBlock - 1] == 42);
  }
  
  SECTION("binary arithmetic")
  {
    SignalBlock a(5.0f);
    SignalBlock b(2.0f);
    
    REQUIRE(allEqual(add(a, b), 7.0f));
    REQUIRE(allEqual(subtract(a, b), 3.0f));
    REQUIRE(allEqual(multiply(a, b), 10.0f));
    REQUIRE(allEqual(divide(a, b), 2.5f));
    
    // Test operators
    REQUIRE(allEqual(a + b, 7.0f));
    REQUIRE(allEqual(a - b, 3.0f));
    REQUIRE(allEqual(a * b, 10.0f));
    REQUIRE(allEqual(a / b, 2.5f));
  }
  
  SECTION("compound assignment")
  {
    SignalBlock a(5.0f);
    SignalBlock b(2.0f);
    
    SignalBlock c = a;
    c += b;
    REQUIRE(allEqual(c, 7.0f));
    
    c = a;
    c -= b;
    REQUIRE(allEqual(c, 3.0f));
    
    c = a;
    c *= b;
    REQUIRE(allEqual(c, 10.0f));
    
    c = a;
    c /= b;
    REQUIRE(allEqual(c, 2.5f));
  }
  
  SECTION("unary operations")
  {
    SignalBlock a(4.0f);
    REQUIRE(allEqual(sqrt(a), 2.0f));
    REQUIRE(nearlyEqual(sqrtApprox(a), SignalBlock(2.0f), 1e-3f));
    
    SignalBlock b(-3.5f);
    REQUIRE(allEqual(abs(b), 3.5f));
    REQUIRE(allEqual(sign(b), -1.0f));
    REQUIRE(allEqual(signBit(b), -1.0f));
    
    SignalBlock c(0.0f);
    REQUIRE(allEqual(sign(c), 0.0f));
  }
  
  SECTION("min/max")
  {
    SignalBlock a = rangeClosed(-1.0f, 1.0f);
    SignalBlock b(0.0f);
    
    auto minResult = min(a, b);
    REQUIRE(minResult[0] == Approx(-1.0f));
    REQUIRE(minResult[kFramesPerBlock - 1] == Approx(0.0f));
    
    auto maxResult = max(a, b);
    REQUIRE(maxResult[0] == Approx(0.0f));
    REQUIRE(maxResult[kFramesPerBlock - 1] == Approx(1.0f));
  }
  
  SECTION("ternary operations")
  {
    SignalBlock a(1.0f);
    SignalBlock b(5.0f);
    SignalBlock mix(0.5f);
    
    auto result = lerp(a, b, mix);
    REQUIRE(allEqual(result, 3.0f));
    
    SignalBlock x(2.5f);
    SignalBlock minBound(1.0f);
    SignalBlock maxBound(5.0f);
    
    auto clamped = clamp(x, minBound, maxBound);
    REQUIRE(allEqual(clamped, 2.5f));
    
    auto clampedLow = clamp(SignalBlock(0.5f), minBound, maxBound);
    REQUIRE(allEqual(clampedLow, 1.0f));
    
    auto clampedHigh = clamp(SignalBlock(6.0f), minBound, maxBound);
    REQUIRE(allEqual(clampedHigh, 5.0f));
  }
  
  SECTION("trig functions precision")
  {
    SignalBlock a(rangeClosed(-kPi, kPi));
    
    auto sinN = ([&]() {
      SignalBlock v;
      for (int i = 0; i < kFramesPerBlock; ++i) {
        v[i] = sinf(a[i]);
      }
      return v;
    });
    auto sinP = ([&]() { return sin(a); });
    auto sinA = ([&]() { return sinApprox(a); });
    
    SignalBlock native = sinN();
    SignalBlock precise = sinP();
    SignalBlock approx = sinA();
    
    float preciseMaxDiff = max(abs(native - precise));
    float approxMaxDiff = max(abs(native - approx));
    
    REQUIRE(preciseMaxDiff < 2e-6f);
    REQUIRE(approxMaxDiff < 2e-4f);
  }
  
  SECTION("log/exp functions precision")
  {
    SignalBlock a(rangeClosed(0.1f, 10.0f));
    
    auto logN = ([&]() {
      SignalBlock v;
      for (int i = 0; i < kFramesPerBlock; ++i) {
        v[i] = logf(a[i]);
      }
      return v;
    });
    auto logP = ([&]() { return log(a); });
    auto logA = ([&]() { return logApprox(a); });
    
    SignalBlock native = logN();
    SignalBlock precise = logP();
    SignalBlock approx = logA();
    
    float preciseMaxDiff = max(abs(native - precise));
    float approxMaxDiff = max(abs(native - approx));
    
    REQUIRE(preciseMaxDiff < 2e-6f);
    REQUIRE(approxMaxDiff < 2e-4f);
  }
  
  SECTION("integer operations")
  {
    SignalBlockInt a(10);
    SignalBlockInt b(3);
    
    auto sum = addInt32(a, b);
    REQUIRE(sum[0] == 13);
    
    auto diff = subtractInt32(a, b);
    REQUIRE(diff[0] == 7);
  }
  
  SECTION("conversions")
  {
    constexpr float x{1.25f};
    SignalBlock a{x};
    SignalBlock b{-x};
    
    auto fa = fractionalPart(a);
    auto fb = fractionalPart(b);
    REQUIRE(fa[kFramesPerBlock - 1] == Approx(0.25f));
    REQUIRE(fb[kFramesPerBlock - 1] == Approx(-0.25f));
    
    SignalBlock c(3.7f);
    auto rounded = roundFloatToInt(c);
    auto truncated = truncateFloatToInt(c);
    
    auto backToFloat = intToFloat(rounded);
    REQUIRE(backToFloat[0] == Approx(4.0f));
  }
  
  SECTION("comparisons")
  {
    SignalBlock a = rangeClosed(0.0f, 10.0f);
    SignalBlock b(5.0f);
    
    auto gt = greaterThan(a, b);
    auto lt = lessThan(a, b);
    auto eq = equal(a, b);
    
    // Or better - actually test the logic:
    // First element (0.0f) should NOT be greater than 5.0f
    REQUIRE(gt[0] == 0);
    // Last element (10.0f) SHOULD be greater than 5.0f
    REQUIRE(gt[kFramesPerBlock - 1] != 0);
    
    // First element (0.0f) SHOULD be less than 5.0f
    REQUIRE(lt[0] != 0);
    // Last element (10.0f) should NOT be less than 5.0f
    REQUIRE(lt[kFramesPerBlock - 1] == 0);
  }
  
  SECTION("horizontal operations")
  {
    SignalBlock a = rangeClosed(0.0f, 63.0f);
    
    float s = sum(a);
    REQUIRE(s == Approx(63.0f * 64.0f / 2.0f)); // Sum of 0..63
    
    float m = mean(a);
    REQUIRE(m == Approx(31.5f));
    
    float minVal = min(a);
    REQUIRE(minVal == Approx(0.0f));
    
    float maxVal = max(a);
    REQUIRE(maxVal == Approx(63.0f));
  }
  
  SECTION("map operations")
  {
    auto squareFn = [](float x) { return x * x; };
    SignalBlock a = rangeClosed(0.0f, 1.0f);
    auto squared = map(squareFn, a);
    
    REQUIRE(squared[0] == Approx(0.0f));
    REQUIRE(squared[kFramesPerBlock - 1] == Approx(1.0f));
    
    auto doubleFn = [](int x) { return static_cast<float>(x * 2); };
    SignalBlockInt b = columnIndexInt();
    SignalBlock doubled = map(doubleFn, b);
    
    REQUIRE(doubled[0] == Approx(0.0f));
    REQUIRE(doubled[1] == Approx(2.0f));
    REQUIRE(doubled[10] == Approx(20.0f));
  }
  
  SECTION("row operations - repeatRows")
  {
    SignalBlock b = columnIndex();
    auto repeated = repeatRows<4>(b);
    
    REQUIRE(nearlyEqual(repeated.getRow(0), b));
    REQUIRE(nearlyEqual(repeated.getRow(1), b));
    REQUIRE(nearlyEqual(repeated.getRow(2), b));
    REQUIRE(nearlyEqual(repeated.getRow(3), b));
  }
  
  SECTION("row operations - stretchRows")
  {
    SignalBlockArray<2> a;
    a.setRow(0, SignalBlock(1.0f));
    a.setRow(1, SignalBlock(5.0f));
    
    auto stretched = stretchRows<5>(a);
    
    REQUIRE(allEqual(stretched.getRow(0), 1.0f));
    REQUIRE(allEqual(stretched.getRow(4), 5.0f));
  }
  
  SECTION("row operations - zeroPadRows")
  {
    SignalBlock b(42.0f);
    auto padded = zeroPadRows<4>(b);
    
    REQUIRE(allEqual(padded.getRow(0), 42.0f));
    REQUIRE(allEqual(padded.getRow(1), 0.0f));
    REQUIRE(allEqual(padded.getRow(2), 0.0f));
    REQUIRE(allEqual(padded.getRow(3), 0.0f));
  }
  
  SECTION("row operations - shiftRows")
  {
    SignalBlockArray<4> a;
    for (size_t i = 0; i < 4; ++i) {
      a.setRow(i, SignalBlock(static_cast<float>(i)));
    }
    
    auto shifted = shiftRows(a, 1);
    REQUIRE(allEqual(shifted.getRow(0), 0.0f)); // Shifted in zero
    REQUIRE(allEqual(shifted.getRow(1), 0.0f));
    REQUIRE(allEqual(shifted.getRow(2), 1.0f));
    REQUIRE(allEqual(shifted.getRow(3), 2.0f));
  }
  
  SECTION("row operations - rotateRows")
  {
    SignalBlockArray<4> a;
    for (size_t i = 0; i < 4; ++i) {
      a.setRow(i, SignalBlock(static_cast<float>(i)));
    }
    
    auto rotated = rotateRows(a, 1);
    REQUIRE(allEqual(rotated.getRow(0), 3.0f)); // Wrapped around
    REQUIRE(allEqual(rotated.getRow(1), 0.0f));
    REQUIRE(allEqual(rotated.getRow(2), 1.0f));
    REQUIRE(allEqual(rotated.getRow(3), 2.0f));
  }
  
  SECTION("row operations - concatRows")
  {
    SignalBlock a(1.0f);
    SignalBlock b(2.0f);
    SignalBlockArray<2> c;
    c.setRow(0, SignalBlock(3.0f));
    c.setRow(1, SignalBlock(4.0f));
    
    auto concat = concatRows(a, b, c);
    
    REQUIRE(allEqual(concat.getRow(0), 1.0f));
    REQUIRE(allEqual(concat.getRow(1), 2.0f));
    REQUIRE(allEqual(concat.getRow(2), 3.0f));
    REQUIRE(allEqual(concat.getRow(3), 4.0f));
  }
  
  SECTION("row operations - separateRows")
  {
    SignalBlockArray<6> a;
    for (size_t i = 0; i < 6; ++i) {
      a.setRow(i, SignalBlock(static_cast<float>(i)));
    }
    
    auto even = evenRows(a);
    REQUIRE(allEqual(even.getRow(0), 0.0f));
    REQUIRE(allEqual(even.getRow(1), 2.0f));
    REQUIRE(allEqual(even.getRow(2), 4.0f));
    
    auto odd = oddRows(a);
    REQUIRE(allEqual(odd.getRow(0), 1.0f));
    REQUIRE(allEqual(odd.getRow(1), 3.0f));
    REQUIRE(allEqual(odd.getRow(2), 5.0f));
    
    auto middle = separateRows<2, 4>(a);
    REQUIRE(allEqual(middle.getRow(0), 2.0f));
    REQUIRE(allEqual(middle.getRow(1), 3.0f));
  }
  
  SECTION("row operations - addRows")
  {
    SignalBlockArray<3> a;
    a.setRow(0, SignalBlock(1.0f));
    a.setRow(1, SignalBlock(2.0f));
    a.setRow(2, SignalBlock(3.0f));
    
    auto sum = addRows(a);
    REQUIRE(allEqual(sum, 6.0f));
  }
  
  SECTION("row operations - RowView")
  {
    SignalBlockArray<2> a;
    a.setRow(0, SignalBlock(1.0f));
    a.setRow(1, SignalBlock(2.0f));
    
    // Test compound assignment
    a.row(0) += SignalBlock(5.0f);
    REQUIRE(allEqual(a.getRow(0), 6.0f));
    
    a.row(1) *= SignalBlock(3.0f);
    REQUIRE(allEqual(a.getRow(1), 6.0f));
    
    // Test element access
    a.row(0)[0] = 42.0f;
    REQUIRE(a.getRow(0)[0] == 42.0f);
  }
  
  SECTION("load and store")
  {
    alignas(16) float data[kFramesPerBlock * 2];
    for (size_t i = 0; i < kFramesPerBlock * 2; ++i) {
      data[i] = static_cast<float>(i);
    }
    
    SignalBlockArray<2> a;
    load(a, data);
    
    REQUIRE(a[0] == 0.0f);
    REQUIRE(a[kFramesPerBlock] == static_cast<float>(kFramesPerBlock));
    
    alignas(16) float result[kFramesPerBlock * 2];
    store(a, result);
    
    for (size_t i = 0; i < kFramesPerBlock * 2; ++i) {
      REQUIRE(result[i] == static_cast<float>(i));
    }
  }
  
  SECTION("SignalBlock4Array transpose")
  {
    SignalBlock4Array<2> block;
    
    // Fill with sequential pattern
    for (size_t row = 0; row < 2; ++row) {
      for (size_t i = 0; i < kFramesPerBlock; ++i) {
        float base = static_cast<float>(row * kFramesPerBlock * 4 + i * 4);
        block.rowPtr(row)[i] = float4(base + 0, base + 1, base + 2, base + 3);
      }
    }
    
    // Save original
    SignalBlock4Array<2> original = block;
    
    // Transpose all rows
    block.transposeRows();
    
    // Verify it changed (check a few values to ensure transpose happened)
    bool changed = false;
    for (size_t i = 0; i < kFramesPerBlock && !changed; ++i) {
      if (!eq(block.rowPtr(0)[i], original.rowPtr(0)[i])) {
        changed = true;
      }
    }
    REQUIRE(changed);
    
    // Transpose again - should get back original
    block.transposeRows();
    
    // Verify we got back the original
    for (size_t row = 0; row < 2; ++row) {
      for (size_t i = 0; i < kFramesPerBlock; ++i) {
        REQUIRE(eq(block.rowPtr(row)[i], original.rowPtr(row)[i]));
      }
    }
    
    // Test single row transpose
    SignalBlock4 singleRow;
    for (size_t i = 0; i < kFramesPerBlock; ++i) {
      float base = static_cast<float>(i * 4);
      singleRow[i] = float4(base + 0, base + 1, base + 2, base + 3);
    }
    
    SignalBlock4 originalSingle = singleRow;
    singleRow.transposeRow(0);
    
    // Verify change
    bool singleChanged = false;
    for (size_t i = 0; i < kFramesPerBlock && !singleChanged; ++i) {
      if (!eq(singleRow[i], originalSingle[i])) {
        singleChanged = true;
      }
    }
    REQUIRE(singleChanged);
    
    // Transpose back
    singleRow.transposeRow(0);
    for (size_t i = 0; i < kFramesPerBlock; ++i) {
      REQUIRE(eq(singleRow[i], originalSingle[i]));
    }
  }
  
  SECTION("validate")
  {
    SignalBlock good(1.0f);
    REQUIRE(validate(good));
    
    SignalBlock bad;
    bad[0] = std::numeric_limits<float>::quiet_NaN();
    REQUIRE(!validate(bad));
  }
  
  
#if DO_TIME_TESTS
  SECTION("time")
  {
    // test speed of precise functions relative to native ones.
    // test speed of approximate functions relative to precise ones.
    // if we are optimizing the compiled code, approximate ones should be faster.
    // in debug builds, not necessarily.
    // std::cout << "nanoseconds per iteration:\n";
    
    int i = 0;
    for (auto fnVec : functionVectors)
    {
      
      // temporarily we have a separate timing function for Apple Silicon, which
      // tries to runs the test on a performance core.
      
#if (defined __ARM_NEON) || (defined __ARM_NEON__)
      TimedResult<SignalBlock> fnTimeNative =
      timeIterationsInThread<SignalBlock>(fnVec.second[0]);
      TimedResult<SignalBlock> fnTimePrecise =
      timeIterationsInThread<SignalBlock>(fnVec.second[1]);
      TimedResult<SignalBlock> fnTimeApprox =
      timeIterationsInThread<SignalBlock>(fnVec.second[2]);
#else
      TimedResult<SignalBlock> fnTimeNative =
      timeIterations<SignalBlock>(fnVec.second[0]);
      TimedResult<SignalBlock> fnTimePrecise =
      timeIterations<SignalBlock>(fnVec.second[1]);
      TimedResult<SignalBlock> fnTimeApprox =
      timeIterations<SignalBlock>(fnVec.second[2]);
#endif
      
      
      std::cout << fnVec.first << " native: " << fnTimeNative.ns
      << ", precise: " << fnTimePrecise.ns
      << ", approx: " << fnTimeApprox.ns << " \n";
      i++;
    }
  }
#endif
  
}


