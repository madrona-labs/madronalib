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

struct Study1 : public SignalProcessor
{
  // TODO this is not working Bank<SawGen, 8> oscs;
  
  PhasorGen<float>::Params pTest;
  
  
  SawGen<float> s1;
  SawGen<float> s2;
  SawGen<float> osc1;
  
  RectGen<float> tick1;
  ADSR env1;
  
  void init()
  {
    env1.coeffs = ADSR::calcCoeffs(0, 0, 1, 1.0f, 48000);
    
    //publishSignal
  }


};


// sineProcess() does all of the audio processing, in SignalBlock-sized chunks.
// It is called every time a new buffer of audio is needed.
void study1Process(AudioContext* ctx, void *untypedProcState)
{
  auto state = static_cast< Study1* >(untypedProcState);

  const float sr = kSampleRate;
  
  
  //auto oscsOut = state->oscs();
  
  // OK auto ticks = state->tick1(RectGen<float>::Params{2.f/sr, 0.01f});
  //  std::array<float, 2> tickParams{2.f/kSampleRate, 0.01f};
  //  auto ticks = state->tick1(tickParams);
  
  auto ticks = state->tick1(2.f/sr, 0.01f);
  
  std::array<float, 2> tickParamsArray{2.f/sr, 0.01f};
  auto ticks2 = state->tick1(tickParamsArray);
  
  RectGen<float>::Params tickParams{2.f/sr, 0.01f};
  auto ticks3 = state->tick1(tickParams);
  
  auto envs = state->env1(ticks);
  
//  auto oscOut = state->osc1(SawGen<float>::Params{110.f/sr});
  
  auto oscOut = state->osc1(110.f/sr);
                            
                            
  // Running the sine generators makes SignalBlocks as output.
  // The input parameter is omega: the frequency in Hz divided by the sample rate.
  // The output sines are multiplied by the gain.
  ctx->outputs[0] = ctx->outputs[1] = oscOut*kOutputGain;
}

int main()
{
  Study1 state;
  AudioContext ctx(kInputChannels, kOutputChannels, kSampleRate);
  AudioTask study1(&ctx, study1Process, &state);
  
  state.init();
  
  //  prelude: test sparklines
  // SignalBlock omega = 2200.f / kSampleRate;
  //auto a = sparkline(state.s1(omega));
  //std::cout << "spark: " << sparkline(state.s1(omega)) <<  "\n";
  //std::cout << "bar: " << sparkgraph(state.s1(omega)) <<  "\n";
  
  
  return study1.runConsoleApp();
}
