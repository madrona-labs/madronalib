// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#include "MLDSPScale.h"

#include <exception>

// Surge XT's Scala engine (Paul Walker, MIT), vendored in external/tuning-library.
// Only this file includes it: it pulls in <regex> and throws on bad input.
#include "Tunings.h"

namespace ml
{

namespace
{
// tuning-library's log-scaled frequency is log2(freq / MIDI note 0); ours is log2(freq / 440).
constexpr double kLogPitchOffset = 69.0 / 12.0;

// the default mapping: 1/1 on MIDI 69, tuned to 440 Hz (Aalto 1's convention).
constexpr int kDefaultRootNote = 69;
constexpr double kDefaultRootFreq = 440.0;
}  // namespace

void Scale::setDefault()
{
  name_ = "12-equal";
  description_ = "The chromatic equal-tempered scale.";
  for (int i = 0; i < kMLNumNotes; ++i)
  {
    pitches_[i] = (i - kDefaultRootNote) / 12.0;
    mapped_[i] = 1;
  }
}

bool Scale::loadScaleFromString(const std::string& scaleStr, const std::string& mapStr)
{
  try
  {
    Tunings::Scale s = Tunings::parseSCLData(scaleStr);
    Tunings::KeyboardMapping k =
        mapStr.empty()
            ? Tunings::startScaleOnAndTuneNoteTo(kDefaultRootNote, kDefaultRootNote, kDefaultRootFreq)
            : Tunings::parseKBMData(mapStr);
    Tunings::Tuning t(s, k);

    // unmapped keys get a pitch interpolated between their mapped neighbours, so continuous
    // pitch (bend, drift) passes through them the way Surge does.
    Tunings::Tuning ti = t.withSkippedNotesInterpolated();

    for (int i = 0; i < kMLNumNotes; ++i)
    {
      pitches_[i] = ti.logScaledFrequencyForMidiNote(i) - kLogPitchOffset;
      mapped_[i] = t.isMidiNoteMapped(i) ? 1 : 0;
    }
    name_ = s.name;
    description_ = s.description;
    return true;
  }
  catch (const std::exception&)
  {
    setDefault();
    return false;
  }
}

}  // namespace ml
