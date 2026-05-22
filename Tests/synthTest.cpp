// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// Compile-time and minimal runtime coverage for the Synth base class in
// source/app/MLSynth.h. The point of this test is to instantiate a Synth
// subclass inside madronalib so that the processVoice/processVector
// override signatures get type-checked by the in-repo build — without
// this, drift in those signatures only surfaces in downstream consumers
// (Aalto, Aaltoverb).

#include "catch.hpp"
#include "MLSynth.h"
#include "MLAudioContext.h"
#include "MLDSPOps.h"
#include "MLEvent.h"

using namespace ml;

namespace {

// Trivial Synth subclass: writes the voice's gate signal into output 0
// and tracks how many times processVoice has been called. We do not
// care about audio fidelity — only that the override chain compiles
// against the current base-class signature and that processVector
// actually dispatches into processVoice.
class TestSynth : public Synth {
public:
  explicit TestSynth(int numVoices = 2) : Synth(numVoices) {}

  void processVoice(int voiceIndex,
                    const EventsToSignals::Voice& voice,
                    const SignalBlockDynamic& inputs,
                    SignalBlockDynamic& outputs,
                    AudioContext* audioContext) override {
    voiceCallCount++;
    lastVoiceIndex = voiceIndex;
    lastSampleRate = audioContext ? audioContext->getSampleRate() : 0.0;

    // Accumulate a constant into output 0 so the test can confirm we ran.
    if (outputs.size() > 0) {
      SignalBlock contribution{1.0f};
      outputs[0] += contribution;
    }
  }

  int voiceCallCount{0};
  int lastVoiceIndex{-1};
  double lastSampleRate{0.0};
};

}  // namespace

TEST_CASE("Synth base class processVector dispatches to processVoice", "[synth]") {
  constexpr int kNumVoices = 2;
  TestSynth synth(kNumVoices);
  synth.setSampleRate(44100.0);

  AudioContext ctx(0, 1);
  ctx.setSampleRate(44100);
  ctx.setInputPolyphony(kNumVoices);

  SignalBlockDynamic inputs(0);
  SignalBlockDynamic outputs(1);
  outputs[0] = SignalBlock{0.f};

  synth.processVector(inputs, outputs, &ctx);

  // isVoiceActive() returns true by default, so every voice should run.
  REQUIRE(synth.voiceCallCount == kNumVoices);
  REQUIRE(synth.getActiveVoiceCount() == kNumVoices);
  REQUIRE(synth.lastSampleRate == Approx(44100.0));

  // Each voice added 1.0 to outputs[0], so every sample should equal kNumVoices.
  for (size_t i = 0; i < kFramesPerBlock; ++i) {
    REQUIRE(outputs[0][i] == Approx(static_cast<float>(kNumVoices)));
  }
}

// TODO: no-op on null AudioContext might be a bad pattern. I don't remember if this is done elsewhere
TEST_CASE("Synth processVector with null AudioContext is a no-op", "[synth]") {
  TestSynth synth(2);
  SignalBlockDynamic inputs(0);
  SignalBlockDynamic outputs(1);
  outputs[0] = SignalBlock{0.5f};

  synth.processVector(inputs, outputs, nullptr);

  REQUIRE(synth.voiceCallCount == 0);
  // outputs untouched
  REQUIRE(outputs[0][0] == Approx(0.5f));
}

TEST_CASE("Synth honors isVoiceActive override", "[synth]") {
  class GatedSynth : public TestSynth {
  public:
    using TestSynth::TestSynth;
    bool isVoiceActive(int voiceIndex,
                       const EventsToSignals::Voice&) override {
      return voiceIndex == 0;
    }
  };

  GatedSynth synth(4);
  AudioContext ctx(0, 1);
  ctx.setSampleRate(44100);
  ctx.setInputPolyphony(4);

  SignalBlockDynamic inputs(0);
  SignalBlockDynamic outputs(1);
  outputs[0] = SignalBlock{0.f};

  synth.processVector(inputs, outputs, &ctx);

  REQUIRE(synth.voiceCallCount == 1);
  REQUIRE(synth.getActiveVoiceCount() == 1);
  REQUIRE(synth.hasActiveVoices());
}
