// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// a unit test made using the Catch framework in catch.hpp / tests.cpp.

#include "catch.hpp"
#include "MLTestUtils.h"
#include "MLDSPGens.h"

#include <cmath>

using namespace ml;
using namespace testUtils;


// ================================================================
//  template tests
// ================================================================

TEST_CASE("madronalib/dsp/gens-template", "[template]")
{
  SECTION("template variations equality")
  {
    
    // we need to burn one buffer to allow coefficient interpolation to finish.
    // TODO make a more concise way to do this in a constructor
    SineGen<float> g1;
    SignalBlock dummy = g1(0.25f);
    
    // signal-rate params
    g1.clear();
    SignalBlock signalParams{0.25f};
    SignalBlock ref = g1(signalParams);
    
    // single Params
    g1.clear();
    SineGen<float>::Params sineGenParams;
    sineGenParams[SineGen<float>::freq] = 0.25f;
    REQUIRE(g1(sineGenParams) == ref);
    
    // single Params from std::array
    g1.clear();
    std::array<float, 1> arrayParams{0.25f};
    REQUIRE(g1(arrayParams) == ref);
    
    // float params
    g1.clear();
    float floatParam{0.25f};
    REQUIRE(g1(floatParam) == ref);
    
    // no params
    g1.clear();
    REQUIRE(g1() == ref);
    
  }
}


TEST_CASE("madronalib/dsp/gens-constructors", "[dsp_gens]")
{
  SECTION("SineGen")
  {
    float freq = 1.f / kFramesPerBlock;
    SineGen<float> g(freq);
    SineGen<float> ref;
    ref.coeffs = SineGen<float>::makeCoeffs({freq});
    REQUIRE(g() == ref());
  }

  SECTION("SawGen")
  {
    float freq = 1.f / kFramesPerBlock;
    SawGen<float> g(freq);
    SawGen<float> ref;
    ref.coeffs = SawGen<float>::makeCoeffs({freq});
    REQUIRE(g() == ref());
  }

  SECTION("PhasorGen")
  {
    float freq = 1.f / kFramesPerBlock;
    PhasorGen<float> g(freq);
    PhasorGen<float> ref;
    ref.coeffs = PhasorGen<float>::makeCoeffs({freq});
    REQUIRE(g() == ref());
  }

  SECTION("TickGen")
  {
    float freq = 1.f / 8.f;
    TickGen<float> g(freq);
    TickGen<float> ref;
    ref.coeffs = TickGen<float>::makeCoeffs({freq});
    REQUIRE(g() == ref());
  }

  SECTION("ImpulseGen")
  {
    float freq = 1.f / kFramesPerBlock;
    ImpulseGen<float> g(freq);
    ImpulseGen<float> ref;
    ref.coeffs = ImpulseGen<float>::makeCoeffs({freq});
    REQUIRE(g() == ref());
  }

  SECTION("PulseGen")
  {
    PulseGen<float> g(0.1f, 0.5f);
    PulseGen<float> ref;
    ref.coeffs = PulseGen<float>::makeCoeffs({0.1f, 0.5f});
    REQUIRE(g() == ref());
  }
}

TEST_CASE("madronalib/dsp/gens", "[dsp_gens]")
{
  SECTION("Counter")
  {
    // output starts at 0 and increments by 1 each sample
    Counter<float> c;
    c.clear();
    auto out = c();
    REQUIRE(out[0] == 0.f);
    REQUIRE(out[kFramesPerBlock - 1] == float(kFramesPerBlock - 1));

    // float and float4 versions produce equivalent output
    Counter<float> cf; cf.clear();
    Counter<float4> cg; cg.clear();
    REQUIRE(repeatRows<4>(cf()) == verticalToHorizontal(cg()));
  }

  SECTION("PhasorGen")
  {
    // at freq = 1/block, one complete ramp spans exactly the block
    // sample 0 = 1/64, sample 31 = 32/64 = 0.5 (last sample wraps back to 0)
    PhasorGen<float> p; p.clear();
    SignalBlockArray<1> params(1.f / kFramesPerBlock);
    auto out = p(params);
    REQUIRE(out[0] == Approx(1.f / kFramesPerBlock));
    REQUIRE(out[kFramesPerBlock / 2 - 1] == Approx(0.5f));
  }

  SECTION("NoiseGen")
  {
    // same seed produces identical output
    uint32_t testSeed{98118};
    NoiseGen<float> n1; n1.setSeed(testSeed);
    NoiseGen<float> n2; n2.setSeed(testSeed);
    REQUIRE(n1() == n2());

    // all samples in range [-1, 1)
    auto out = n1();
    for (size_t t = 0; t < kFramesPerBlock; ++t)
      REQUIRE((out[t] >= -1.f && out[t] < 1.f));
  }

  SECTION("ImpulseGen")
  {
    // at freq = 1/block, one impulse per block
    ImpulseGen<float> imp; imp.clear();
    SignalBlockArray<1> params(1.f / kFramesPerBlock);

    // warm up to get past startup transient
    imp(params);
    auto out = imp(params);

    // exactly one peak near 1.0, all other samples near 0
    int peaks = 0;
    for (size_t t = 0; t < kFramesPerBlock; ++t)
      if (out[t] > 0.5f) peaks++;
    REQUIRE(peaks == 1);

    float maxVal = 0.f;
    for (size_t t = 0; t < kFramesPerBlock; ++t)
      maxVal = std::max(maxVal, out[t]);
    REQUIRE(maxVal == Approx(1.f).margin(0.01f));
  }

  SECTION("SineGen")
  {
    // one complete cycle ends near 0
    SineGen<float> s; s.clear();
    SignalBlockArray<1> params(1.f / kFramesPerBlock);
    auto out = s(params);
    REQUIRE(std::abs(out[kFramesPerBlock - 1]) < dBToAmp(-40.f));

    // peak amplitude near 1
    float maxVal = 0.f;
    for (size_t t = 0; t < kFramesPerBlock; ++t)
      maxVal = std::max(maxVal, std::abs(out[t]));
    REQUIRE(maxVal == Approx(1.f).margin(0.05f));
  }

  SECTION("TestSineGen")
  {
    // one complete cycle ends very close to 0 (higher precision than SineGen)
    TestSineGen<float> s; s.clear();
    SignalBlockArray<1> params(1.f / kFramesPerBlock);
    auto out = s(params);
    REQUIRE(std::abs(out[kFramesPerBlock - 1]) < 1e-5f);

    // peak amplitude very close to 1
    float maxVal = 0.f;
    for (size_t t = 0; t < kFramesPerBlock; ++t)
      maxVal = std::max(maxVal, std::abs(out[t]));
    REQUIRE(maxVal == Approx(1.f).margin(0.001f));
  }

  SECTION("SawGen")
  {
    // zero mean over one complete cycle
    SawGen<float> saw; saw.clear();
    SignalBlockArray<1> params(1.f / kFramesPerBlock);
    auto out = saw(params);
    float sum = 0.f;
    for (size_t t = 0; t < kFramesPerBlock; ++t)
      sum += out[t];
    REQUIRE(std::abs(sum / kFramesPerBlock) < 0.05f);
  }

  SECTION("PulseGen")
  {
    // 50% width: zero mean
    PulseGen<float> pulse; pulse.clear();
    SignalBlockArray<2> params50;
    params50.setRow(0, SignalBlock(1.f / 16));   // freq: 4 cycles per block
    params50.setRow(1, SignalBlock(0.5f));        // width: 50%
    auto out50 = pulse(params50);
    float mean50 = 0.f;
    for (size_t t = 0; t < kFramesPerBlock; ++t)
      mean50 += out50[t];
    REQUIRE(std::abs(mean50 / kFramesPerBlock) < 0.05f);

    // 25% width: mean = 1 - 2*0.25 = +0.5
    pulse.clear();
    SignalBlockArray<2> params25;
    params25.setRow(0, SignalBlock(1.f / 16));
    params25.setRow(1, SignalBlock(0.25f));
    auto out25 = pulse(params25);
    float mean25 = 0.f;
    for (size_t t = 0; t < kFramesPerBlock; ++t)
      mean25 += out25[t];
    REQUIRE(mean25 / kFramesPerBlock == Approx(0.5f).margin(0.05f));
  }
}


// ================================================================
//  repeatableRandomValue
// ================================================================

TEST_CASE("madronalib/dsp/gens/repeatable_random", "[dsp_gens]")
{
  // the same inputs always give the same value
  REQUIRE(repeatableRandomValue(1234, 5) == repeatableRandomValue(1234, 5));
  REQUIRE(repeatableRandomValue(1234, 5, 6) == repeatableRandomValue(1234, 5, 6));

  // values are on [-1, 1), centered, and spread out
  constexpr int kValues{100000};
  for (uint64_t channel : {0ull, 1ull, 2ull, 0xFFFFFFFFFFFFFFFFull})
  {
    double sum{0}, sumSquares{0};
    float lo{1.f}, hi{-1.f};
    for (int i = 0; i < kValues; ++i)
    {
      float v = repeatableRandomValue(i, channel);
      sum += v;
      sumSquares += v * v;
      lo = std::min(lo, v);
      hi = std::max(hi, v);
    }
    REQUIRE(lo >= -1.f);
    REQUIRE(hi < 1.f);
    REQUIRE(lo < -0.999f);
    REQUIRE(hi > 0.999f);
    REQUIRE(fabs(sum / kValues) < 0.01);
    // the variance of a uniform value on [-1, 1) is 1/3
    REQUIRE(sumSquares / kValues == Approx(1. / 3.).epsilon(0.02));
  }

  // channels, seeds and neighboring indices are different sequences: they
  // agree no more often than chance, and are not correlated
  auto correlation = [&](uint64_t channelA, uint64_t seedA, uint64_t offsetA,
                         uint64_t channelB, uint64_t seedB, uint64_t offsetB)
  {
    double sum{0};
    for (int i = 0; i < kValues; ++i)
    {
      sum += repeatableRandomValue(i + offsetA, channelA, seedA) *
             repeatableRandomValue(i + offsetB, channelB, seedB);
    }
    return fabs(sum / kValues * 3.);
  };
  REQUIRE(correlation(0, 0, 0, 0, 0, 0) == Approx(1.).epsilon(0.02));
  REQUIRE(correlation(0, 0, 0, 1, 0, 0) < 0.02);
  REQUIRE(correlation(1, 0, 0, 2, 0, 0) < 0.02);
  REQUIRE(correlation(0, 0, 0, 0, 1, 0) < 0.02);
  REQUIRE(correlation(0, 0, 0, 0, 0, 1) < 0.02);
  // swapping index and channel is not the same value
  REQUIRE(correlation(2, 0, 3, 3, 0, 2) < 0.02);
  REQUIRE(repeatableRandomValue(2, 3) != repeatableRandomValue(3, 2));
  REQUIRE(repeatableRandomValue(1, 6) != repeatableRandomValue(2, 3));
}
