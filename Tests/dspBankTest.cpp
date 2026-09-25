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

TEST_CASE("madronalib/bank/lopass_bank", "[bank]")
{
  FilterBank<Lopass, 8> lopassBank;
  lopassBank.clear();

  using inputType  = FilterBank<Lopass, 8>::inputType;
  using outputType = FilterBank<Lopass, 8>::outputType;
  using Params     = FilterBank<Lopass, 8>::Params;
  constexpr int kProcs = FilterBank<Lopass, 8>::kNumFloat4Procs;

  // DC audio input: all voices = constant 1.0
  inputType dcInput{float4(1.0f)};

  // low cutoff params (omega=0.05, k=0.5) for all filters
  std::array<Params, kProcs> loParams;
  loParams.fill(Params{float4(0.05f), float4(0.5f)});

  SECTION("stored coefficients: DC passes at unity")
  {
    auto loCoeffs = FilterBank<Lopass, 8>::Processor::makeCoeffs(Params{float4(0.05f), float4(0.5f)});
    for (int p = 0; p < kProcs; ++p)
      lopassBank[p].coeffs = loCoeffs;

    outputType output;
    for (int i = 0; i < 30; ++i)
      output = lopassBank(dcInput);

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
    GenBank<SineGen, 8>::inputType sineFreqs{float4(0.25f)};

    auto output = lopassBank(sineBank(sineFreqs), loParams);

    auto hOutput = verticalToHorizontal<kProcs>(output);
    for (int v = 0; v < 8; ++v)
    {
      const float* row = hOutput.rowPtr(v);
      for (size_t t = 0; t < kFramesPerBlock; ++t)
        REQUIRE(std::abs(row[t]) < 0.05f);
    }
  }
}


