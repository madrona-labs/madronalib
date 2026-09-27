//
// Created by Randy Jones on 2/27/25.
//

#pragma once

#include "MLDSPOps.h"
#include "MLEventsToSignals.h"

using namespace ml;
namespace ml
{

// AudioContext: where our signal processors meet the rest of the world.
// an AudioContext defines the sample rate and provides audio and event I/O.
class AudioContext;

using MainInputs = const SignalBlockDynamic&;
using MainOutputs = SignalBlockDynamic&;

constexpr size_t kMaxIOFramesDefault{4096};

class AudioContext final
{
 public:
  // AudioContext::ProcessTime maintains the current time in a DSP process and can track
  // the time in the host application if there is one.
  class ProcessTime
  {
   public:
    ProcessTime() = default;
    ~ProcessTime() = default;

    // Set the time and bpm. The time refers to the start of the current engine processing block.
    void setTimeAndRate(const double ppqPos, const double bpmIn, bool isPlaying,
                        double sampleRateIn);

    // clear state
    void clear();

    void makeTimeSignals();

    // externally readable values 
    SignalBlock quarterNotesPhase_;

    double bpm{0};
    double sampleRate{0};
    uint64_t samplesSinceStart{0};

   private:
    float omega_{0};
    bool playing1_{false};
    bool active1_{false};
    double dpdt_{0};
    size_t samplesSincePreviousTime_{0};
    double ppqPos1_{-1.};
    double ppqPhase1_{0};
  };
  
  AudioContext(size_t nInputs, size_t nOutputs, size_t nEventChannels = 1);
  ~AudioContext() = default;

  void clear();

  void setSampleRate(double r);
  double getSampleRate() { return currentTime.sampleRate; }

  // default is one event channel -this lets existing code work after adding multiple channels
  void resizeBuffers(size_t nInputs, size_t nOutputs, size_t maxFrames, size_t nEventChannels = 1);

  void updateTime(const double ppqPos, const double bpmIn, bool isPlaying, double sampleRateIn);
  SignalBlock getBeatPhase() { return currentTime.quarterNotesPhase_; }
  const ProcessTime& getTimeInfo() { return currentTime; }

  void setInputPolyphony(int voices, int chan = 0) { eventsToSignals_[chan].setPolyphony(voices); }
  size_t getInputPolyphony(int chan = 0) { return eventsToSignals_[chan].getPolyphony(); }
  
  void clearInputEvents(int chan = 0) { eventsToSignals_[chan].clearEvents(); }
  void addInputEvent(const Event& e, int chan = 0);

  void setInputPitchBend(float p, int chan = 0) { eventsToSignals_[chan].setPitchBendInSemitones(p); }
  void setInputMPEPitchBend(float p, int chan = 0) { eventsToSignals_[chan].setMPEPitchBendInSemitones(p); }
  void setInputGlideTimeInSeconds(float s, int chan = 0) { eventsToSignals_[chan].setPitchGlideInSeconds(s); }
  void setInputDriftAmount(float d, int chan = 0) { eventsToSignals_[chan].setDriftAmount(d); }
  void setInputUnison(bool u, int chan = 0) { eventsToSignals_[chan].setUnison(u); }
  void setInputProtocol(Symbol p, int chan = 0) { eventsToSignals_[chan].setProtocol(p); }
  void setInputModCC(int p, int chan = 0) { eventsToSignals_[chan].setModCC(p); }

  int getNewestInputVoice(int chan = 0) { return eventsToSignals_[chan].getNewestVoice(); }

  // clients can access these directly to do processing
  SignalBlockDynamic inputs;
  SignalBlockDynamic outputs;
  //
  // TODO unify API for audio signals above and events below.
  // make k-rate signals for events.
  //
  // get input voice v of event channel c
  const EventsToSignals::Voice& getInputVoice(int n, int c = 0) const;
  
  // get input controller value
  SignalBlock getInputController(size_t n, int channel = 0) const;

  void process(const float** externalInputs, float** externalOutputs,
                             int externalFrames,
               std::function<void(AudioContext*)> processFn);

 private:
  ProcessTime currentTime;
  
  std::vector< ml::EventsToSignals > eventsToSignals_;
  
  // buffers containing audio to / from outside world, in bigger chunks
  std::vector< ml::DSPBuffer > inputBuffers_;
  std::vector< ml::DSPBuffer > outputBuffers_;
  
  // max chunk size for outside I/O
  size_t maxFramesPerBlock_{kMaxIOFramesDefault};
  
  // samples accumulated since the last process call.
  // used to remap event times from host-buffer-relative to internal timeline.
  int inputSamplesAccumulated_{0};

};


}  // namespace ml

