// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// a unit test made using the Catch framework in catch.hpp / tests.cpp.

#include "catch.hpp"
#include "madronalib.h"

using namespace ml;

namespace
{
// pitch bend message: data byte 1 is the low 7 bits, data byte 2 the high 7 bits.
float bendValue(uint8_t loByte, uint8_t hiByte)
{
  Event e = MIDIMessageToEvent(MIDIMessage{0xE0, loByte, hiByte});
  REQUIRE(e.type == kPitchBend);
  return e.value1;
}
}  // namespace

TEST_CASE("madronalib/core/midi/pitch-bend", "[midi]")
{
  // EventsToSignals multiplies value1 by the bend range, so full scale is [-1, 1].
  REQUIRE(bendValue(0x00, 0x40) == 0.f);
  REQUIRE(bendValue(0x00, 0x00) == -1.f);
  REQUIRE(bendValue(0x7F, 0x7F) == Approx(8191.f / 8192));

  // both data bytes count
  REQUIRE(bendValue(0x01, 0x40) == Approx(1.f / 8192));
  REQUIRE(bendValue(0x7F, 0x3F) == Approx(-1.f / 8192));

  Event e = MIDIMessageToEvent(MIDIMessage{0xE3, 0x00, 0x60});
  REQUIRE(e.channel == 4);
  REQUIRE(e.value1 == 0.5f);
}
