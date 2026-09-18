// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#pragma once

#include <array>
#include <string>

#include "MLDSPMath.h"

namespace ml
{

// number of notes to calculate. While only 0-127 are needed for MIDI notes,
// the higher ones are used in Virta to quantize partials.
const int kMLNumNotes = 256;

// A musical scale in Scala .scl / .kbm form, converted to a table of log pitch per note.
//
// Parsing (loadScaleFromString) uses Surge XT's tuning-library and allocates: call it
// from the main thread. The lookups read a finished table and are safe on the audio thread.
//
// Default: 12-tone equal temperament with 1/1 on MIDI note 69 = 440 Hz. A .scl loaded
// without a .kbm keeps that root, so existing Aalto patches keep their pitch. With a .kbm
// the frequencies equal Surge XT's for the same two files.
class Scale
{
 public:
  // The finished lookup table: log pitch per note and whether the mapping assigns the
  // note a degree. Trivially copyable so a plugin can parse on the main thread and hand
  // the audio thread a finished table (through a blob parameter, say).
  struct Table
  {
    // log pitch for each integer note number, log2(freq / 440 Hz).
    std::array<double, kMLNumNotes> pitches;
    // 1 if the keyboard mapping assigns the note a scale degree.
    std::array<unsigned char, kMLNumNotes> mapped;
  };

  Scale() { setDefault(); }
  ~Scale() = default;

  // load a scale from Scala text along with an optional keyboard mapping.
  // returns false and falls back to 12-ET if either text is rejected.
  bool loadScaleFromString(const std::string& scaleStr, const std::string& mapStr = "");

  // return the pitch of the given fractional note in log pitch (1.0 per octave) space with
  // 440.0 Hz = 0. Fractional notes interpolate linearly in log pitch between the two
  // adjacent integer notes, as Surge does. Notes are clamped to [0, kMLNumNotes - 1].
  float noteToLogPitch(float note) const
  {
    if (ml::isNaN(note)) return 0.f;

    float fn = ml::clamp(note, 0.f, (float)(kMLNumNotes - 1));
    int i = (int)fn;
    double fracPart = fn - (double)i;
    double p0 = table_.pitches[i];
    double p1 = (i < kMLNumNotes - 1) ? table_.pitches[i + 1] : p0;
    return (float)(p0 + fracPart * (p1 - p0));
  }

  // true if the keyboard mapping assigns a scale degree to the note. Unmapped notes still
  // have a pitch (interpolated from their mapped neighbours); a synth may choose to
  // filter them at note-on instead.
  bool isNoteMapped(int note) const
  {
    if ((note < 0) || (note >= kMLNumNotes)) return false;
    return table_.mapped[note] != 0;
  }

  // return log pitch of the note of the current scale just below the input.
  float quantizePitch(float a) const
  {
    float r = 0.f;
    for (int i = kMLNumNotes - 1; i > 0; i--)
    {
      float p = (float)table_.pitches[i];
      if (p <= a)
      {
        r = p;
        break;
      }
    }
    return r;
  }

  // return log pitch of the note of the current scale closest to the input.
  float quantizePitchNearest(float a) const
  {
    float fLower{0};
    float fHigher{0};
    int lowerIdx{0};
    for (int i = kMLNumNotes - 1; i > 0; i--)
    {
      float p = (float)table_.pitches[i];
      if (p <= a)
      {
        fLower = p;
        if (i < kMLNumNotes - 1)
        {
          fHigher = (float)table_.pitches[i + 1];
        }
        lowerIdx = i;
        break;
      }
    }

    if (lowerIdx == kMLNumNotes - 1)
    {
      return fLower;
    }
    else if (lowerIdx <= 0)
    {
      return (float)table_.pitches[0];
    }

    float d1 = (a - fLower);
    float d2 = (fHigher - a);
    if (d1 < d2)
    {
      return fLower;
    }
    else
    {
      return fHigher;
    }
  }

  void setName(const std::string& nameStr) { name_ = nameStr; }
  void setDescription(const std::string& descStr) { description_ = descStr; }
  const std::string& getName() const { return name_; }
  const std::string& getDescription() const { return description_; }

  // the table alone: name and description stay with the parser.
  const Table& getTable() const { return table_; }
  void setTable(const Table& t) { table_ = t; }

 private:
  // 12-ET, 1/1 on MIDI 69 = 440 Hz, every note mapped. No parsing, no allocation.
  void setDefault();

  std::string name_;
  std::string description_;

  Table table_;
};

}  // namespace ml
