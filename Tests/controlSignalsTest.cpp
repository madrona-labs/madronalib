// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// a unit test made using the Catch framework in catch.hpp / tests.cpp.
// Tests for control-rate signals: gate transition codes and EventsToSignals' VoiceControls.

#include <cmath>
#include <type_traits>

#include "catch.hpp"
#include "madronalib.h"

using namespace ml;

namespace
{
Event noteOn(int key, float pitch, float velocity, int time = 0)
{
  Event e;
  e.type = kNoteOn;
  e.channel = 1;
  e.sourceIdx = key;
  e.time = time;
  e.value1 = pitch;
  e.value2 = velocity;
  return e;
}

Event noteOff(int key, int time = 0)
{
  Event e;
  e.type = kNoteOff;
  e.channel = 1;
  e.sourceIdx = key;
  e.time = time;
  return e;
}

Event pitchBend(float amount)
{
  Event e;
  e.type = kPitchBend;
  e.channel = 1;
  e.value1 = amount;
  return e;
}

Event controller(int number, float value, int time = 0)
{
  Event e;
  e.type = kController;
  e.channel = 1;
  e.sourceIdx = number;
  e.time = time;
  e.value1 = value;
  return e;
}

Event channelPressure(float value)
{
  Event e;
  e.type = kChannelPressure;
  e.channel = 1;
  e.value1 = value;
  return e;
}
}  // namespace

TEST_CASE("madronalib/core/events/controller-values", "[control]")
{
  EventsToSignals e2s;
  e2s.setSampleRate(48000);
  e2s.setPolyphony(2);

  REQUIRE(kNumControllerValues == 128);
  REQUIRE(sizeof(ControllerValues) == 128 * sizeof(float));

  // nothing yet
  for (float v : e2s.getControllerValues()) REQUIRE(v == 0.f);

  // a controller event sets its slot and nothing else, and the value persists
  e2s.addEvent(controller(1, 0.5f));
  e2s.addEvent(controller(74, 0.25f, 9));
  e2s.makeSignalBlock();
  REQUIRE(e2s.getControllerValues()[1] == 0.5f);
  REQUIRE(e2s.getControllerValues()[74] == 0.25f);
  REQUIRE(e2s.getControllerValues()[2] == 0.f);
  e2s.makeSignalBlock();
  REQUIRE(e2s.getControllerValues()[1] == 0.5f);

  // channel pressure does not land in the table
  e2s.addEvent(channelPressure(0.9f));
  e2s.makeSignalBlock();
  for (size_t i = 0; i < kNumControllerValues; ++i)
  {
    if (i != 1 && i != 74) REQUIRE(e2s.getControllerValues()[i] == 0.f);
  }

  // clear() resets the table
  e2s.clear();
  REQUIRE(e2s.getControllerValues()[1] == 0.f);
}

TEST_CASE("madronalib/core/dsp/gate-transition", "[control]")
{
  SECTION("encode and decode round trip, including frames beyond a 16-frame block")
  {
    for (size_t frame : {size_t(0), size_t(1), size_t(7), size_t(15), size_t(63), size_t(200)})
    {
      float rise = encodeGateTransition(true, frame);
      float fall = encodeGateTransition(false, frame);
      REQUIRE(gateHasTransition(rise));
      REQUIRE(gateHasTransition(fall));
      REQUIRE(gateFrame(rise) == frame);
      REQUIRE(gateFrame(fall) == frame);
      REQUIRE(gateLevelAfter(rise, false));
      REQUIRE(!gateLevelAfter(fall, true));
    }
    REQUIRE(!gateHasTransition(kNoGateTransition));
    REQUIRE(gateLevelAfter(kNoGateTransition, true));
    REQUIRE(!gateLevelAfter(kNoGateTransition, false));
  }

  SECTION("expand a rise")
  {
    SignalBlock s;
    bool after = expandGate(encodeGateTransition(true, 5), false, s);
    REQUIRE(after);
    for (size_t i = 0; i < 5; ++i) REQUIRE(s[i] == 0.f);
    for (size_t i = 5; i < kFramesPerBlock; ++i) REQUIRE(s[i] == 1.f);
  }

  SECTION("expand a fall")
  {
    SignalBlock s;
    bool after = expandGate(encodeGateTransition(false, 11), true, s);
    REQUIRE(!after);
    for (size_t i = 0; i < 11; ++i) REQUIRE(s[i] == 1.f);
    for (size_t i = 11; i < kFramesPerBlock; ++i) REQUIRE(s[i] == 0.f);
  }

  SECTION("no transition keeps the level")
  {
    SignalBlock s;
    REQUIRE(expandGate(kNoGateTransition, true, s));
    for (size_t i = 0; i < kFramesPerBlock; ++i) REQUIRE(s[i] == 1.f);
    REQUIRE(!expandGate(kNoGateTransition, false, s));
    for (size_t i = 0; i < kFramesPerBlock; ++i) REQUIRE(s[i] == 0.f);
  }

  SECTION("a rise at frame 0 is all high")
  {
    SignalBlock s;
    expandGate(encodeGateTransition(true, 0), false, s);
    for (size_t i = 0; i < kFramesPerBlock; ++i) REQUIRE(s[i] == 1.f);
  }
}

TEST_CASE("madronalib/core/events/voice-controls", "[control]")
{
  EventsToSignals e2s;
  e2s.setSampleRate(48000);
  e2s.setPolyphony(4);

  SECTION("layout")
  {
    REQUIRE(kNumControlRows == 10);
    REQUIRE(kNumControlVoices == EventsToSignals::kMaxVoices);
    REQUIRE(sizeof(ControlRow) == 16 * sizeof(float));
    REQUIRE(sizeof(VoiceControls) == 10 * 16 * sizeof(float));
    static_assert(std::is_trivially_copyable<VoiceControls>::value, "VoiceControls must be memcpy-able");
  }

  SECTION("note-on at frame 0 on a fresh voice")
  {
    const float pitch = 0.25f;
    const float velocity = 0.75f;
    e2s.addEvent(noteOn(60, pitch, velocity));
    e2s.makeSignalBlock();

    const VoiceControls& vc = e2s.getVoiceControls();
    int v = e2s.getNewestVoice();
    REQUIRE(v >= 0);

    REQUIRE(vc.rows[kCtlGate][v] == encodeGateTransition(true, 0));
    REQUIRE(vc.rows[kCtlVelocity][v] == Approx(velocity));
    REQUIRE(vc.rows[kCtlPitch][v] == Approx(pitch));
    REQUIRE(vc.rows[kCtlPitch][v] == e2s.getVoice(v).outputs.rowPtr(kPitch)[kFramesPerBlock - 1]);

    // voices beyond the polyphony are zero
    REQUIRE(vc.rows[kCtlGate][kNumControlVoices - 1] == 0.f);
    REQUIRE(vc.rows[kCtlPitch][kNumControlVoices - 1] == 0.f);

    // the next block has no transition and the same values
    e2s.makeSignalBlock();
    REQUIRE(e2s.getVoiceControls().rows[kCtlGate][v] == kNoGateTransition);
    REQUIRE(e2s.getVoiceControls().rows[kCtlVelocity][v] == Approx(velocity));
    REQUIRE(e2s.getVoiceControls().rows[kCtlPitch][v] == Approx(pitch));
  }

  SECTION("note-on in the middle of a block is sample accurate")
  {
    e2s.addEvent(noteOn(64, 0.5f, 0.9f, 8));
    e2s.makeSignalBlock();
    int v = e2s.getNewestVoice();
    REQUIRE(v >= 0);
    const VoiceControls& vc = e2s.getVoiceControls();
    REQUIRE(vc.rows[kCtlGate][v] == encodeGateTransition(true, 8));
    REQUIRE(vc.rows[kCtlVelocity][v] == Approx(0.9f));
    REQUIRE(vc.rows[kCtlPitch][v] == Approx(0.5f));

    // expanding the code reproduces the audio-rate gate's shape exactly
    SignalBlock expanded;
    expandGate(vc.rows[kCtlGate][v], false, expanded);
    const float* gateRow = e2s.getVoice(v).outputs.rowPtr(kGate);
    for (size_t i = 0; i < kFramesPerBlock; ++i) REQUIRE(expanded[i] == (gateRow[i] > 0.f ? 1.f : 0.f));
  }

  SECTION("note-off steps the gate at its frame")
  {
    e2s.addEvent(noteOn(60, 0.25f, 1.f));
    e2s.makeSignalBlock();
    int v = e2s.getNewestVoice();
    REQUIRE(v >= 0);

    e2s.addEvent(noteOff(60, 11));
    e2s.makeSignalBlock();
    REQUIRE(e2s.getVoiceControls().rows[kCtlGate][v] == encodeGateTransition(false, 11));
    REQUIRE(e2s.getVoiceControls().rows[kCtlPitch][v] == Approx(0.25f));

    e2s.makeSignalBlock();
    REQUIRE(e2s.getVoiceControls().rows[kCtlGate][v] == kNoGateTransition);
  }

  SECTION("a stolen voice: fall this block, rise at frame 0 of the next")
  {
    e2s.setPolyphony(1);
    e2s.addEvent(noteOn(60, 0.f, 1.f));
    e2s.makeSignalBlock();
    int v = e2s.getNewestVoice();
    REQUIRE(v >= 0);

    // second note while the first is held steals the only voice with a retrigger:
    // the audio-rate gate falls for one frame then rises. We encode the fall.
    e2s.addEvent(noteOn(67, 0.5f, 0.8f, 8));
    e2s.makeSignalBlock();
    REQUIRE(e2s.getNewestVoice() == v);
    const float code = e2s.getVoiceControls().rows[kCtlGate][v];
    REQUIRE(gateHasTransition(code));
    REQUIRE(!gateLevelAfter(code, true));

    // the next block corrects: a rise at frame 0 carrying the new note's values
    e2s.makeSignalBlock();
    REQUIRE(e2s.getVoiceControls().rows[kCtlGate][v] == encodeGateTransition(true, 0));
    REQUIRE(e2s.getVoiceControls().rows[kCtlPitch][v] == Approx(0.5f));
    REQUIRE(e2s.getVoiceControls().rows[kCtlVelocity][v] == Approx(0.8f));
  }

  SECTION("pitch bend changes the pitch row without touching the gate")
  {
    e2s.addEvent(noteOn(60, 0.f, 1.f));
    e2s.makeSignalBlock();
    int v = e2s.getNewestVoice();
    REQUIRE(v >= 0);

    e2s.addEvent(pitchBend(1.f));
    float previous = e2s.getVoiceControls().rows[kCtlPitch][v];
    bool moved = false;
    for (int block = 0; block < 8; ++block)
    {
      e2s.makeSignalBlock();
      REQUIRE(e2s.getVoiceControls().rows[kCtlGate][v] == kNoGateTransition);
      float p = e2s.getVoiceControls().rows[kCtlPitch][v];
      if (p != previous) moved = true;
      REQUIRE(p >= previous);
      previous = p;
    }
    REQUIRE(moved);
  }
}
