// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// a unit test made using the Catch framework in catch.hpp / tests.cpp.

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
}  // namespace

TEST_CASE("madronalib/core/dsp/control-block", "[control]")
{
  // a ControlBlock is one float4 of plain data
  static_assert(sizeof(ControlBlock) == 4 * sizeof(float), "ControlBlock must be one float4");
  static_assert(std::is_trivially_copyable<ControlBlock>::value, "ControlBlock must be memcpy-able");

  SECTION("ramp lands on end at the last frame")
  {
    SignalBlock s = interpolateBlockLinear(ControlBlock::ramp(0.f, 1.f));
    REQUIRE(s[kFramesPerBlock - 1] == Approx(1.f));
    REQUIRE(s[0] == Approx(1.f / kFramesPerBlock));
    for (size_t i = 1; i < kFramesPerBlock; ++i)
    {
      REQUIRE(s[i] > s[i - 1]);
    }
  }

  SECTION("a ramp with equal ends is flat")
  {
    SignalBlock flat = interpolateBlockLinear(ControlBlock::ramp(0.5f, 0.5f));
    for (size_t i = 0; i < kFramesPerBlock; ++i)
    {
      REQUIRE(flat[i] == 0.5f);
    }
  }

  SECTION("step holds before until its frame, then after")
  {
    ControlBlock c = ControlBlock::step(-1.f, 2.f, 5);
    REQUIRE(c.isStep());
    SignalBlock s = interpolateBlockLinear(c);
    for (size_t i = 0; i < 5; ++i) REQUIRE(s[i] == -1.f);
    for (size_t i = 5; i < kFramesPerBlock; ++i) REQUIRE(s[i] == 2.f);
  }

  SECTION("step at frame 0 is all after")
  {
    SignalBlock s = interpolateBlockLinear(ControlBlock::step(-1.f, 2.f, 0));
    for (size_t i = 0; i < kFramesPerBlock; ++i) REQUIRE(s[i] == 2.f);
  }

  SECTION("default ControlBlock is a flat zero ramp")
  {
    ControlBlock c;
    REQUIRE(!c.isStep());
    SignalBlock s = interpolateBlockLinear(c);
    for (size_t i = 0; i < kFramesPerBlock; ++i) REQUIRE(s[i] == 0.f);
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
    REQUIRE(sizeof(ControlRow) == 16 * 4 * sizeof(float));
    REQUIRE(sizeof(VoiceControls) == 10 * sizeof(ControlRow));
  }

  SECTION("note-on at frame 0 steps every row at frame 0")
  {
    const float pitch = 0.25f;
    const float velocity = 0.75f;
    e2s.addEvent(noteOn(60, pitch, velocity));
    e2s.makeSignalBlock();

    const VoiceControls& vc = e2s.getVoiceControls();
    int v = e2s.getNewestVoice();
    REQUIRE(v >= 0);

    // the after value is the last frame of the audio-rate row
    const auto& voice = e2s.getVoice(v);
    REQUIRE(vc.rows[kPitch][v].end == voice.outputs.rowPtr(kPitch)[kFramesPerBlock - 1]);

    for (int r : {kPitch, kGate, kVoice, kElapsedTime})
    {
      REQUIRE(vc.rows[r][v].isStep());
      REQUIRE(vc.rows[r][v].offset == 0.f);
    }
    REQUIRE(vc.rows[kPitch][v].end == Approx(pitch));
    REQUIRE(vc.rows[kGate][v].end == Approx(velocity));

    // the reserved row and voices beyond the polyphony are zero ramps
    REQUIRE(!vc.rows[kNumControlRows - 1][v].isStep());
    REQUIRE(vc.rows[kNumControlRows - 1][v].end == 0.f);
    REQUIRE(vc.rows[kPitch][kNumControlVoices - 1].end == 0.f);
    REQUIRE(vc.rows[kGate][kNumControlVoices - 1].end == 0.f);
  }

  SECTION("a note-on in the middle of a block is a sample-accurate step")
  {
    e2s.addEvent(noteOn(60, 0.f, 1.f));
    e2s.makeSignalBlock();
    int v = e2s.getNewestVoice();
    REQUIRE(v >= 0);

    // the next block has no events: the held note is a flat ramp
    e2s.makeSignalBlock();
    const ControlBlock& held = e2s.getVoiceControls().rows[kPitch][v];
    REQUIRE(!held.isStep());
    REQUIRE(held.start == held.end);

    // a new note on another key at frame 8 takes a different voice and steps there
    e2s.addEvent(noteOn(64, 0.5f, 0.9f, 8));
    e2s.makeSignalBlock();
    int v2 = e2s.getNewestVoice();
    REQUIRE(v2 >= 0);
    REQUIRE(v2 != v);
    const ControlBlock& p = e2s.getVoiceControls().rows[kPitch][v2];
    REQUIRE(p.isStep());
    REQUIRE(p.offset == 8.f);
    REQUIRE(p.end == Approx(0.5f));
    const ControlBlock& g = e2s.getVoiceControls().rows[kGate][v2];
    REQUIRE(g.isStep());
    REQUIRE(g.offset == 8.f);
    REQUIRE(g.start == 0.f);
    REQUIRE(g.end == Approx(0.9f));

    // expanding the gate step reproduces the audio-rate gate exactly
    SignalBlock expanded = interpolateBlockLinear(g);
    const float* gateRow = e2s.getVoice(v2).outputs.rowPtr(kGate);
    for (size_t i = 0; i < kFramesPerBlock; ++i) REQUIRE(expanded[i] == gateRow[i]);
  }

  SECTION("a note-off steps the gate at its frame and ramps other rows")
  {
    e2s.addEvent(noteOn(60, 0.25f, 1.f));
    e2s.makeSignalBlock();
    int v = e2s.getNewestVoice();
    REQUIRE(v >= 0);

    e2s.addEvent(noteOff(60, 11));
    e2s.makeSignalBlock();
    const VoiceControls& vc = e2s.getVoiceControls();
    REQUIRE(vc.rows[kGate][v].isStep());
    REQUIRE(vc.rows[kGate][v].offset == 11.f);
    REQUIRE(vc.rows[kGate][v].start == Approx(1.f));
    REQUIRE(vc.rows[kGate][v].end == 0.f);
    REQUIRE(!vc.rows[kPitch][v].isStep());
    REQUIRE(vc.rows[kPitch][v].end == Approx(0.25f));
  }

  SECTION("a glide ramps and stays continuous across blocks")
  {
    e2s.addEvent(noteOn(60, 0.f, 1.f));
    e2s.makeSignalBlock();
    int v = e2s.getNewestVoice();
    REQUIRE(v >= 0);

    // pitch bend is smoothed over kGlideTimeSeconds, so it ramps for many blocks
    e2s.addEvent(pitchBend(1.f));
    e2s.makeSignalBlock();

    float previousEnd = e2s.getVoiceControls().rows[kPitch][v].end;
    bool sawRamp = false;
    for (int block = 0; block < 8; ++block)
    {
      e2s.makeSignalBlock();
      const ControlBlock& c = e2s.getVoiceControls().rows[kPitch][v];
      REQUIRE(!c.isStep());
      float slope = c.end - c.start;
      if (std::fabs(slope) > 0.f) sawRamp = true;

      // start of this block is within one sample step of the previous block's end
      float oneSampleStep = std::fabs(slope) / (kFramesPerBlock - 1);
      REQUIRE(std::fabs(c.start - previousEnd) <= oneSampleStep * 1.5f + 1e-6f);
      previousEnd = c.end;
    }
    REQUIRE(sawRamp);
  }
}
