// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#include "MLAudioTask.h"

namespace ml
{

bool AudioTask::hasQuit() const
{
  return processData.hasQuit;
}

}  // namespace ml
