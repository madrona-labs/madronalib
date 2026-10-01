// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// ml::MTSTuning wraps ODDSound's MTS-ESP client (external/mts-esp, ISC) so a
// plugin can follow the tuning of an MTS-ESP master in the same session.
//
// One instance per plugin. Construct and destroy on the main thread: the
// client registers with libMTS and allocates. Everything else follows the
// same practice as Surge XT: retuneSemitones() and shouldFilterNote() are
// called from the audio thread and read libMTS's shared table without
// allocating or locking. pollMaster() asks libMTS whether a master is present
// and stores the answer for hasMaster(), which any thread may read.
//
// With no libMTS installed, or no master running, the client answers from a
// local table that starts at 12-ET and follows MIDI Tuning Standard SysEx
// passed to parseSysEx().
//
// Every call in an instance should use the same channel convention: the note's
// MIDI channel (0-15), or kNoChannel while MPE is on. The client latches
// "multi-channel" from the last channel argument it saw on any call.

#pragma once

#include <atomic>

struct MTSClient;

namespace ml
{
class MTSTuning
{
 public:
  static constexpr int kNoChannel = -1;

  MTSTuning();
  ~MTSTuning();
  MTSTuning(const MTSTuning&) = delete;
  MTSTuning& operator=(const MTSTuning&) = delete;

  // Ask libMTS whether a master is connected. Audio thread, every ~1024 blocks.
  void pollMaster();

  // Result of the last pollMaster(). Any thread.
  bool hasMaster() const { return hasMaster_.load(std::memory_order_relaxed); }

  // Name of the master's scale, or of the local SysEx tuning. Main thread.
  const char* scaleName() const;

  // True when the master leaves this key unmapped: drop the note-on.
  bool shouldFilterNote(int note, int channel) const;

  // Offset in semitones to add to a 12-ET note number. Fractional notes lerp
  // between the two adjacent keys, linear in semitones. Notes clamp to 0..127.
  float retuneSemitones(float note, int channel) const;

  // Feed MIDI Tuning Standard SysEx into the local table (used without a master).
  void parseSysEx(const unsigned char* data, int len);

 private:
  MTSClient* client_;
  std::atomic<bool> hasMaster_{false};
};
}  // namespace ml
