// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// a unit test made using the Catch framework in catch.hpp / tests.cpp.

#include "catch.hpp"
#include "MLTestUtils.h"
#include "MLDSPFilters.h"
#include "MLDSPSample.h"

using namespace ml;

TEST_CASE("madronalib/core/projections", "[projections]")
{
  {
    auto pa = projections::piecewiseLinear({3, 5, 8});
    REQUIRE(pa(0) == 3);
    REQUIRE(pa(0.5) == 5);
    REQUIRE(pa(1.0) == 8);
  }
  
  {
    auto pa = projections::piecewise(
                                     {1, 2, 3}, {projections::easeIn, projections::easeOut});
    
    // the first interval m of the easeIn should be symmetrical with the last interval m of the easeOut.
    float m{0.0625};
    float a = pa(0.f);
    float b = pa(m);
    float c = pa(1.f - m);
    float d = pa(1.f);
    REQUIRE(b - a == d - c);
  }
  
  {
    auto p0 = projections::bisquared;
    auto p1 = projections::invBisquared;
    
    for(int i=-5; i<5; ++i)
    {
      float x = i/5.f;
      REQUIRE(testUtils::nearlyEqual(p0(p1(x)), x));
    }
  }
}

