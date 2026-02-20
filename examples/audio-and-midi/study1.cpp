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
  NoiseGen<float> test1;
  PinkFilter<float> pinker;
  PhasorGen<float> phasor1;
  TickGen<float> tick1;
  PulseGen<float> osc1;
  ADSR env1;
  
  void init(float sr)
  {
    pinker.init(sr);
    env1.coeffs = ADSR::calcCoeffs(0, 0, 1, 1.0f, 48000);
    publishSignal("osc", 512, 1, 1, 0);
    setPublishedSignalsActive(true);
  }
  
  SignalBlock process(AudioContext* ctx)
  {
    const float sr = ctx->getSampleRate();

    auto ticks = tick1(2.f/sr);
    auto envs = env1(ticks);
    
    SignalBlock modOut = phasor1(0.25f/sr);
    SignalBlock osc1Freq = (20.f + modOut*4000.f)/sr;
    SignalBlock osc1Duty {0.5f};
    SignalBlockArray<2> rectParams = concatRows(osc1Freq, osc1Duty);
    auto oscOut = osc1(rectParams);
    
    // store signal from oscillator as "osc"
    // NOTE: downsampling param does not seem to be working
    storePublishedSignal("osc", oscOut, kFramesPerBlock, 1);
    
    // test pink noise
    auto pinkNoise = pinker(test1())*dBToAmp(-40.f);
 
    // mono output
    return envs*oscOut*kOutputGain;
  }
};


// study1Process() does all of the audio processing, in SignalBlock-sized chunks.
// It is called every time a new buffer of audio is needed.
void study1Process(AudioContext* ctx, void *untypedProcState)
{
  auto state = static_cast< Study1* >(untypedProcState);
  ctx->outputs[0] = ctx->outputs[1] = state->process(ctx);
}

int main()
{
  Study1 state;
  AudioContext ctx(kInputChannels, kOutputChannels, kSampleRate);
  AudioTask study1Task(&ctx, study1Process, &state);
  const float sr = ctx.getSampleRate();
  
  state.init(sr);
  
  // very simple background thread for reading signals from state
  std::thread ticker([&]() {
    while (!study1Task.hasQuit())
    {
      SignalBlock osc1Sig;
      auto& signals = state.getPublishedSignals();
      auto framesAvailable = signals["osc"]->getReadAvailable();
      if(framesAvailable > kFramesPerBlock)
      {
        auto& buf = signals["osc"]->buffer_;
        buf.read(osc1Sig.data(), kFramesPerBlock);
        std::cout << sparkline(osc1Sig) << "\n";
      }
      
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  });
  

  auto result = study1Task.runConsoleApp();
  ticker.join();
  return result;
}
