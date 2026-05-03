// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#include "madronalib.h"

using namespace ml;

constexpr int kInputChannels = 0;
constexpr int kOutputChannels = 2;
constexpr float kOutputGain = 0.1f;

struct SineExampleState
{
  SineGen<float> s1, s2;
};

// sineProcess() does all of the audio processing, in SignalBlock-sized chunks.
// It is called every time a new buffer of audio is needed.
void sineProcess(AudioContext* ctx, SineExampleState *state)
{
  // Running the sine generators makes SignalBlocks as output.
  // The input parameter is omega: the frequency in Hz divided by the sample rate.
  // The output sines are multiplied by the gain.
  float sr = ctx->getSampleRate();
  ctx->outputs[0] = state->s1(220.f/sr)*kOutputGain;
  ctx->outputs[1] = state->s2(275.f/sr)*kOutputGain;
  
  // TEMP
  static int counter{0};
  counter += kFramesPerBlock;
  if (counter > 48000)
  {
    std::cout << "HEY! " << ctx->outputs[0][0] << "\n";
  }
}

int main()
{
  SineExampleState state;
  AudioContext ctx(kInputChannels, kOutputChannels);
  AudioTask sineExample(&ctx, sineProcess, &state);
  return sineExample.runConsoleApp();
}
