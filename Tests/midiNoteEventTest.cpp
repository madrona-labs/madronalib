// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// a unit test made using the Catch framework in catch.hpp / tests.cpp.
// MIDI note messages must become Events in the layout EventsToSignals reads:
// value1 = note number, value2 = velocity. A note-on that lands velocity in the
// wrong field starts a voice with a zero gate, which is silent.

#include "catch.hpp"
#include "madronalib.h"

using namespace ml;

TEST_CASE("madronalib/core/midi/note-events", "[midi]")
{
  // note on, channel 5 (status 0x94), key 59, velocity 64
  Event on = MIDIMessageToEvent(MIDIMessage{0x94, 59, 64});
  REQUIRE(on.type == kNoteOn);
  REQUIRE(on.channel == 5);
  REQUIRE(on.sourceIdx == 59);
  REQUIRE(on.value1 == 59.f);
  REQUIRE(on.value2 == Approx(64.f / 127));

  // note off, same key, release velocity 32
  Event off = MIDIMessageToEvent(MIDIMessage{0x84, 59, 32});
  REQUIRE(off.type == kNoteOff);
  REQUIRE(off.channel == 5);
  REQUIRE(off.sourceIdx == 59);
  REQUIRE(off.value1 == 59.f);
  REQUIRE(off.value2 == Approx(32.f / 127));
}

TEST_CASE("madronalib/core/midi/note-on-starts-a-voice", "[midi]")
{
  EventsToSignals e2s;
  e2s.setSampleRate(48000);
  e2s.setPolyphony(1);

  // a converted note-on must open the gate and put the note number in the pitch row
  e2s.addEvent(MIDIMessageToEvent(MIDIMessage{0x94, 59, 64}));
  e2s.makeSignalBlock();
  const VoiceControls& vc = e2s.getVoiceControls();
  REQUIRE(gateHasTransition(vc.rows[kCtlGate][0]));
  REQUIRE(gateLevelAfter(vc.rows[kCtlGate][0], false));
  REQUIRE(vc.rows[kCtlVelocity][0] == Approx(64.f / 127));
  REQUIRE(vc.rows[kCtlPitch][0] == Approx(59.f));

  // and the note-off closes it
  e2s.addEvent(MIDIMessageToEvent(MIDIMessage{0x84, 59, 0}));
  e2s.makeSignalBlock();
  REQUIRE(gateHasTransition(vc.rows[kCtlGate][0]));
  REQUIRE_FALSE(gateLevelAfter(vc.rows[kCtlGate][0], true));
}
