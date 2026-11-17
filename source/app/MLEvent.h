//
// Created by Randy Jones on 2/25/25.
//

#pragma once

#include <cstdint>
#include <iostream>

namespace ml
{

enum EventType
{
  kNull = 0,
  kNoteOn,
  kNoteRetrig,
  kNoteSustain,
  kNoteOff,
  kSustainPedal,  // when sustain pedal is held, key releases generate kNoteSustain events
  kController,
  kPitchBend,
  kNotePressure,
  kChannelPressure,
  kProgramChange,

  // Note expressions: per-note changes addressed to a key, for hosts that send
  // them directly instead of as MPE. sourceIdx is the key and channel is the
  // note's channel, or 0 for any channel. They apply in every protocol.
  kNotePitchOffset,  // value1 is added to the note's pitch, in the units of the note-on's value1
  kNoteZ,            // value1 is the note's pressure, [0, 1]
  kNoteY,            // value1 is the note's timbre, [0, 1]
  kNumEventTypes
};

// Event: something that happens in a performance.
//
struct Event
{
  Event() = default;
  ~Event() = default;

  uint8_t type{kNull};
  uint8_t channel{0};

  // which of multiple things (keys, controls) originated the event?
  uint16_t sourceIdx{0};

  // Onset time in samples from start of current top-level buffer.
  int time{0};

  // float values that have different meanings for different event types.
  float value1{0};
  float value2{0};

  explicit operator bool() const { return type != kNull; }

  static const char* typeNames[kNumEventTypes];
};

inline std::ostream& operator<<(std::ostream& out, const Event& e)
{
  out << "[" << Event::typeNames[e.type] << " " << (int)e.channel << "/" << (int)e.sourceIdx
            << "/" << (int)e.time << " " << e.value1 << ", " << e.value2 << "]";
  return out;
}

}  // namespace ml
