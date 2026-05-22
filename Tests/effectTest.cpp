// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// Compile-time and minimal runtime coverage for the Effect base class
// in source/app/MLEffect.h. Like synthTest.cpp, the primary goal is to
// instantiate an Effect subclass inside madronalib so processVector's
// override signature is exercised by the in-repo build.

#include "catch.hpp"
#include "MLEffect.h"
#include "MLAudioContext.h"
#include "MLDSPOps.h"

using namespace ml;

namespace {

// Trivial Effect subclass that doubles every sample. Records whether
// processVector was actually called so the test can detect a silently
// missed override.
class TestEffect : public Effect {
public:
  void processVector(const SignalBlockDynamic& inputs,
                     SignalBlockDynamic& outputs,
                     AudioContext* audioContext) override {
    processCallCount++;
    lastContext = audioContext;

    int channels = std::min(inputs.size(), outputs.size());
    for (int i = 0; i < channels; ++i) {
      outputs[i] = inputs[i] * SignalBlock{2.0f};
    }
  }

  int processCallCount{0};
  AudioContext* lastContext{nullptr};
};

}  // namespace

TEST_CASE("Effect base class default processVector is multichannel passthrough",
          "[effect]") {
  Effect effect;

  SignalBlockDynamic inputs(2);
  SignalBlockDynamic outputs(2);
  inputs[0] = SignalBlock{0.25f};
  inputs[1] = SignalBlock{-0.75f};
  outputs[0] = SignalBlock{99.f};
  outputs[1] = SignalBlock{99.f};

  effect.processVector(inputs, outputs, nullptr);

  for (size_t i = 0; i < kFramesPerBlock; ++i) {
    REQUIRE(outputs[0][i] == Approx(0.25f));
    REQUIRE(outputs[1][i] == Approx(-0.75f));
  }
}

TEST_CASE("Effect base class zeroes extra output channels", "[effect]") {
  Effect effect;

  SignalBlockDynamic inputs(1);
  SignalBlockDynamic outputs(2);
  inputs[0] = SignalBlock{0.5f};
  outputs[0] = SignalBlock{99.f};
  outputs[1] = SignalBlock{99.f};

  effect.processVector(inputs, outputs, nullptr);

  for (size_t i = 0; i < kFramesPerBlock; ++i) {
    REQUIRE(outputs[0][i] == Approx(0.5f));
    REQUIRE(outputs[1][i] == Approx(0.0f));
  }
}

TEST_CASE("Effect subclass processVector override runs with AudioContext",
          "[effect]") {
  TestEffect effect;
  effect.setSampleRate(48000.0);

  AudioContext ctx(2, 2);
  ctx.setSampleRate(48000);

  SignalBlockDynamic inputs(2);
  SignalBlockDynamic outputs(2);
  inputs[0] = SignalBlock{0.5f};
  inputs[1] = SignalBlock{-0.5f};

  effect.processVector(inputs, outputs, &ctx);

  REQUIRE(effect.processCallCount == 1);
  REQUIRE(effect.lastContext == &ctx);
  for (size_t i = 0; i < kFramesPerBlock; ++i) {
    REQUIRE(outputs[0][i] == Approx(1.0f));
    REQUIRE(outputs[1][i] == Approx(-1.0f));
  }
}
