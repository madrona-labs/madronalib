//
// Created by Randy Jones on 2/21/25.
//

#pragma once

#include "MLEvent.h"
#include <vector>
#include <functional>

namespace ml
{
enum MIDIMessageType
{
  kMIDINoteOff = 0,
  kMIDINoteOn = 1,
  kMIDIPolyPressure = 2,
  kMIDIControlChange = 3,
  kMIDIProgramChange = 4,
  kMIDIChannelPressure = 5,
  kMIDIPitchBend = 6
};

using MIDIMessage = std::vector<unsigned char>;

using MIDIMessageHandler = std::function<void(const MIDIMessage&)>;

class MIDIInput
{
 public:
  MIDIInput();
  ~MIDIInput();

  // enumerate available MIDI input ports
  unsigned int getPortCount();
  std::string getPortName(unsigned int portNumber);

  // start processing messages from the input with the given handler function.
  bool start(MIDIMessageHandler handler, unsigned int portNumber = 0);
  void stop();

  std::string getAPIDisplayName();

 private:
  struct Impl;
  std::unique_ptr<Impl> pImpl;
};

// convert a MIDI message into an Event for use with EventsToSignals.
Event MIDIMessageToEvent(const MIDIMessage& message);

}  // namespace ml
