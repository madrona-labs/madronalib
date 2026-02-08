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

#define DO_TIME_TESTS 0

using namespace ml;
using namespace testUtils;

TEST_CASE("madronalib/core/dsp_ops", "[dsp_ops]")
{
  SignalBlock a(rangeClosed(-kPi, kPi));

  auto sinN = ([&]() {
    SignalBlock v;
    for (int i = 0; i < kFramesPerBlock; ++i)
    {
      v[i] = sinf(a[i]);
    }
    return v;
  });
  auto sinP = ([&]() { return sin(a); });
  auto sinA = ([&]() { return sinApprox(a); });
  std::vector<std::function<SignalBlock(void)> > sinFunctions{sinN, sinP, sinA};

  auto cosN = ([&]() {
    SignalBlock v;
    for (int i = 0; i < kFramesPerBlock; ++i)
    {
      v[i] = cosf(a[i]);
    }
    return v;
  });
  auto cosP = ([&]() { return cos(a); });
  auto cosA = ([&]() { return cosApprox(a); });
  std::vector<std::function<SignalBlock(void)> > cosFunctions{cosN, cosP, cosA};

  auto logN = ([&]() {
    SignalBlock v;
    for (int i = 0; i < kFramesPerBlock; ++i)
    {
      v[i] = logf(a[i]);
    }
    return v;
  });
  auto logP = ([&]() { return log(a); });
  auto logA = ([&]() { return logApprox(a); });
  std::vector<std::function<SignalBlock(void)> > logFunctions{logN, logP, logA};

  auto expN = ([&]() {
    SignalBlock v;
    for (int i = 0; i < kFramesPerBlock; ++i)
    {
      v[i] = expf(a[i]);
    }
    return v;
  });
  auto expP = ([&]() { return exp(a); });
  auto expA = ([&]() { return expApprox(a); });
  std::vector<std::function<SignalBlock(void)> > expFunctions{expN, expP, expA};

  std::vector<
      std::pair<std::string, std::vector<std::function<SignalBlock(void)> > > >
      functionVectors{{"sin", sinFunctions},
                      {"cos", cosFunctions},
                      {"log", logFunctions},
                      {"exp", expFunctions}};

  SECTION("precision")
  {
    // test precision of sin, cos, log, exp and approximations.
    // use native math as reference.
    // NOTE this does not measure the maximum error accurately! It uses equally-spaced
    // samples over the entire input range of the functions, just to provide a reference.
    // std::cout << "max differences from reference:\n";

    for (auto fnVec : functionVectors)
    {
      SignalBlock native = fnVec.second[0]();
      SignalBlock precise = fnVec.second[1]();
      SignalBlock approx = fnVec.second[2]();

      float nativeMaxDiff = max(abs(native - native));
      float preciseMaxDiff = max(abs(native - precise));
      float approxMaxDiff = max(abs(native - approx));

      
      // these differences are to accommodate the exp functions, the other ones
      // are a lot more precise.
      REQUIRE(preciseMaxDiff < 2e-6f);
      REQUIRE(approxMaxDiff < 2e-4f);
    }
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
  

  SECTION("convert")
  {
    constexpr float x{1.25f};
    SignalBlock a{x};
    SignalBlock b{-x};
    auto fa = fractionalPart(a);
    auto fb = fractionalPart(b);
    REQUIRE(fa[kFramesPerBlock - 1] == -fb[kFramesPerBlock - 1]);
  }
    
  SECTION("row operations")
  {
    SignalBlockArray<2> a{repeatRows<2>(columnIndex())};
    auto a2{a * 2.f};

    SignalBlock b{columnIndex()};
    auto b2 = b * 2.f;

    SignalBlockArray<2> x{3.f};
    SignalBlockArray<1> y{3.f};
    auto xy = x * repeatRows<2>(y);
    auto yx = repeatRows<2>(y) * x;

    auto e = a * repeatRows<2>(b);

    auto aa = repeatRows<4>(a);

    auto f{repeatRows<2>(columnIndex())};
    auto g = map([&](SignalBlock x, int j) { return x * (j + 1); }, f);

    auto h = stretchRows<6>(g);

    auto k = zeroPadRows<6>(columnIndex());

    auto m = rotateRows(k, -1) * 3.f;

    auto n = shiftRows(k, 2);
    // TODO actual tests
  }
  
  SECTION("combining")
  {
    SignalBlockArray<2> a{repeatRows<2>(columnIndex())};
    SignalBlockArray<2> b{rowIndex<2>() + 1};
    SignalBlockArray<2> c{1};

    auto sum = add(a, b, c);
    
    SignalBlockArray<3> gains = concatRows(SignalBlock{0.300f}, SignalBlock{0.030f}, SignalBlock{0.003f});

    auto mixResult = mix(gains, c, c, c);
    
    SignalBlockArray<6> gg = repeatRows<2>(gains);

    SignalBlockArray<2> h = separateRows<4, 6>(gg);
    
    // TODO tests
  }
  
  SECTION("multiplex")
  {
    SignalBlockArray<2> input{columnIndex<2>() + rowIndex<2>()};
    SignalBlockArray<2> a{7};
    SignalBlockArray<2> b{11};
    SignalBlockArray<2> c{13};
    SignalBlockArray<2> d{17};
    SignalBlockArray<2> e{19};

    // rangeOpen(0-1): equal amounts of a, b, c, d, e
    auto dv = multiplex(rangeOpen(0, 1), a, b, c, d, e);
    REQUIRE(dv[kFramesPerBlock - 1] == e[kFramesPerBlock - 1]);
    
    // rangeClosed(0, 4.f/5.f): last element should be e
    auto dw = multiplexLinear(rangeClosed(0, 4.f/5.f), a, b, c, d, e);
    REQUIRE(dv[kFramesPerBlock - 1] == e[kFramesPerBlock - 1]);

    // the sum of the linear demultiplexer's outputs should equal the input
    auto demuxInput2 = repeatRows<2>(SignalBlock{1});
    demultiplexLinear(rangeClosed(0, 3.f/4.f), demuxInput2, &a, &b, &c, &d);
    auto sumOfDemuxOutputs2 = add(a, b, c, d);
    REQUIRE(demuxInput2 == sumOfDemuxOutputs2);
    REQUIRE(sumOfDemuxOutputs2[kFramesPerBlock - 1] == 1);
    
    // demultiplex to multiple outputs, then multiplex wtih same selector
    // should equal the input
    auto selectorSignal = rangeOpen(0, 1);
    auto demuxInput3 = columnIndex<2>();
    demultiplex(selectorSignal, demuxInput3, &a, &b, &c, &d);
    auto demuxThenMux = multiplex(selectorSignal, a, b, c, d);
    REQUIRE(demuxInput3 == demuxThenMux);
  }
  
}

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

