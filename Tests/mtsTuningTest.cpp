// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// Tests for ml::MTSTuning, the wrapper around ODDSound's MTS-ESP client.
//
// These run without an MTS-ESP master. The client then answers from its local
// table, which starts at 12-ET and follows MIDI Tuning Standard SysEx, so the
// SysEx path is how a test puts a known tuning into the client. If a master is
// running on this machine its table would win, so each case checks first.

#include <cmath>
#include <limits>
#include <string>
#include <vector>

#include "MLMTSClient.h"
#include "catch.hpp"

using namespace ml;

namespace
{
bool masterRunning(const MTSTuning& t)
{
  if (t.hasMaster())
  {
    WARN("An MTS-ESP master is running; skipping offline MTSTuning checks");
    return true;
  }
  return false;
}

// MIDI Tuning Standard real-time single note tuning change:
// F0 7F <device> 08 02 <program> <count> [key, target key, fraction hi, fraction lo]... F7
// The fraction is 14 bits of a semitone above the target key.
std::vector<unsigned char> singleNoteChange(std::vector<std::vector<unsigned char>> entries)
{
  std::vector<unsigned char> msg = {0xF0, 0x7F, 0x7F, 0x08, 0x02, 0x00, (unsigned char)entries.size()};
  for (auto& e : entries) msg.insert(msg.end(), e.begin(), e.end());
  msg.push_back(0xF7);
  return msg;
}
}  // namespace

TEST_CASE("MTSTuning: without a master every key is 12-ET and nothing is filtered", "[mts]")
{
  MTSTuning t;
  if (masterRunning(t)) return;

  t.pollMaster();
  CHECK_FALSE(t.hasMaster());
  CHECK(std::string(t.scaleName()) == "12-TET");

  const int channels[] = {MTSTuning::kNoChannel, 0, 15};
  for (int ch : channels)
  {
    for (int note = 0; note < 128; ++note)
    {
      CHECK(t.retuneSemitones((float)note, ch) == 0.f);
      CHECK_FALSE(t.shouldFilterNote(note, ch));
    }
  }
}

TEST_CASE("MTSTuning: retuning lerps in semitones between adjacent keys", "[mts]")
{
  MTSTuning t;
  if (masterRunning(t)) return;

  // key 61 sounds as key 62: +1 semitone. key 64 sounds half a semitone sharp.
  auto msg = singleNoteChange({{61, 62, 0x00, 0x00}, {64, 64, 0x40, 0x00}});
  t.parseSysEx(msg.data(), (int)msg.size());

  CHECK(t.retuneSemitones(60.f, -1) == Approx(0.0).margin(1e-6));
  CHECK(t.retuneSemitones(61.f, -1) == Approx(1.0));
  CHECK(t.retuneSemitones(62.f, -1) == Approx(0.0).margin(1e-6));
  CHECK(t.retuneSemitones(60.25f, -1) == Approx(0.25));
  CHECK(t.retuneSemitones(60.75f, -1) == Approx(0.75));
  CHECK(t.retuneSemitones(61.5f, -1) == Approx(0.5));
  // 0x40 0x00 = 8192 of 16383 fourteenths of a semitone
  CHECK(t.retuneSemitones(64.f, -1) == Approx(8192.0 / 16383.0).margin(1e-5));
}

TEST_CASE("MTSTuning: a bulk dump names the scale", "[mts]")
{
  MTSTuning t;
  if (masterRunning(t)) return;

  // MIDI Tuning Standard bulk dump reply:
  // F0 7E <device> 08 01 <program> <16-char name> [target key, fraction hi, fraction lo] x 128 <checksum> F7
  std::vector<unsigned char> msg = {0xF0, 0x7E, 0x7F, 0x08, 0x01, 0x00};
  const std::string name = "aalto-test-scale";  // exactly 16 chars
  msg.insert(msg.end(), name.begin(), name.end());
  for (int note = 0; note < 128; ++note)
  {
    unsigned char target = (note == 69) ? 70 : (unsigned char)note;
    msg.push_back(target);
    msg.push_back(0x00);
    msg.push_back(0x00);
  }
  msg.push_back(0x00);  // checksum, not verified by the client
  msg.push_back(0xF7);
  t.parseSysEx(msg.data(), (int)msg.size());

  CHECK(std::string(t.scaleName()) == "aalto-test-scale");
  CHECK(t.retuneSemitones(68.f, -1) == Approx(0.0).margin(1e-6));
  CHECK(t.retuneSemitones(69.f, -1) == Approx(1.0));
}

TEST_CASE("MTSTuning: notes outside 0..127 clamp and NaN retunes by zero", "[mts]")
{
  MTSTuning t;
  if (masterRunning(t)) return;

  // key 0 up a semitone, key 127 down a semitone
  auto msg = singleNoteChange({{0, 1, 0x00, 0x00}, {127, 126, 0x00, 0x00}});
  t.parseSysEx(msg.data(), (int)msg.size());

  CHECK(t.retuneSemitones(0.f, -1) == Approx(1.0));
  CHECK(t.retuneSemitones(-3.f, -1) == Approx(1.0));
  CHECK(t.retuneSemitones(127.f, -1) == Approx(-1.0));
  CHECK(t.retuneSemitones(127.5f, -1) == Approx(-1.0));
  CHECK(t.retuneSemitones(200.f, -1) == Approx(-1.0));
  CHECK(t.retuneSemitones(std::numeric_limits<float>::quiet_NaN(), -1) == 0.f);
  CHECK_FALSE(t.shouldFilterNote(-1, -1));
  CHECK_FALSE(t.shouldFilterNote(128, -1));
}
