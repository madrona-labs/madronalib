//
// Created by Randy Jones on 2/27/25.
//

#pragma once

#include <memory>

#include "MLDSPOps.h"
#include "MLEventsToSignals.h"
#include "MLQueue.h"

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
    void setTimeAndRate(const double ppqPos, const double bpmIn, bool isPlaying);

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

  void updateTime(const double ppqPos, const double bpmIn, bool isPlaying);
  SignalBlock getBeatPhase() { return currentTime.quarterNotesPhase_; }

  // Optional veto on note-ons, asked before the event reaches a voice: return true to
  // drop it. Set once from the audio thread with a plain function so no allocation is
  // involved; nullptr removes it. Note-offs and all other events always pass. It's currently
  // used with MTS-ESP to prevent unmapped keys from generating notes.
  using NoteOnFilterFn = bool (*)(void* context, const Event& e);
  void setNoteOnFilter(NoteOnFilterFn fn, void* context)
  {
    noteOnFilter_ = fn;
    noteOnFilterContext_ = context;
  }
  const ProcessTime& getTimeInfo() { return currentTime; }

  void setInputPolyphony(int voices, int chan = 0) { eventsToSignals_[chan].setPolyphony(voices); }
  size_t getInputPolyphony(int chan = 0) { return eventsToSignals_[chan].getPolyphony(); }
  
  void clearInputEvents(int chan = 0) { eventsToSignals_[chan].clearEvents(); }

  // Add an event to event channel chan for the next block. Call only from the thread
  // that calls process(): a plugin adds its host's events this way before processing.
  void addInputEvent(const Event& e, int chan = 0);

  // Post an event to event channel chan from another thread, such as a MIDI input
  // callback. Events wait in a lock-free queue and process() moves them into the
  // channel before its first block, where they take effect at frame 0. One posting
  // thread per channel (the queue is single-producer). Returns false if the queue
  // was full and the event was dropped.
  bool postInputEvent(const Event& e, int chan = 0);

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

  // get the control-rate outputs of all voices of event channel c. See VoiceControls.
  const VoiceControls& getInputVoiceControls(int c = 0) const { return eventsToSignals_[c].getVoiceControls(); }

  // get the raw control-rate controller values of event channel c. See ControllerValues.
  const ControllerValues& getInputControllerValues(int c = 0) const { return eventsToSignals_[c].getControllerValues(); }

  size_t getNumEventChannels() const { return eventsToSignals_.size(); }

  // get input controller value
  SignalBlock getInputController(size_t n, int channel = 0) const;

  void process(const float** externalInputs, float** externalOutputs,
                             int externalFrames,
               std::function<void(AudioContext*)> processFn);

 private:
  ProcessTime currentTime;
  
  std::vector< ml::EventsToSignals > eventsToSignals_;

  // events posted from other threads, one queue per event channel. Queues hold
  // atomics so they live behind pointers; the vector is sized in resizeBuffers().
  static constexpr size_t kPostedEventQueueSize{1024};
  std::vector< std::unique_ptr< Queue< Event > > > postedEvents_;
  void drainPostedEvents();

  NoteOnFilterFn noteOnFilter_{nullptr};
  void* noteOnFilterContext_{nullptr};
  
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

