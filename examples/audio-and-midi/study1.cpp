// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#include "madronalib.h"
#include "mldsp.h"
#include "MLSparkline.h"

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
  
  SineGen<float> modOsc1;
  
  TickGen<float> tick1;
  TickGen<float> tick2;
  ADSR env1;
  
  void init()
  {
    env1.coeffs = ADSR::calcCoeffs(0, 0, 1, 1.0f, 48000);
    publishSignal("ticks", 512, 1, 1, 0);
  }
  
  SignalBlock process(AudioContext* ctx)
  {
    SignalBlock output;
    
    const float sr = ctx->getSampleRate();
    
    
    //auto oscsOut = state->oscs();
    
    // OK auto ticks = state->tick1(RectGen<float>::Params{2.f/sr, 0.01f});
    //  std::array<float, 2> tickParams{2.f/kSampleRate, 0.01f};
    //  auto ticks = state->tick1(tickParams);
    
    SignalBlock tickParams{2.f/sr};
    //std::array<float, 1> tickParams{2.f/sr};
    //float tickParams{2.f/sr};
    auto ticks = tick1(tickParams);
    
    auto envs = env1(ticks);
    
    //  auto oscOut = state->osc1(SawGen<float>::Params{110.f/sr});
    
    SignalBlock modOscOut = modOsc1(257.f/sr);
    SignalBlock osc1Freq = (220.f + modOscOut*100.f)/sr;
    auto oscOut = osc1(osc1Freq);
    
    storePublishedSignal("ticks", ticks, kFramesPerBlock, 1);
 
    // mono output
    return envs*oscOut*kOutputGain;
  }
};


// sineProcess() does all of the audio processing, in SignalBlock-sized chunks.
// It is called every time a new buffer of audio is needed.
void study1Process(AudioContext* ctx, void *untypedProcState)
{
  auto state = static_cast< Study1* >(untypedProcState);
            
  // Running the sine generators makes SignalBlocks as output.
  // The input parameter is omega: the frequency in Hz divided by the sample rate.
  // The output sines are multiplied by the gain.
  ctx->outputs[0] = ctx->outputs[1] = state->process(ctx);
}

int main()
{
  Study1 state;
  AudioContext ctx(kInputChannels, kOutputChannels, kSampleRate);
  AudioTask study1(&ctx, study1Process, &state);
  const float sr = ctx.getSampleRate();

  state.init();
  
  //  prelude: test sparklines
  //float omega{2200.f / sr};
  //SignalBlock omega{2200.f/sr};
  std::array<float, 1> omega{2200.f/sr};

  //float tickParams{2.f/sr};

  auto a = sparkline(state.s1(omega));
  std::cout << "spark: " << a <<  "\n";
  std::cout << "bar: " << a <<  "\n";
  
  
  return study1.runConsoleApp();
}
