// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// a unit test made using the Catch framework in catch.hpp / tests.cpp.

#include "catch.hpp"
#include "MLTestUtils.h"
#include "MLDSPSample.h"
#include "MLDSPGens.h"
#include "MLDSPFilters.h"
#include "MLDSPMath.h"

using namespace ml;

TEST_CASE("madronalib/core/dsp_filters", "[dsp_filters]")
{
  float freq1 = 0.13f;
  SineGen s1gen;
  float freq2(0.013f);
  Lopass<float> filter1;
  
  for(int i=0; i<4; ++i)
  {
//    std::cout << filter1(s1gen(freq1), Lopass<float>::Params{freq2, 0.5f});
  }

}
