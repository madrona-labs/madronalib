// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// a unit test made using the Catch framework in catch.hpp / tests.cpp.


#include "catch.hpp"
#include "MLTestUtils.h"
#include "MLDSPGens.h"
#include "MLDSPBank.h"


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


