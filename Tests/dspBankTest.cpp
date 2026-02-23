// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// a unit test made using the Catch framework in catch.hpp / tests.cpp.


#include "catch.hpp"
#include "MLTestUtils.h"
#include "MLDSPGens.h"
#include "MLDSPFilters.h"
#include "MLDSPBank.h"

#include <cmath>


using namespace ml;
using namespace testUtils;

TEST_CASE("madronalib/bank/tickgen_bank", "[bank]")
{
  GenBank<TickGen, 8> bank;
  bank.clear();

  using inputType = GenBank<TickGen, 8>::inputType;
  using outputType = GenBank<TickGen, 8>::outputType;

  // 8 voices with frequencies that divide evenly into 64 samples:
  // voices 0-3: tick every 8, 16, 32, 64 samples
  // voices 4-7: tick every 8, 16, 32, 64 samples (same pattern, second group)
  inputType freqInput;
  for (size_t t = 0; t < kFramesPerBlock; ++t)
  {
    freqInput.rowPtr(0)[t] = float4(1.f/8, 1.f/16, 1.f/32, 1.f/64);
    freqInput.rowPtr(1)[t] = float4(1.f/8, 1.f/16, 1.f/32, 1.f/64);
  }
  
  
  // run two blocks to get past any startup transient
  bank(freqInput);
  outputType output = bank(freqInput);
  
  // convert to horizontal for easy per-voice inspection
  auto hOutput = verticalToHorizontal<2>(output);
  
  // count ticks per voice (tick = 1.0)
  auto countTicks = [&](int voice) {
    int count = 0;
    const float* row = hOutput.rowPtr(voice);
    for (size_t t = 0; t < kFramesPerBlock; ++t)
    {
      if (row[t] > 0.5f) count++;
    }
    return count;
  };
  
  // voices 0 and 4: freq=1/8, expect 8 ticks per block
  REQUIRE(countTicks(0) == 8);
  REQUIRE(countTicks(4) == 8);
  
  // voices 1 and 5: freq=1/16, expect 4 ticks
  REQUIRE(countTicks(1) == 4);
  REQUIRE(countTicks(5) == 4);
  
  // voices 2 and 6: freq=1/32, expect 2 ticks
  REQUIRE(countTicks(2) == 2);
  REQUIRE(countTicks(6) == 2);
  
  // voices 3 and 7: freq=1/64, expect 1 tick
  REQUIRE(countTicks(3) == 1);
  REQUIRE(countTicks(7) == 1);
  
  // verify matching outputs between the two groups
  // (same frequency → same tick pattern)
  for (int v = 0; v < 4; ++v)
  {
    const float* rowA = hOutput.rowPtr(v);
    const float* rowB = hOutput.rowPtr(v + 4);
    for (size_t t = 0; t < kFramesPerBlock; ++t)
    {
      REQUIRE(rowA[t] == rowB[t]);
    }
  }

}


TEST_CASE("madronalib/bank/lopass_bank", "[bank]")
{
  FilterBank<Lopass, 8> bank;
  bank.clear();

  using inputType  = FilterBank<Lopass, 8>::inputType;
  using outputType = FilterBank<Lopass, 8>::outputType;
  using Params     = FilterBank<Lopass, 8>::Params;
  constexpr int kProcs = FilterBank<Lopass, 8>::kNumFloat4Procs;

  // DC audio input: all voices = constant 1.0
  inputType dcInput{float4(1.0f)};

  // low cutoff params (omega=0.05, k=0.5) for all processors
  std::array<Params, kProcs> loParams;
  loParams.fill(Params{float4(0.05f), float4(0.5f)});

  SECTION("stored coefficients: DC passes at unity")
  {
    auto loCoeffs = FilterBank<Lopass, 8>::Processor::makeCoeffs(Params{float4(0.05f), float4(0.5f)});
    for (int p = 0; p < kProcs; ++p)
      bank[p].coeffs = loCoeffs;

    outputType output;
    for (int i = 0; i < 30; ++i)
      output = bank(dcInput);

    auto hOutput = verticalToHorizontal<kProcs>(output);
    for (int v = 0; v < 8; ++v)
    {
      const float* row = hOutput.rowPtr(v);
      for (size_t t = 0; t < kFramesPerBlock; ++t)
        REQUIRE(std::abs(row[t] - 1.0f) < 0.01f);
    }
  }

  SECTION("per-block params: near-Nyquist sine is attenuated")
  {
    // GenBank<SineGen, 8>::outputType == FilterBank<Lopass, 8>::inputType,
    // so the sine output feeds directly into the lopass input.
    GenBank<SineGen, 8> sineBank;
    sineBank.clear();

    // all 8 voices at near-Nyquist frequency
    GenBank<SineGen, 8>::inputType sineFreqs{float4(0.49f)};

    outputType output;
    for (int i = 0; i < 10; ++i)
      output = bank(sineBank(sineFreqs), loParams);

    auto hOutput = verticalToHorizontal<kProcs>(output);
    for (int v = 0; v < 8; ++v)
    {
      const float* row = hOutput.rowPtr(v);
      for (size_t t = 0; t < kFramesPerBlock; ++t)
        REQUIRE(std::abs(row[t]) < 0.05f);
    }
  }
}


