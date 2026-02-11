// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// a unit test made using the Catch framework in catch.hpp / tests.cpp.


#include "catch.hpp"
#include "MLTestUtils.h"
#include "MLDSPGens.h"

using namespace ml;

TEST_CASE("madronalib/core/dsp_gens", "[dsp_gens]")
{
  PhasorGen p1;
  p1.clear();
  auto v0 = p1(1.f/kFramesPerBlock);
  
  
//  std::cout << "phasor: " << v0 << "\n";
  
  SineGen s1;
  s1.clear();
  auto v1 = s1(1.f/kFramesPerBlock);
  
  
//  std::cout << "sine: " << v1 << "\n";
  
  // one cycle of sine wave should end at 0
  float epsilon = dBToAmp(-120.f);
  REQUIRE(fabs(v1[kFramesPerBlock - 1]) < epsilon);
  
  // one shot
  OneShotGen g1;
  auto vg0 = g1(1.f/kFramesPerBlock);
  
  g1.trigger();
  auto vg1 = g1(1.f/kFramesPerBlock);
  auto vg2 = g1(1.f/kFramesPerBlock);

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
    
}



