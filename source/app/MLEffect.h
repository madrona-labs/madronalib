// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2025 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#pragma once

#include "MLSignalProcessor.h"
#include "MLAudioContext.h"
#include "mldsp.h"

namespace ml
{

// Effect: Base class for audio effects
// - Default implementation: multichannel passthrough
// - Override SignalProcessor::processVector() for processing
// - Override Actor::onMessage() for setting parameters and
//   anything else we want our View to control
// - Infer number of i/o channels from SignalBlockDynamic sizes

class Effect : public SignalProcessor, public Actor {
public:
  Effect() = default;
  virtual ~Effect() = default;

  // Default implementation: multichannel passthrough
  void processVector(const SignalBlockDynamic& inputs,
                    SignalBlockDynamic& outputs,
                    AudioContext* stateData) override {
    int numInputs = inputs.size();
    int numOutputs = outputs.size();

    // Copy inputs to outputs (as many channels as possible)
    int channelsToCopy = std::min(numInputs, numOutputs);
    for (int i = 0; i < channelsToCopy; ++i) {
      outputs[i] = inputs[i];
    }

    // Zero any extra output channels
    for (int i = channelsToCopy; i < numOutputs; ++i) {
      outputs[i] = SignalBlock{0.f};
    }
  }
  
  void onMessage(ml::Message m) override {
    if (head(m.address) == "set_param") {
      setParamFromNormalizedValue(ml::tail(m.address), m.value.getFloatValue());
    }
  }
};

} // namespace ml
