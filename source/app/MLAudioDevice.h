// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#pragma once

#include "MLSignalProcessor.h"
#include "MLAudioContext.h"
#include "mldsp.h"


namespace ml
{


struct AudioProcessData
{
  std::atomic<bool> hasQuit{false};
  AudioContext* processContext{nullptr};
  std::function<void(AudioContext*)> processFn;
};

class AudioDevice
{
public:
  AudioDevice();
  ~AudioDevice();

  int getOutputSampleRate();
  long getStreamLatency();

  int openAudioDevice(const AudioProcessData& processData);
  int startAudioDevice();
  void stopAudioDevice();
  void closeAudioDevice();

private:
  struct Impl;
  std::unique_ptr<Impl> pImpl;
};

}  // namespace ml
