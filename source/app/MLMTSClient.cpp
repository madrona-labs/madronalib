// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#include "MLMTSClient.h"

#include <algorithm>
#include <cmath>

#include "libMTSClient.h"

namespace ml
{
namespace
{
constexpr int kLastKey = 127;

// libMTS takes a signed char channel: 0-15, or -1 for "unknown / MPE".
inline signed char toChannel(int channel)
{
  return (channel >= 0 && channel <= 15) ? (signed char)channel : (signed char)-1;
}
}  // namespace

MTSTuning::MTSTuning() : client_(MTS_RegisterClient()) { pollMaster(); }

MTSTuning::~MTSTuning() { MTS_DeregisterClient(client_); }

void MTSTuning::pollMaster() { hasMaster_.store(MTS_HasMaster(client_), std::memory_order_relaxed); }

const char* MTSTuning::scaleName() const { return MTS_GetScaleName(client_); }

bool MTSTuning::shouldFilterNote(int note, int channel) const
{
  if (note < 0 || note > kLastKey) return false;
  return MTS_ShouldFilterNote(client_, (char)note, toChannel(channel));
}

float MTSTuning::retuneSemitones(float note, int channel) const
{
  if (std::isnan(note)) return 0.f;
  const float fn = std::min(std::max(note, 0.f), (float)kLastKey);
  const int k0 = (int)fn;
  const int k1 = std::min(k0 + 1, kLastKey);
  const double frac = fn - (double)k0;
  const signed char ch = toChannel(channel);

  const double r0 = MTS_RetuningInSemitones(client_, (char)k0, ch);
  if (frac == 0.0 || k1 == k0) return (float)r0;
  const double r1 = MTS_RetuningInSemitones(client_, (char)k1, ch);
  return (float)(r0 + frac * (r1 - r0));
}

void MTSTuning::parseSysEx(const unsigned char* data, int len) { MTS_ParseMIDIDataU(client_, data, len); }
}  // namespace ml
