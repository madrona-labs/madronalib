// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2022 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#include "catch.hpp"
#include "madronalib.h"
#include "MLTestUtils.h"

using namespace ml;

// no-op signal processing function — we're testing event routing, not DSP
std::function<void(AudioContext*)> nullProcessFn = [](AudioContext*){};

// helper: simulate one host callback. adds events, then calls process().
static void hostCallback(AudioContext& ctx,
                          float** outputs, int nFrames,
                          const std::vector<Event>& events = {})
{
  for (const auto& e : events)
  {
    ctx.addInputEvent(e);
  }
  ctx.process(nullptr, outputs, nFrames, nullProcessFn);
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
static constexpr int kPolyphony = 4;

// helper: set up context and process buffer for testing
struct TestFixture
{
  AudioContext ctx{0, 2};
  float outputData[2][kMaxTestFrames]{};
  float* outputs[2]{outputData[0], outputData[1]};

  TestFixture()
  {
    // there's no hardware so set a default rate
    ctx.setSampleRate(48000);
    ctx.setInputPolyphony(kPolyphony);
  }

  void callback(int nFrames, const std::vector<Event>& events = {})
  {
    hostCallback(ctx, outputs, nFrames, events);
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

TEST_CASE("madronalib/core/events/small_buffer", "[events]")
{
  TestFixture t;
  const int bufSize = kFramesPerBlock/4;
  
  // callback 1: note on
  t.callback(bufSize, {makeNoteOn(60, 60.f, 0.8f, 5)});
  t.callback(bufSize);
  t.callback(bufSize);
  t.callback(bufSize);
  
  // after kFramesPerBlock samples, processBlock ran — gate should be on
  REQUIRE(t.gateEnd(0) > 0.f);
  
  // callback 2: note off — loop may not run yet, events must survive
  t.callback(bufSize, {makeNoteOff(60, 60.f, 2)});
  t.callback(bufSize);
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

TEST_CASE("madronalib/core/events/odd_buffer_size", "[events]")
{
  TestFixture t;
  const int bufSize = kFramesPerBlock + 5;
  
  Event e1{makeNoteOn(60, 60.f, 0.8f, 1)};
  Event e3{makeNoteOff(60, 60.f, bufSize - 3)};
  
  // callback 1: note on and off
  // 5 extra samples at end contain note-off
  t.callback(bufSize, {e1, e3});
  
  // run one more callback to ensure processBlock has processed both
  t.callback(bufSize);
  
  // all voices should be off
  bool foundNoteOn = false;
  REQUIRE(!foundNoteOn);
}


TEST_CASE("madronalib/core/events/basic_note_on_off", "[events]")
{
  TestFixture t;
  const int bufSize = kFramesPerBlock;

  // note on
  t.callback(bufSize, {makeNoteOn(60, 60.f, 0.8f, 5)});
  t.callback(bufSize);
  
  REQUIRE(t.gateEnd(0) > 0.f);
  REQUIRE(t.pitchAt(0, kFramesPerBlock - 1) == Approx(60.f));

  // note off
  t.callback(bufSize, {makeNoteOff(60, 60.f, 12)});
  
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

  // run enough empty callbacks to ensure processBlock processes the note-off
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
  
  // run one more callback to ensure processBlock has processed both
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

TEST_CASE("madronalib/core/events/onoff_startend", "[events]")
{
  TestFixture t;
  const int bufSize = 32;
  
  Event e1{makeNoteOn(60, 60.f, 0.8f, 0)};
  Event e2{makeNoteOff(60, 60.f, bufSize - 1)};
  
  // callback 1: note on and off
  // TODO this fails even at buffer size = 32!
  t.callback(bufSize, {e1, e2});
  
  // run one more callback to ensure processBlock has processed both
  t.callback(bufSize);
  
  // all voices should be off
  bool foundNoteOn = false;
  for (int v = 0; v < kPolyphony; ++v)
  {
    float gate = t.gateEnd(v);
    float pitch = t.pitchAt(v, kFramesPerBlock - 1);
    if (gate > 0.f) foundNoteOn = true;
  }
  REQUIRE(!foundNoteOn);
  
}

TEST_CASE("madronalib/core/events/rapid_on_off", "[events]")
{
  TestFixture t;
  const int bufSize = 32;
  
  // note on at sample 2, note off at sample 20, both in same callback
  t.callback(bufSize, {makeNoteOn(60, 60.f, 0.8f, 2), makeNoteOff(60, 60.f, 3)});
  
  // run callbacks until processBlock has processed everything
  t.callback(bufSize);
  t.callback(bufSize);
  
  REQUIRE(t.gateEnd(0) == 0.f);
}

TEST_CASE("madronalib/core/events/on_off_same_time", "[events]")
{
  TestFixture t;
  const int bufSize = 32;
  
  // note on at sample 2, note off at sample 20, both in same callback
  t.callback(bufSize, {makeNoteOn(60, 60.f, 0.8f, 2), makeNoteOff(60, 60.f, 2)});
  
  // run callbacks until processBlock has processed everything
  t.callback(bufSize);
  t.callback(bufSize);
  
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


// CLAP addresses "all keys" / "all channels" with -1. ml::Event stores both
// fields unsigned (sourceIdx is uint16_t, channel is uint8_t), so a -1 that
// reaches here has already become 65535 or 255 -- and keyStates_ only has
// kMaxPhysicalKeys entries. Before getKeyIndex clamped, this wrote about a
// megabyte past the end of the array and segfaulted.
TEST_CASE("madronalib/core/events/out_of_range_key", "[events]")
{
  TestFixture t;
  const int bufSize = kFramesPerBlock;

  // what a CLAP wildcard key (-1) looks like by the time it arrives
  const uint16_t wildcardKey = (uint16_t)-1;
  REQUIRE(wildcardKey == 65535);

  Event wild;
  wild.type = kNoteOn;
  wild.channel = 1;
  wild.sourceIdx = wildcardKey;
  wild.value1 = 60.f;
  wild.value2 = 0.8f;

  // must not write outside keyStates_
  t.callback(bufSize, {wild});

  Event wildOff = wild;
  wildOff.type = kNoteOff;
  t.callback(bufSize, {wildOff});

  // a normal note still behaves after the clamped one
  t.callback(bufSize, {makeNoteOn(60, 60.f, 0.8f, 0)});
  REQUIRE(t.gateEnd(1) > 0.f);
  t.callback(bufSize, {makeNoteOff(60, 60.f, 0)});
  t.callback(bufSize);
  REQUIRE(t.gateEnd(1) == 0.f);
}

// under MPE the channel only routes expression and is never an index, so a
// wildcard channel must be harmless there too.
TEST_CASE("madronalib/core/events/out_of_range_channel", "[events]")
{
  TestFixture t;
  t.ctx.setInputProtocol(Symbol("MPE"));

  Event wild;
  wild.type = kNoteOn;
  wild.channel = (uint8_t)-1;  // CLAP wildcard channel
  wild.sourceIdx = 60;
  wild.value1 = 60.f;
  wild.value2 = 0.8f;

  REQUIRE(wild.channel == 255);

  t.callback(kFramesPerBlock, {wild});
}

// helper: make a MIDI controller event
static Event makeController(int cc, float value, int time = 0)
{
  Event e;
  e.type = kController;
  e.channel = 1;
  e.sourceIdx = cc;
  e.time = time;
  e.value1 = value;
  return e;
}

// CC 120 (all sound off) is handled inside makeSignalBlock()'s loop over the
// event buffer, so it must not empty that buffer out from under the loop.
TEST_CASE("madronalib/core/events/all_sound_off", "[events]")
{
  TestFixture t;
  const int bufSize = kFramesPerBlock;

  t.callback(bufSize, {makeNoteOn(60, 60.f, 0.8f, 0)});
  REQUIRE(t.gateEnd(0) > 0.f);

  t.callback(bufSize, {makeController(120, 0.f, 0)});
  REQUIRE(t.gateEnd(0) == 0.f);
}

// all sound off silences what is sounding now. a note queued after it in the
// same host buffer still plays.
TEST_CASE("madronalib/core/events/note_after_all_sound_off", "[events]")
{
  TestFixture t;
  const int bufSize = kFramesPerBlock * 2;

  t.callback(bufSize, {makeController(120, 0.f, 0), makeNoteOn(60, 60.f, 0.8f, kFramesPerBlock + 8)});
  REQUIRE(t.gateEnd(0) > 0.f);
}

// ---- creator channel ----
// A synth following an MTS-ESP master retunes each voice on the MIDI channel
// that started it, through its release tail, so the voice keeps that channel
// after note-off.

static Event makeNoteOnOnChannel(int channel, int key, int time = 0)
{
  Event e = makeNoteOn(key, (float)key, 0.8f, time);
  e.channel = channel;
  return e;
}

TEST_CASE("madronalib/core/events/voice_keeps_creator_channel", "[events]")
{
  TestFixture t;
  t.ctx.setInputPolyphony(1);

  t.callback(kFramesPerBlock, {makeNoteOnOnChannel(3, 60)});
  REQUIRE(t.ctx.getInputVoice(0).creatorChannel_ == 3);

  // note-off ends the gate but the voice's channel stays for the release tail
  t.callback(kFramesPerBlock, {makeNoteOff(60, 60.f)});
  REQUIRE(t.gateEnd(0) == 0.f);
  REQUIRE(t.ctx.getInputVoice(0).creatorChannel_ == 3);

  // the next note on another channel takes over
  t.callback(kFramesPerBlock, {makeNoteOnOnChannel(5, 62)});
  REQUIRE(t.ctx.getInputVoice(0).creatorChannel_ == 5);
}

// ---- pitch across a reset ----
// A reset (polyphony or protocol change, host reset, all sound off) ends the
// note but keeps the voice's pitch, as Aalto 1's input proc did. A synth that
// reads pitch while the gate is closed then sees the last note, not note 0.

TEST_CASE("madronalib/core/events/voice_keeps_pitch_across_reset", "[events]")
{
  TestFixture t;
  t.ctx.setInputPolyphony(1);

  t.callback(kFramesPerBlock, {makeNoteOn(60, 60.f, 0.8f)});
  t.callback(kFramesPerBlock, {makeNoteOff(60, 60.f)});
  REQUIRE(t.gateEnd(0) == 0.f);
  REQUIRE(t.pitchAt(0, kFramesPerBlock - 1) == 60.f);

  // a polyphony change resets every voice
  t.ctx.setInputPolyphony(2);
  t.callback(kFramesPerBlock);
  REQUIRE(t.gateEnd(0) == 0.f);
  REQUIRE(t.pitchAt(0, kFramesPerBlock - 1) == 60.f);

  // so does all sound off
  t.callback(kFramesPerBlock, {makeController(120, 0.f, 0)});
  t.callback(kFramesPerBlock);
  REQUIRE(t.pitchAt(0, kFramesPerBlock - 1) == 60.f);

  // the next note moves it as usual
  t.callback(kFramesPerBlock, {makeNoteOn(62, 62.f, 0.8f)});
  REQUIRE(t.pitchAt(0, kFramesPerBlock - 1) == 62.f);
}

// helper: make a pitch bend event, value in [-1, 1]
static Event makePitchBend(int channel, float value, int time = 0)
{
  Event e;
  e.type = kPitchBend;
  e.channel = channel;
  e.time = time;
  e.value1 = value;
  return e;
}

static float bendEnd(TestFixture& t, int voice)
{
  return t.ctx.getInputVoice(voice).outputs.constRow(kBend)[kFramesPerBlock - 1];
}

// enough blocks to finish the pitch bend glide
static void runPastBendGlide(TestFixture& t)
{
  for (int i = 0; i < 32; ++i) t.callback(kFramesPerBlock);
}

TEST_CASE("madronalib/core/events/bend_row_zero_without_bend", "[events]")
{
  TestFixture t;
  t.callback(kFramesPerBlock, {makeNoteOn(60, 60.f, 0.8f)});
  runPastBendGlide(t);

  REQUIRE(bendEnd(t, 0) == 0.f);
  REQUIRE(t.pitchAt(0, kFramesPerBlock - 1) == Approx(60.f));
}

TEST_CASE("madronalib/core/events/bend_row_holds_bend_added_to_pitch", "[events]")
{
  TestFixture t;
  t.ctx.setInputPitchBend(12.f);

  t.callback(kFramesPerBlock, {makeNoteOn(60, 60.f, 0.8f)});
  t.callback(kFramesPerBlock, {makePitchBend(1, 0.5f)});
  runPastBendGlide(t);

  // half of a 12 semitone range is half an octave
  REQUIRE(bendEnd(t, 0) == Approx(0.5f));
  REQUIRE(t.pitchAt(0, kFramesPerBlock - 1) == Approx(60.5f));

  // subtracting the bend row recovers the note at every point in the glide
  t.callback(kFramesPerBlock, {makePitchBend(1, -1.f)});
  float pitch = t.pitchAt(0, kFramesPerBlock - 1);
  float bend = bendEnd(t, 0);
  REQUIRE(bend < 0.5f);
  REQUIRE(bend > -1.f);
  REQUIRE(pitch - bend == Approx(60.f));
}

TEST_CASE("madronalib/core/events/bend_row_mpe_sums_main_and_note_bend", "[events]")
{
  TestFixture t;
  t.ctx.setInputProtocol("MPE");
  t.ctx.setInputPitchBend(12.f);
  t.ctx.setInputMPEPitchBend(24.f);

  Event noteOn = makeNoteOn(60, 60.f, 0.8f);
  noteOn.channel = 2;
  t.callback(kFramesPerBlock, {noteOn});

  // per-note bend on the member channel, main bend on channel 1
  t.callback(kFramesPerBlock, {makePitchBend(2, 0.5f), makePitchBend(1, 0.25f)});
  runPastBendGlide(t);

  int v = t.ctx.getNewestInputVoice();
  REQUIRE(v >= 0);
  const float expected = 0.5f * 24.f / 12.f + 0.25f * 12.f / 12.f;
  REQUIRE(bendEnd(t, v) == Approx(expected));
  REQUIRE(t.pitchAt(v, kFramesPerBlock - 1) == Approx(60.f + expected));
}

// ---- MPE: a key is a note, the channel routes expression ----
// Several notes can share a channel: a plain keyboard into MPE mode, a host
// that sends every note on one channel, or an MPE controller out of channels.
// A note-off must end only its own note.

static Event makeNoteOffOnChannel(int channel, int key)
{
  Event e = makeNoteOff(key, (float)key);
  e.channel = channel;
  return e;
}

static Event makeChannelPressure(int channel, float value)
{
  Event e;
  e.type = kChannelPressure;
  e.channel = channel;
  e.value1 = value;
  return e;
}

static Event makeSustainPedal(float value)
{
  Event e;
  e.type = kSustainPedal;
  e.channel = 1;
  e.value1 = value;
  return e;
}

static float zEnd(TestFixture& t, int voice)
{
  return t.ctx.getInputVoice(voice).outputs.constRow(kZ)[kFramesPerBlock - 1];
}

TEST_CASE("madronalib/core/events/mpe_notes_sharing_a_channel", "[events]")
{
  TestFixture t;
  t.ctx.setInputProtocol("MPE");

  t.callback(kFramesPerBlock, {makeNoteOnOnChannel(1, 60)});
  t.callback(kFramesPerBlock, {makeNoteOnOnChannel(1, 64)});
  REQUIRE(t.gateEnd(0) > 0.f);
  REQUIRE(t.gateEnd(1) > 0.f);

  // releasing the first note leaves the second sounding
  t.callback(kFramesPerBlock, {makeNoteOffOnChannel(1, 60)});
  t.callback(kFramesPerBlock);
  REQUIRE(t.gateEnd(0) == 0.f);
  REQUIRE(t.gateEnd(1) > 0.f);

  t.callback(kFramesPerBlock, {makeNoteOffOnChannel(1, 64)});
  t.callback(kFramesPerBlock);
  REQUIRE(t.gateEnd(1) == 0.f);
}

TEST_CASE("madronalib/core/events/mpe_same_key_on_two_channels", "[events]")
{
  TestFixture t;
  t.ctx.setInputProtocol("MPE");

  t.callback(kFramesPerBlock, {makeNoteOnOnChannel(2, 60)});
  t.callback(kFramesPerBlock, {makeNoteOnOnChannel(3, 60)});

  t.callback(kFramesPerBlock, {makeNoteOffOnChannel(2, 60)});
  t.callback(kFramesPerBlock);
  REQUIRE(t.gateEnd(0) == 0.f);
  REQUIRE(t.gateEnd(1) > 0.f);
}

TEST_CASE("madronalib/core/events/mpe_unison_notes_sharing_a_channel", "[events]")
{
  TestFixture t;
  t.ctx.setInputProtocol("MPE");
  t.ctx.setInputUnison(true);

  t.callback(kFramesPerBlock, {makeNoteOnOnChannel(1, 60)});
  t.callback(kFramesPerBlock, {makeNoteOnOnChannel(1, 64)});

  // one key is still down, so the voices keep playing, back on the held note
  t.callback(kFramesPerBlock, {makeNoteOffOnChannel(1, 64)});
  t.callback(kFramesPerBlock);
  REQUIRE(t.gateEnd(0) > 0.f);
  REQUIRE(t.ctx.getInputVoice(0).currentPitch == 60.f);

  t.callback(kFramesPerBlock, {makeNoteOffOnChannel(1, 60)});
  t.callback(kFramesPerBlock);
  REQUIRE(t.gateEnd(0) == 0.f);
}

TEST_CASE("madronalib/core/events/mpe_expression_follows_the_channel", "[events]")
{
  TestFixture t;
  t.ctx.setInputProtocol("MPE");
  t.ctx.setInputMPEPitchBend(24.f);

  t.callback(kFramesPerBlock, {makeNoteOnOnChannel(2, 60)});
  t.callback(kFramesPerBlock, {makeNoteOnOnChannel(3, 64)});

  // bend and pressure on channel 2 move only the channel 2 voice
  t.callback(kFramesPerBlock, {makePitchBend(2, 0.5f), makeChannelPressure(2, 0.75f)});
  runPastBendGlide(t);
  REQUIRE(bendEnd(t, 0) == Approx(0.5f * 24.f / 12.f));
  REQUIRE(zEnd(t, 0) == Approx(0.75f));
  REQUIRE(bendEnd(t, 1) == 0.f);
  REQUIRE(zEnd(t, 1) == 0.f);

  // after its note-off the voice stops following the channel
  t.callback(kFramesPerBlock, {makeNoteOffOnChannel(2, 60)});
  t.callback(kFramesPerBlock, {makePitchBend(2, -0.5f)});
  runPastBendGlide(t);
  REQUIRE(bendEnd(t, 0) == Approx(0.5f * 24.f / 12.f));
}

TEST_CASE("madronalib/core/events/mpe_sustain_same_key_on_two_channels", "[events]")
{
  TestFixture t;
  t.ctx.setInputProtocol("MPE");

  t.callback(kFramesPerBlock, {makeNoteOnOnChannel(2, 60)});
  t.callback(kFramesPerBlock, {makeNoteOnOnChannel(3, 60)});

  // the pedal holds the released note
  t.callback(kFramesPerBlock, {makeSustainPedal(1.f)});
  t.callback(kFramesPerBlock, {makeNoteOffOnChannel(2, 60)});
  t.callback(kFramesPerBlock);
  REQUIRE(t.gateEnd(0) > 0.f);
  REQUIRE(t.gateEnd(1) > 0.f);

  // pedal up ends it, and leaves the key still down on channel 3
  t.callback(kFramesPerBlock, {makeSustainPedal(0.f)});
  t.callback(kFramesPerBlock);
  REQUIRE(t.gateEnd(0) == 0.f);
  REQUIRE(t.gateEnd(1) > 0.f);
}

// ---- note expressions ----
// Per-note changes addressed to a key, as a host sends them when it does the
// per-note work itself. Every note can be on one channel.

static Event makeNoteExpression(int type, int channel, int key, float value, int time = 0)
{
  Event e;
  e.type = type;
  e.channel = channel;
  e.sourceIdx = key;
  e.time = time;
  e.value1 = value;
  return e;
}

static float yEnd(TestFixture& t, int voice)
{
  return t.ctx.getInputVoice(voice).outputs.constRow(kY)[kFramesPerBlock - 1];
}

TEST_CASE("madronalib/core/events/note_expressions_follow_the_key", "[events]")
{
  for (const char* protocol : {"MIDI", "MPE"})
  {
    TestFixture t;
    t.ctx.setInputProtocol(protocol);

    t.callback(kFramesPerBlock, {makeNoteOnOnChannel(1, 60)});
    t.callback(kFramesPerBlock, {makeNoteOnOnChannel(1, 64)});

    t.callback(kFramesPerBlock, {makeNoteExpression(kNotePitchOffset, 1, 64, 0.5f),
                                 makeNoteExpression(kNoteZ, 1, 64, 0.75f),
                                 makeNoteExpression(kNoteY, 1, 64, 0.25f)});
    runPastBendGlide(t);

    // only the voice playing key 64 moves
    REQUIRE(t.pitchAt(0, kFramesPerBlock - 1) == Approx(60.f));
    REQUIRE(bendEnd(t, 0) == 0.f);
    REQUIRE(zEnd(t, 0) == 0.f);
    REQUIRE(yEnd(t, 0) == 0.f);
    REQUIRE(t.pitchAt(1, kFramesPerBlock - 1) == Approx(64.5f));
    REQUIRE(bendEnd(t, 1) == Approx(0.5f));
    REQUIRE(zEnd(t, 1) == Approx(0.75f));
    REQUIRE(yEnd(t, 1) == Approx(0.25f));
  }
}

TEST_CASE("madronalib/core/events/note_expression_channels", "[events]")
{
  TestFixture t;

  // the same key on two channels
  t.callback(kFramesPerBlock, {makeNoteOnOnChannel(2, 60)});
  t.callback(kFramesPerBlock, {makeNoteOnOnChannel(3, 60)});

  // a channel picks one of them
  t.callback(kFramesPerBlock, {makeNoteExpression(kNoteZ, 3, 60, 0.5f)});
  runPastBendGlide(t);
  REQUIRE(zEnd(t, 0) == 0.f);
  REQUIRE(zEnd(t, 1) == Approx(0.5f));

  // channel 0 is any channel
  t.callback(kFramesPerBlock, {makeNoteExpression(kNoteZ, 0, 60, 0.25f)});
  runPastBendGlide(t);
  REQUIRE(zEnd(t, 0) == Approx(0.25f));
  REQUIRE(zEnd(t, 1) == Approx(0.25f));
}

TEST_CASE("madronalib/core/events/note_pitch_offset_starts_with_the_note", "[events]")
{
  TestFixture t;
  t.ctx.setInputPolyphony(1);

  // an offset in the note-on's block is the note's starting pitch: no glide
  t.callback(kFramesPerBlock, {makeNoteOnOnChannel(1, 60), makeNoteExpression(kNotePitchOffset, 1, 60, 0.5f)});
  REQUIRE(t.pitchAt(0, kFramesPerBlock - 1) == Approx(60.5f));

  // a later offset glides
  t.callback(kFramesPerBlock, {makeNoteExpression(kNotePitchOffset, 1, 60, 1.f)});
  REQUIRE(t.pitchAt(0, kFramesPerBlock - 1) < 61.f);
  runPastBendGlide(t);
  REQUIRE(t.pitchAt(0, kFramesPerBlock - 1) == Approx(61.f));

  // the next note starts with no offset
  t.callback(kFramesPerBlock, {makeNoteOffOnChannel(1, 60)});
  t.callback(kFramesPerBlock, {makeNoteOnOnChannel(1, 62)});
  REQUIRE(t.pitchAt(0, kFramesPerBlock - 1) == Approx(62.f));

  // and a released note no longer follows its key
  t.callback(kFramesPerBlock, {makeNoteOffOnChannel(1, 62)});
  t.callback(kFramesPerBlock, {makeNoteExpression(kNotePitchOffset, 1, 62, 1.f)});
  runPastBendGlide(t);
  REQUIRE(t.pitchAt(0, kFramesPerBlock - 1) == Approx(62.f));
}
