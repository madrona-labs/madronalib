// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// AudioTask: adaptor from RtAudio's main loop to madronalib vector processing

#pragma once

#include "MLSignalProcessor.h"
#include "MLAudioContext.h"
#include "mldsp.h"


namespace ml
{


class AudioDevice
{
public:
  AudioDevice();
  ~AudioDevice();

  int getOutputSampleRate();
  
private:
  struct Impl;
  std::unique_ptr<Impl> pImpl;
};

}  // namespace ml
