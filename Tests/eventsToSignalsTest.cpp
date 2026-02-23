// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2022 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#include "catch.hpp"
#include "madronalib.h"
#include "MLTestUtils.h"

using namespace ml;

// no-op signal processing function — we're testing event routing, not DSP
static void nullProcessFn(AudioContext*, void*) {}

// helper: simulate one host callback. adds events, then calls process().
static void hostCallback(SignalProcessBuffer& buf, AudioContext& ctx,
                          float** outputs, int nFrames,
                          const std::vector<Event>& events = {})
{
  for (const auto& e : events)
  {
    ctx.addInputEvent(e);
  }
  buf.process(nullptr, outputs, nFrames, &ctx, nullProcessFn, nullptr);
}

// helper: make a note-on event
static Event makeNoteOn(int key, float pitch, float velocity, int time = 0)
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

// helper: make a note-off event
static Event makeNoteOff(int key, float pitch, int time = 0)
{
  Event e;
  e.type = kNoteOff;
  e.channel = 1;
  e.sourceIdx = key;
  e.time = time;
  e.value1 = pitch;
  e.value2 = 0.f;
  return e;
}

static constexpr int kMaxTestFrames = 256;
static constexpr int kSampleRate = 48000;
static constexpr int kPolyphony = 4;

// helper: set up context and process buffer for testing
struct TestFixture
{
  AudioContext ctx{0, 2, kSampleRate};
  SignalProcessBuffer buf{0, 2, kMaxTestFrames};
  float outputData[2][kMaxTestFrames]{};
  float* outputs[2]{outputData[0], outputData[1]};

  TestFixture()
  {
    ctx.setInputPolyphony(kPolyphony);
  }

  void callback(int nFrames, const std::vector<Event>& events = {})
  {
    hostCallback(buf, ctx, outputs, nFrames, events);
  }

  float gateAt(int voice, int sample)
  {
    return ctx.getInputVoice(voice).outputs.constRow(kGate)[sample];
  }

  float pitchAt(int voice, int sample)
  {
    return ctx.getInputVoice(voice).outputs.constRow(kPitch)[sample];
  }

  // check gate at end of most recent DSP vector
  float gateEnd(int voice) { return gateAt(voice, kFramesPerBlock - 1); }
};


TEST_CASE("madronalib/core/events/basic_note_on_off", "[events]")
{
  TestFixture t;
  const int bufSize = kFramesPerBlock;

  // note on
  t.callback(bufSize, {makeNoteOn(60, 60.f, 0.8f, 5)});
  REQUIRE(t.gateEnd(0) > 0.f);
  REQUIRE(t.pitchAt(0, kFramesPerBlock - 1) == Approx(60.f));

  // note off
  t.callback(bufSize, {makeNoteOff(60, 60.f, 5)});
  REQUIRE(t.gateEnd(0) == 0.f);
}

TEST_CASE("madronalib/core/events/small_buffer_32", "[events]")
{
  // This is the exact stuck-notes scenario: note-on in callback 1,
  // note-off in callback 2, with 32-sample host buffers.
  TestFixture t;
  const int bufSize = 32;

  // callback 1: note on
  t.callback(bufSize, {makeNoteOn(60, 60.f, 0.8f, 5)});

  // after first callback, processVector ran — gate should be on
  REQUIRE(t.gateEnd(0) > 0.f);

  // callback 2: note off — loop may not run yet, events must survive
  t.callback(bufSize, {makeNoteOff(60, 60.f, 5)});

  // callback 3: empty — processVector runs, consuming the note-off
  t.callback(bufSize);

  // gate must be off — the note-off must not have been dropped
  REQUIRE(t.gateEnd(0) == 0.f);
}

TEST_CASE("madronalib/core/events/small_buffer_16", "[events]")
{
  TestFixture t;
  const int bufSize = 16;

  // callback 1: note on
  t.callback(bufSize, {makeNoteOn(60, 60.f, 0.8f, 2)});

  // callbacks 2-3: empty
  t.callback(bufSize);
  t.callback(bufSize);

  // callback 4: note off
  t.callback(bufSize, {makeNoteOff(60, 60.f, 2)});

  // run enough empty callbacks to ensure processVector processes the note-off
  for (int i = 0; i < 4; ++i)
  {
    t.callback(bufSize);
  }

  REQUIRE(t.gateEnd(0) == 0.f);
}

TEST_CASE("madronalib/core/events/multiple_notes_small_buffer", "[events]")
{
  TestFixture t;
  const int bufSize = 32;

  // callback 1: note on key 60
  t.callback(bufSize, {makeNoteOn(60, 60.f, 0.8f, 0)});

  // callback 2: note on key 64 (different voice)
  t.callback(bufSize, {makeNoteOn(64, 64.f, 0.7f, 0)});

  // run one more callback to ensure processVector has processed both
  t.callback(bufSize);

  // both voices should be active
  bool foundPitch60 = false;
  bool foundPitch64 = false;
  for (int v = 0; v < kPolyphony; ++v)
  {
    float gate = t.gateEnd(v);
    float pitch = t.pitchAt(v, kFramesPerBlock - 1);
    if (gate > 0.f && pitch == Approx(60.f)) foundPitch60 = true;
    if (gate > 0.f && pitch == Approx(64.f)) foundPitch64 = true;
  }
  REQUIRE(foundPitch60);
  REQUIRE(foundPitch64);
}

TEST_CASE("madronalib/core/events/rapid_on_off_same_buffer", "[events]")
{
  TestFixture t;
  const int bufSize = 32;

  // note on at sample 2, note off at sample 20, both in same callback
  t.callback(bufSize, {makeNoteOn(60, 60.f, 0.8f, 2), makeNoteOff(60, 60.f, 20)});

  // run callbacks until processVector has processed everything
  t.callback(bufSize);
  t.callback(bufSize);

  REQUIRE(t.gateEnd(0) == 0.f);
}

TEST_CASE("madronalib/core/events/large_buffer", "[events]")
{
  // regression test: 128-sample buffer (larger than kFramesPerBlock)
  TestFixture t;
  const int bufSize = 128;

  // note on in first half, note off in second half
  t.callback(bufSize, {makeNoteOn(60, 60.f, 0.8f, 10), makeNoteOff(60, 60.f, 80)});

  // gate should be off after processing
  REQUIRE(t.gateEnd(0) == 0.f);
}

TEST_CASE("madronalib/core/events/sustained_sequence_small_buffer", "[events]")
{
  // play a sequence of notes with small buffers to check for
  // accumulated timing drift or stuck notes over many callbacks
  TestFixture t;
  const int bufSize = 32;

  for (int note = 0; note < 10; ++note)
  {
    int key = 60 + note;

    // note on
    t.callback(bufSize, {makeNoteOn(key, (float)key, 0.8f, 0)});
    t.callback(bufSize);

    // note off
    t.callback(bufSize, {makeNoteOff(key, (float)key, 0)});
    t.callback(bufSize);
  }

  // after all notes, all voices should be off
  for (int v = 0; v < kPolyphony; ++v)
  {
    REQUIRE(t.gateEnd(v) == 0.f);
  }
}
