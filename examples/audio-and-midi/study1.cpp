// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#include "madronalib.h"
#include "mldsp.h"

using namespace ml;


constexpr int kInputChannels = 0;
constexpr int kOutputChannels = 2;
constexpr int kSampleRate = 48000;
constexpr float kOutputGain = 0.1f;

struct Study1State
{
  Bank<SawGen, 8> oscs;
  
  SawGen<float> s1;
  SawGen<float> s2;
  SawGen<float> osc1;
  
  TickGen<float> tick1;
};

// sineProcess() does all of the audio processing, in SignalBlock-sized chunks.
// It is called every time a new buffer of audio is needed.
void study1Process(AudioContext* ctx, void *state)
{
  // at the beginning of the main process function we need to cast the void* to
  // the type of our state. Making AudioTask a template would have been an alternative
  // to this but would have added a lot of template code behind the scenes.
  auto procState = static_cast<Study1State*>(state);
   
  //auto oscsOut = procState->oscs();
  
  auto ticks = procState->tick1(1.f/kSampleRate);
  
  
  auto ramp =
  auto oscOut = procState->osc1(freqRamp);
  
  // Running the sine generators makes SignalBlocks as output.
  // The input parameter is omega: the frequency in Hz divided by the sample rate.
  // The output sines are multiplied by the gain.
  ctx->outputs[0] = procState->s1(220.f/kSampleRate)*kOutputGain;
  ctx->outputs[1] = procState->s2(275.f/kSampleRate)*kOutputGain;
}

int main()
{
  Study1State state;
  AudioContext ctx(kInputChannels, kOutputChannels, kSampleRate);
  AudioTask study1(&ctx, study1Process, &state);
  
  //  prelude: test sparklines
  SignalBlock omega = 2200.f / kSampleRate;
  auto a = sparkline(state.s1(omega));
  std::cout << "spark: " << sparkline(state.s1(omega)) <<  "\n";
  std::cout << "bar: " << sparkgraph(state.s1(omega)) <<  "\n";
  return study1.runConsoleApp();
}
