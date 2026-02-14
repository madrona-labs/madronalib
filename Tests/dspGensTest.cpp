// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// a unit test made using the Catch framework in catch.hpp / tests.cpp.


#include "catch.hpp"
#include "MLTestUtils.h"
#include "MLDSPGens.h"

#define DO_TIME_TESTS 0

using namespace ml;
using namespace testUtils;

TEST_CASE("madronalib/core/dsp_gens", "[dsp_gens]")
{
  PhasorGen<float> p1;
  p1.clear();
  auto v0 = p1(1.f/kFramesPerBlock);
  
  
//  std::cout << "phasor: " << v0 << "\n";
  
  SineGen<float> s1;
  s1.clear();
  auto v1 = s1(1.f/kFramesPerBlock);
  
  
//  std::cout << "sine: " << v1 << "\n";
  
  // one cycle of sine wave should end at 0
  float epsilon = dBToAmp(-120.f);
  REQUIRE(fabs(v1[kFramesPerBlock - 1]) < epsilon);
  
  // validate element order and horizontal <-> vertical transforms
  Counter<float> cf;
  SignalBlock cfOut = cf();
    
  auto cfh = repeatRows<4>(cfOut);
  
  Counter<float4> cg;
  SignalBlock4 cgOut = cg();
  auto cgh = verticalToHorizontal(cgOut);
  REQUIRE(cfh == cgh);
    
  // ticks
  TickGen<float4> ticker;
  Block<float4> freqs(float4(0.1, 0.2, 0.333, 0.50));
  auto t1 = ticker(freqs);
  auto th = verticalToHorizontal(t1);
  // neat! std::cout << th << "\n";
    
  
  // noise
  NoiseGen<float4> noisy;
  noisy.clear();

  // impulses
  const SignalBlock impFreq(1.f/20.f);
  const SignalBlock4 impFreq4(1.f/20.f);
  ImpulseGen<float> imp;
  ImpulseGen<float4> imp4;
  imp.clear();
  imp4.clear();

  
  
#if DO_TIME_TESTS

  
  SECTION("time")
  {
    // test speed of precise functions relative to native ones.
    // test speed of approximate functions relative to precise ones.
    // if we are optimizing the compiled code, approximate ones should be faster.
    // in debug builds, not necessarily.
    // std::cout << "nanoseconds per iteration:\n";
    
    const SignalBlock impFreq(1.f/20.f);
    const SignalBlock4 impFreq4(1.f/20.f);
    ImpulseGen<float> imp;
    ImpulseGen<float4> imp4;
    imp.clear();
    imp4.clear();

    auto impTest = ([&]() {
      SignalBlock r;
      for(int i=0; i<4; ++i) {
        r += imp(impFreq);
      };
      return r;
    });
    auto imp4Test = ([&]() {
      return imp4(impFreq4);
    });

      
      // temporarily we have a separate timing function for Apple Silicon, which
      // tries to runs the test on a performance core.
      
#if (defined __ARM_NEON) || (defined __ARM_NEON__)
      TimedResult<SignalBlock> impulsesFloatTime =
      timeIterationsInThread<SignalBlock>(impTest);
      TimedResult<SignalBlock4> impulsesFloat4Time =
      timeIterationsInThread<SignalBlock4>(imp4Test);

#else
      TimedResult<SignalBlock> impulsesFloatTime =
      timeIterations<SignalBlock>(impTest);
      TimedResult<SignalBlock4> impulsesFloat4Time =
      timeIterations<SignalBlock4>(imp4Test);

#endif
      
    std::cout << "impulses float: " << impulsesFloatTime.ns << "\n";
    std::cout << "impulses float4: " << impulsesFloat4Time.ns << "\n";
    }
  
#endif
  
}



