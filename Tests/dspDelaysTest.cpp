// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// a unit test made using the Catch framework in catch.hpp / tests.cpp.

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

// A delay longer than the buffer used to wrap round to a near-zero delay, so a
// size control built on it stopped being monotonic: turn it up and the delay
// suddenly collapses. Saturating keeps it monotonic and sounds like what it is.
TEST_CASE("madronalib/dsp/delays/overlong_delay_saturates", "[delays]")
{
  constexpr int kMax = 1000;

  SECTION("via setDelayInSamples")
  {
    // setMaxDelayInSamples rounds up to a power of two, so the buffer is
    // pow2_ceil(1000 + 64) = 2048 and the longest representable delay is 2047.
    constexpr int kLongestRepresentable = 2047;

    for (int requested : {kMax * 4, kMax * 40, kMax * 400})
    {
      IntegerDelay d;
      d.setMaxDelayInSamples(static_cast<float>(kMax));
      int measured = measureDelay(d, requested, kLongestRepresentable + 4 * kFramesPerBlock);
      INFO("requested " << requested << ", measured " << measured);
      // Saturate, do not wrap. Wrapping gives requested % 2048, which for these
      // inputs is 1952, 1088 and 640 -- all plausible-looking delays that the
      // caller never asked for, and all smaller as the request gets larger.
      REQUIRE(measured == kLongestRepresentable);
    }
  }

  SECTION("via the modulated operator")
  {
    IntegerDelay d;
    d.setMaxDelayInSamples(static_cast<float>(kMax));
    SignalBlock x{0.f};
    x[0] = 1.0f;
    SignalBlock hugeDelay{static_cast<float>(kMax * 40)};
    // only requirement here is that it stays finite and in bounds
    SignalBlock y = d(x, hugeDelay);
    for (int i = 0; i < kFramesPerBlock; ++i) REQUIRE(std::isfinite(y[i]));
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
