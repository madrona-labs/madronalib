// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// AudioTask: adaptor from RtAudio's main loop to madronalib vector processing

#pragma once

#include "MLSignalProcessor.h"
#include "MLAudioContext.h"
#include "mldsp.h"
#include "MLAudioDevice.h"

namespace ml
{

class AudioTask
{
  
public:
  // on creation, openAudioDevice() is called. If open is successful,
  // getSampleRate() will return the current rate.
  template<typename State>
  AudioTask(AudioContext* ctx, void(*fn)(AudioContext*, State*), State* state)
  {
    if(!ctx) return;
    processData.processContext = ctx;
    processData.processFn = [fn, state](AudioContext* c) { fn(c, state); };
    auto deviceSampleRate = devs.openAudioDevice(processData);
    processData.processContext->setSampleRate(deviceSampleRate);
  }
  
  ~AudioTask()
  {
    devs.closeAudioDevice();
    processData.hasQuit = true;
  }

  int runConsoleApp();
  bool hasQuit() const;
  
private:
  AudioDevice devs;
  AudioProcessData processData;
};

}  // namespace ml
