//
// Created by Randy Jones on 2/27/25.
//


#include "MLAudioContext.h"

namespace ml
{

// AudioContext::ProcessTime

// Set the time and bpm. The time refers to the start of the current processing block.
// In a plugin, this should be called before each processing block with the latest info from the
// host. In an app, this can be called only when there are time / rate changes.

void AudioContext::ProcessTime::setTimeAndRate(const double ppqPos, const double bpmIn,
                                               bool isPlaying, double sampleRateIn)
{
  // working around a bug I can't reproduce, so I'm covering all the bases.
  if (((ml::isNaN(ppqPos)) || (ml::isInfinite(ppqPos))) ||
      ((ml::isNaN(bpmIn)) || (ml::isInfinite(bpmIn))))
  {
    // debug << "PluginProcessor::ProcessTime::setTimeAndRate: bad input! \n";
    return;
  }

  sampleRate = sampleRateIn;
  bpm = bpmIn;
  bool active = (ppqPos1_ != ppqPos) && isPlaying;
  bool justStarted = isPlaying && !playing1_;

  double ppqPhase = 0.;
  if (active)
  {
    if (ppqPos > 0.f)
    {
      ppqPhase = ppqPos - floor(ppqPos);
    }
    else
    {
      ppqPhase = ppqPos;
    }

    omega_ = ppqPhase;

    if (justStarted)
    {
      // just start at 0 and don't attempt to match the playhead position.
      // this works well when we start at any 1/4 note.
      // there is still some weirdness when we try to lock onto other 16ths.
      samplesSinceStart = 0;
      omega_ = 0.;
      double dsdt = (1. / sampleRate);
      double minutesPerSample = dsdt / 60.;
      dpdt_ = bpm * minutesPerSample;
    }
    else
    {
      double dPhase = ppqPhase - ppqPhase1_;
      if (dPhase < 0.)
      {
        dPhase += 1.;
      }
      dpdt_ = ml::clamp(dPhase / static_cast<double>(samplesSincePreviousTime_), 0., 1.);
    }
  }
  else
  {
    omega_ = -1.;
    dpdt_ = 0.;
  }

  ppqPos1_ = ppqPos;
  ppqPhase1_ = ppqPhase;
  active1_ = active;
  playing1_ = isPlaying;
  samplesSincePreviousTime_ = 0;
}

void AudioContext::ProcessTime::clear(void)
{
  dpdt_ = 0.;
  active1_ = false;
  playing1_ = false;
}

// generate phasors from the input parameters
void AudioContext::ProcessTime::makeTimeSignals()
{
  for (int n = 0; n < kFramesPerBlock; ++n)
  {
    quarterNotesPhase_[n] = omega_;
    omega_ += dpdt_;
    if (omega_ > 1.f)
    {
      omega_ -= 1.f;
    }
  }
  samplesSincePreviousTime_ += kFramesPerBlock;
  samplesSinceStart += kFramesPerBlock;
}


// AudioContext

AudioContext::AudioContext(size_t nInputs, size_t nOutputs, int rate)
    : inputs(nInputs), outputs(nOutputs)
{
  resizeBuffers(nInputs, nOutputs, kMaxIOFramesDefault);
  setSampleRate(rate);
  clear();
}

void AudioContext::setSampleRate(int r)
{
  currentTime.sampleRate = r;
  eventsToSignals.setSampleRate(r);
}

void AudioContext::resizeBuffers(size_t nInputs, size_t nOutputs, size_t maxFrames)
{
  inputBuffers_.resize(nInputs);
  for (int i = 0; i < nInputs; ++i)
  {
    inputBuffers_[i].resize((int)maxFrames);
  }
  
  outputBuffers_.resize(nOutputs);
  for (int i = 0; i < nOutputs; ++i)
  {
    outputBuffers_[i].resize((int)maxFrames);
  }
}

void AudioContext::clear()
{
  currentTime.clear();
  eventsToSignals.clear();
  
  // add a block of zeros to output buffer. We have a constant one-block delay between input and output.
  SignalBlock emptyBlock(0.f);
  for (auto& buf : outputBuffers_)
  {
    buf.clear();
    buf.write(emptyBlock);
  }
  
  inputSamplesAccumulated_ = 0;
}

// Buffer the external context and provide an internal context for the process function.
// Then run the process function in the internal context as many times are necessary to
// generate externalFrames of output.
void AudioContext::process(const float** externalInputs, float** externalOutputs,
                                  int externalFrames,
                                  SignalProcessFn processFn, void* state)
{
  size_t nInputs = inputBuffers_.size();
  size_t nOutputs = outputBuffers_.size();
  if (nOutputs < 1) return;
  if (!externalOutputs) return;
  if (externalFrames > (int)maxFrames_) return;
  
  // write vectors from external inputs (if any) to inputBuffers
  for (int c = 0; c < nInputs; c++)
  {
    if(externalInputs[c])
    {
      inputBuffers_[c].write(externalInputs[c], externalFrames);
    }
  }
  
  inputSamplesAccumulated_ += externalFrames;
  while (inputSamplesAccumulated_ >= kFramesPerBlock)
  {
    // read one block from each input buffer
    for (int c = 0; c < nInputs; c++)
    {
      inputs[c] = inputBuffers_[c].read();
    }
    
    // generate one block of time / event / controller signals
    currentTime.makeTimeSignals();
    eventsToSignals.makeSignalBlock();
    
    // run the signal processing function
    processFn(this, state);
    
    // write one block to each output buffer
    for (int c = 0; c < nOutputs; c++)
    {
      outputBuffers_[c].write(outputs[c]);
    }
        
    // shift any remaining events in buffer forward and fix accum counter
    eventsToSignals.adjustEventsInBuffer(kFramesPerBlock);
    inputSamplesAccumulated_ -= kFramesPerBlock;
  }
  
  // read from outputBuffers to external outputs
  for(int c = 0; c < nOutputs; c++)
  {
    if (externalOutputs[c])
    {
      outputBuffers_[c].read(externalOutputs[c], externalFrames);
    }
  }
}

SignalBlock AudioContext::getInputController(size_t n) const
{
  return eventsToSignals.getController(n).output;
}

void AudioContext::addInputEvent(const Event& e)
{
  Event adjusted = e;
  adjusted.time += inputSamplesAccumulated_;
  eventsToSignals.addEvent(adjusted);
}

void AudioContext::updateTime(const double ppqPos, const double bpmIn, bool isPlaying,
                              double sampleRateIn)
{
  currentTime.setTimeAndRate(ppqPos, bpmIn, isPlaying, sampleRateIn);
}

}  // namespace ml
