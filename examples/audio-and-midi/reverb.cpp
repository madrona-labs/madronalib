// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// example of RtAudio wrapping low-level madronalib DSP code.
// The reverb in this example is the Aaltoverb algorithm (madronalabs.com/products/Aaltoverb) without the tone control and some filtering.

#include "MLAudioTask.h"

using namespace ml;

// Mac OS note: need to ask for microphone access if kInputChannels is nonzero!
constexpr int kInputChannels = 2;
constexpr int kOutputChannels = 2;
constexpr int kSampleRate = 48000;

// log projection for decay parameter
constexpr float kDecayLo = 0.8, kDecayHi = 20;
Projection unityToDecay(projections::unityToLogParam({kDecayLo, kDecayHi}));

struct AaltoverbState
{
  // parameter smoothers
  LinearGlide mSmoothFeedback;
  LinearGlide mSmoothDelay;

  // reverb machinery
  Allpass< PitchbendableDelay > mAp1, mAp2, mAp3, mAp4;
  Allpass< PitchbendableDelay > mAp5, mAp6, mAp7, mAp8, mAp9, mAp10;
  PitchbendableDelay mDelayL, mDelayR;

  // feedback storage
  SignalBlock mvFeedbackL, mvFeedbackR;
};

void initializeReverb(AaltoverbState& r)
{
  // set fixed parameters for reverb
  r.mSmoothFeedback.setGlideTimeInSamples(0.1f*kSampleRate);
  r.mSmoothDelay.setGlideTimeInSamples(0.1f*kSampleRate);

  // set allpass filter coefficients
  r.mAp1.mGain = 0.75f;
  r.mAp2.mGain = 0.70f;
  r.mAp3.mGain = 0.625f;
  r.mAp4.mGain = 0.625f;
  r.mAp5.mGain = r.mAp6.mGain = 0.7f;
  r.mAp7.mGain = r.mAp8.mGain = 0.6f;
  r.mAp9.mGain = r.mAp10.mGain = 0.5f;

  // allocate delay memory
  r.mAp1.setMaxDelayInSamples(500.f);
  r.mAp2.setMaxDelayInSamples(500.f);
  r.mAp3.setMaxDelayInSamples(1000.f);
  r.mAp4.setMaxDelayInSamples(1000.f);
  r.mAp5.setMaxDelayInSamples(2600.f);
  r.mAp6.setMaxDelayInSamples(2600.f);
  r.mAp7.setMaxDelayInSamples(8000.f);
  r.mAp8.setMaxDelayInSamples(8000.f);
  r.mAp9.setMaxDelayInSamples(10000.f);
  r.mAp10.setMaxDelayInSamples(10000.f);
  r.mDelayL.setMaxDelayInSamples(3500.f);
  r.mDelayR.setMaxDelayInSamples(3500.f);
}

// processVector() does all of the audio processing, in SignalBlock-sized chunks.
// It is called every time a new buffer of audio is needed.
void processVector(AudioContext* ctx, void *stateData)
{
  AaltoverbState* r = static_cast< AaltoverbState* >(stateData);

  const float sr = kSampleRate;
  const float RT60const = 0.001f;

  // size and decay parameters from 0-1. It will be more interesting to change these over time in some way.
  float sizeU = 0.5f;
  float decayU = 0.5f;

  // generate delay and feedback scalars
  float decayTime = unityToDecay(decayU);
  float decayIterations = decayTime/(sizeU*0.5);
  float feedback = (decayU < 1.0f) ? powf(RT60const, 1.0f/decayIterations) : 1.0f;

  // generate smoothed delay time and feedback gain vectors
  SignalBlock vSmoothDelay = r->mSmoothDelay(sizeU*2.0f);
  SignalBlock vSmoothFeedback = r->mSmoothFeedback(feedback);

  // get the minimum possible delay in samples, which is the length of a SignalBlock.
  SignalBlock vMin(kFramesPerBlock);

  // get smoothed allpass times in samples
  SignalBlock delayParamInSamples = sr*vSmoothDelay;
  SignalBlock vt1 = max(0.00476*delayParamInSamples, vMin);
  SignalBlock vt2 = max(0.00358*delayParamInSamples, vMin);
  SignalBlock vt3 = max(0.00973*delayParamInSamples, vMin);
  SignalBlock vt4 = max(0.00830*delayParamInSamples, vMin);
  SignalBlock vt5 = max(0.029*delayParamInSamples, vMin);
  SignalBlock vt6 = max(0.021*delayParamInSamples, vMin);
  SignalBlock vt7 = max(0.078*delayParamInSamples, vMin);
  SignalBlock vt8 = max(0.090*delayParamInSamples, vMin);
  SignalBlock vt9 = max(0.111*delayParamInSamples, vMin);
  SignalBlock vt10 = max(0.096*delayParamInSamples, vMin);

  // sum stereo inputs and diffuse with four allpass filters in series
  SignalBlock monoInput = (ctx->inputs[0] + ctx->inputs[1]);
  SignalBlock diffusedInput = r->mAp4(r->mAp3(r->mAp2(r->mAp1(monoInput, vt1), vt2), vt3), vt4);

  // get delay times in samples, subtracting the constant delay of one SignalBlock and clamping to zero
  SignalBlock vDelayTimeL = max(0.0313*delayParamInSamples - vMin, SignalBlock(0.f));
  SignalBlock vDelayTimeR = max(0.0371*delayParamInSamples - vMin, SignalBlock(0.f));

  // sum diffused input with feedback, and apply late diffusion of two more allpass filters to each channel
  SignalBlock vTapL = r->mAp7(r->mAp5(diffusedInput + r->mDelayL(r->mvFeedbackL, vDelayTimeL), vt5), vt7);
  SignalBlock vTapR = r->mAp8(r->mAp6(diffusedInput + r->mDelayR(r->mvFeedbackR, vDelayTimeR), vt6), vt8);

  // apply final allpass filter and gain, and store the feedback
  r->mvFeedbackR = r->mAp9(vTapL, vt9)*vSmoothFeedback;
  r->mvFeedbackL = r->mAp10(vTapR, vt10)*vSmoothFeedback;

  // write the stereo outputs
  ctx->outputs[0] = vTapL;
  ctx->outputs[1] = vTapR;
}

int main()
{
  // create and initialize the reverb state.
  AaltoverbState r;
  initializeReverb(r);

  // make a context and run the audio task.
  AudioContext ctx(kInputChannels, kOutputChannels, kSampleRate);
  AudioTask reverbExample(&ctx, processVector, &r);
  return reverbExample.runConsoleApp();
}
