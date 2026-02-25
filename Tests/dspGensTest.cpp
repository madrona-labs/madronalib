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

  SECTION("TickGen")
  {
    // at freq = 1/8, expect exactly 8 ticks per block
    TickGen<float> ticker; ticker.clear();
    SignalBlockArray<1> params(1.f / 8);
    auto out = ticker(params);
    int ticks = 0;
    for (size_t t = 0; t < kFramesPerBlock; ++t)
      if (out[t] > 0.5f) ticks++;
    REQUIRE(ticks == 8);
  }

  SECTION("NoiseGen")
  {
    // same seed produces identical output
    NoiseGen<float> n1; n1.clear();
    NoiseGen<float> n2; n2.clear();
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
