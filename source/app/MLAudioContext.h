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
using SignalProcessFn = void (*)(AudioContext*, void*);

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


  AudioContext(size_t nInputs, size_t nOutputs, int sampleRate);
  ~AudioContext() = default;

  void clear();

  // update everything needed to create a new vector of context signals.
  // startOffset is the start frame of the vector in the host buffer.
  void makeContextSignalsAtOffset(int startOffset);
  
  
  

  void setSampleRate(int r);
  void resizeBuffers(size_t nInputs, size_t nOutputs, size_t maxFrames);

  void setInputPolyphony(int voices) { eventsToSignals.setPolyphony(voices); }
  size_t getInputPolyphony() { return eventsToSignals.getPolyphony(); }

  void updateTime(const double ppqPos, const double bpmIn, bool isPlaying, double sampleRateIn);
  SignalBlock getBeatPhase() { return currentTime.quarterNotesPhase_; }

  void addInputEvent(const Event& e);
  void clearInputEvents() { eventsToSignals.clearEvents(); }

  void setInputPitchBend(float p) { eventsToSignals.setPitchBendInSemitones(p); }
  void setInputMPEPitchBend(float p) { eventsToSignals.setMPEPitchBendInSemitones(p); }
  void setInputGlideTimeInSeconds(float s) { eventsToSignals.setPitchGlideInSeconds(s); }
  void setInputDriftAmount(float d) { eventsToSignals.setDriftAmount(d); }
  void setInputUnison(bool u) { eventsToSignals.setUnison(u); }
  void setInputProtocol(Symbol p) { eventsToSignals.setProtocol(p); }
  void setInputModCC(int p) { eventsToSignals.setModCC(p); }
  const EventsToSignals::Voice& getInputVoice(int n) { return eventsToSignals.getVoice(n); }

  int getNewestInputVoice() { return eventsToSignals.getNewestVoice(); }
  SignalBlock getInputController(size_t n) const;

  double getSampleRate() { return currentTime.sampleRate; }
  const ProcessTime& getTimeInfo() { return currentTime; }

  // clients can access these directly to do processing
  SignalBlockDynamic inputs;
  SignalBlockDynamic outputs;
  
  void process(const float** inputs, float** outputs, int nFrames,
               SignalProcessFn processFn, void* pState);
  

 private:
  ProcessTime currentTime;
  
  ml::EventsToSignals eventsToSignals;
  
  // buffers containing audio to / from outside world, in bigger chunks
  std::vector<ml::DSPBuffer> inputBuffers_;
  std::vector<ml::DSPBuffer> outputBuffers_;
  
  // max chunk size for outside I/O
  size_t maxFrames_{kMaxIOFramesDefault};
  
  // samples accumulated since the last process call.
  // used to remap event times from host-buffer-relative to internal timeline.
  int samplesAccumulated_{0};

};


}  // namespace ml

