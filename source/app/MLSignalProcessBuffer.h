//
// Created by Randy Jones on 2/27/25.
//

#pragma once

#include "MLAudioContext.h"
#include "MLDSPBuffer.h"
#include "MLDSPOps.h"

using namespace ml;
namespace ml
{
// SignalProcessBuffer: utility class to serve a main loop with varying
// arbitrary chunk sizes, buffer inputs and outputs, and compute DSP in
// DSPVector-sized chunks.

using SignalProcessFn = void (*)(AudioContext*, void*);

class SignalProcessBuffer final
{
  // buffers containing audio to / from outside world, in bigger chunks
  std::vector<ml::DSPBuffer> inputBuffers_;
  std::vector<ml::DSPBuffer> outputBuffers_;

  // max chunk size for outside I/O
  size_t maxFrames_;

  // samples accumulated since the last processVector call.
  // used to remap event times from host-buffer-relative to internal timeline.
  int samplesAccumulated_{0};

 public:
  SignalProcessBuffer(size_t inputs, size_t outputs, size_t maxFrames);
  ~SignalProcessBuffer();

  int getSamplesAccumulated() const { return samplesAccumulated_; }

  void process(const float** inputs, float** outputs, int nFrames, AudioContext* ctx,
               SignalProcessFn processFn, void* pState);
};

}  // namespace ml
