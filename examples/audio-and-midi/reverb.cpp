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

// log projection for decay parameter
constexpr float kDecayLo = 0.8, kDecayHi = 20;
Projection unityToDecay(projections::unityToLogParam({kDecayLo, kDecayHi}));

// Delay times in seconds, at a size setting of 1.0. Both the buffer sizing in
// setupParams() and the per-block delay requests in processBlock() read these,
// so they cannot drift apart.
constexpr int kNumAllpasses{10};
constexpr float kMaxSizeScale{2.0f};
constexpr float kApDelaySeconds[kNumAllpasses]{
    0.00476f, 0.00358f, 0.00973f, 0.00830f, 0.029f,
    0.021f,   0.078f,   0.090f,   0.111f,   0.096f};
constexpr float kMainDelaySecondsL{0.0313f};
constexpr float kMainDelaySecondsR{0.0371f};

struct AaltoverbState
{
  // parameter smoothers
  LinearGlide mSmoothFeedback;
  LinearGlide mSmoothDelay;

  // reverb machinery
  Allpass< PitchbendableDelay > mAp1, mAp2, mAp3, mAp4;
  Allpass< PitchbendableDelay > mAp5, mAp6, mAp7, mAp8, mAp9, mAp10;
  PitchbendableDelay mDelayL, mDelayR;

  // indexed access so the sizing loop can walk the allpasses
  Allpass< PitchbendableDelay >* allpass(int i)
  {
    Allpass< PitchbendableDelay >* aps[]{&mAp1, &mAp2, &mAp3, &mAp4, &mAp5,
                                         &mAp6, &mAp7, &mAp8, &mAp9, &mAp10};
    return aps[i];
  }

  // feedback storage
  SignalBlock mvFeedbackL, mvFeedbackR;
};

void initializeReverb(AudioContext* ctx, AaltoverbState* r)
{
  float sr = ctx->getSampleRate();
  
  // set fixed parameters for reverb
  r->mSmoothFeedback.setGlideTimeInSamples(0.1f*sr);
  r->mSmoothDelay.setGlideTimeInSamples(0.1f*sr);
  
  // set allpass filter coefficients
  r->mAp1.mGain = 0.75f;
  r->mAp2.mGain = 0.70f;
  r->mAp3.mGain = 0.625f;
  r->mAp4.mGain = 0.625f;
  r->mAp5.mGain = r->mAp6.mGain = 0.7f;
  r->mAp7.mGain = r->mAp8.mGain = 0.6f;
  r->mAp9.mGain = r->mAp10.mGain = 0.5f;
  
  // Allocate delay memory for this sample rate. The delay times below scale
  // with sr, so sizing from constants only works up to whatever rate those
  // constants were picked for -- past it the requested delay exceeds the
  // buffer and the delay saturates, shortening the reverb instead of
  // lengthening it. Sizing from the same numbers the process loop asks for
  // is what keeps the two in agreement.
  for (int i = 0; i < kNumAllpasses; ++i)
  {
    r->allpass(i)->setMaxDelayInSamples(kApDelaySeconds[i] * kMaxSizeScale * sr);
  }
  r->mDelayL.setMaxDelayInSamples(kMainDelaySecondsL * kMaxSizeScale * sr);
  r->mDelayR.setMaxDelayInSamples(kMainDelaySecondsR * kMaxSizeScale * sr);
}

// processBlock() does all of the audio processing, in SignalBlock-sized chunks.
// It is called every time a new buffer of audio is needed.
void processBlock(AudioContext* ctx, AaltoverbState* r)
{
  float sr = ctx->getSampleRate();
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
  SignalBlock vt1 = max(kApDelaySeconds[0]*delayParamInSamples, vMin);
  SignalBlock vt2 = max(kApDelaySeconds[1]*delayParamInSamples, vMin);
  SignalBlock vt3 = max(kApDelaySeconds[2]*delayParamInSamples, vMin);
  SignalBlock vt4 = max(kApDelaySeconds[3]*delayParamInSamples, vMin);
  SignalBlock vt5 = max(kApDelaySeconds[4]*delayParamInSamples, vMin);
  SignalBlock vt6 = max(kApDelaySeconds[5]*delayParamInSamples, vMin);
  SignalBlock vt7 = max(kApDelaySeconds[6]*delayParamInSamples, vMin);
  SignalBlock vt8 = max(kApDelaySeconds[7]*delayParamInSamples, vMin);
  SignalBlock vt9 = max(kApDelaySeconds[8]*delayParamInSamples, vMin);
  SignalBlock vt10 = max(kApDelaySeconds[9]*delayParamInSamples, vMin);

  // sum stereo inputs and diffuse with four allpass filters in series
  SignalBlock monoInput = (ctx->inputs[0] + ctx->inputs[1]);
  SignalBlock diffusedInput = r->mAp4(r->mAp3(r->mAp2(r->mAp1(monoInput, vt1), vt2), vt3), vt4);

  // get delay times in samples, subtracting the constant delay of one SignalBlock and clamping to zero
  SignalBlock vDelayTimeL = max(kMainDelaySecondsL*delayParamInSamples - vMin, SignalBlock(0.f));
  SignalBlock vDelayTimeR = max(kMainDelaySecondsR*delayParamInSamples - vMin, SignalBlock(0.f));

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

  // make a context and run the audio task.
  AudioContext ctx(kInputChannels, kOutputChannels);
  AudioTask reverbExample(&ctx, processBlock, &r);
  initializeReverb(&ctx, &r);
  
  return reverbExample.runConsoleApp();
}
