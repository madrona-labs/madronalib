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


struct AudioProcessData
{
  std::atomic<bool> hasQuit{false};
  AudioContext* processContext{nullptr};
  std::function<void(AudioContext*)> processFn;
};

class AudioTask
{
  static constexpr int kMaxBlockSize{4096};
  
public:
  template<typename State>
  AudioTask(AudioContext* ctx, void(*fn)(AudioContext*, State*), State* state)
  {
    processData.processContext = ctx;
    processData.processFn = [fn, state](AudioContext* c) { fn(c, state); };
    

  }
  
  ~AudioTask();
  
  int startAudio();
  void stopAudio();
  int runConsoleApp();
  bool hasQuit() const;
  
private:
  //RtAudio adac;
  AudioDevice audioDevice;
  AudioProcessData processData;
};

}  // namespace ml
