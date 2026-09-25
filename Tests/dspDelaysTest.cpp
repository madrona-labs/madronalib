// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// a unit test made using the Catch framework in catch.hpp / tests.cpp.

#include <cmath>

#include "catch.hpp"
#include "madronalib.h"
#include "MLDSPDelays.h"

using namespace ml;

namespace
{
// Push an impulse through a delay one block at a time and report the sample
// index at which it comes back out, or -1 if it does not within the window.
int measureDelay(IntegerDelay& d, int requestedDelay, int searchSamples)
{
  d.setDelayInSamples(requestedDelay);

  int sampleIndex = 0;
  for (int block = 0; block * kFramesPerBlock < searchSamples; ++block)
  {
    SignalBlock x{0.f};
    if (block == 0) x[0] = 1.0f;
    SignalBlock y = d(x);
    for (int i = 0; i < kFramesPerBlock; ++i)
    {
      if (y[i] > 0.5f) return sampleIndex + i;
    }
    sampleIndex += kFramesPerBlock;
  }
  return -1;
}
}  // namespace

// setMaxDelayInSamples(d) has to actually support a delay of d. Nothing pinned
// this before, and every reverb in the family sizes its buffers by hand.
TEST_CASE("madronalib/dsp/delays/max_delay_is_usable", "[delays]")
{
  for (int maxDelay : {64, 100, 512, 1000, 3000, 9000, 12000})
  {
    IntegerDelay d;
    d.setMaxDelayInSamples(static_cast<float>(maxDelay));
    INFO("max delay " << maxDelay);
    REQUIRE(measureDelay(d, maxDelay, maxDelay + 4 * kFramesPerBlock) == maxDelay);
  }
}


// The reverbs in this family request delays of coeff * sampleRate while sizing
// their buffers from constants, so the two disagree once the rate is high
// enough. This pins the sizing contract those call sites depend on: ask for the
// delay the highest supported rate needs, and the delay you get is the delay
// you asked for.
TEST_CASE("madronalib/dsp/delays/sized_for_rate", "[delays]")
{
  // the longest coefficient used by the Madrona reverbs, in seconds, at the
  // maximum size setting (size doubles it)
  constexpr float kLongestDelaySeconds = 0.111f * 2.f;

  for (float sr : {44100.f, 48000.f, 88200.f, 96000.f, 176400.f, 192000.f})
  {
    const int needed = static_cast<int>(kLongestDelaySeconds * sr);
    IntegerDelay d;
    d.setMaxDelayInSamples(static_cast<float>(needed));
    INFO("sample rate " << sr << " needs " << needed << " samples");
    REQUIRE(measureDelay(d, needed, needed + 4 * kFramesPerBlock) == needed);
  }
}
