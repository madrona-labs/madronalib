// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// a unit test made using the Catch framework in catch.hpp / tests.cpp.


#include "catch.hpp"
#include "MLTestUtils.h"
#include "MLDSPOps.h"
#include "MLDSPFunctional.h"
#include "MLDSPUtils.h"
#include "MLDSPRouting.h"
#include "MLDSPGens.h"

#include <iostream>
#include <iomanip> // Required for setprecision

using namespace ml;
using namespace testUtils;

TEST_CASE("madronalib/core/dsp_functional", "[dsp_functional]")
{

  SECTION("map")
  {
    constexpr int rows = 2;
    auto a{repeatRows<rows>(columnIndex())};

    // map void -> float
    auto b = map([&]() { return 4; }, a);

    // map float -> float
    auto c = map([&](float x) { return x * 2.f; }, a);

    // map int -> float
    auto d = map([&](int x) { return x * 2; }, a);

    // map SignalBlock -> SignalBlock
    auto e = map([&](SignalBlock x) { return x * 2.f; }, a);

    // map SignalBlock, int row -> SignalBlock
    auto f = map([&](SignalBlock x, int j) { return j * 2; }, a);

    REQUIRE(c == d);
    REQUIRE(d == e);
  }

}

