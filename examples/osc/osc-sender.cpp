// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2025 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// Clocked OSC sender example - sends a counter every quarter note.
// Uses madronalib's AudioTask and TickGen for sample-accurate timing.
//
// Usage: osc-sender [host] [port] [bpm] [address]
//   host:    destination host (default: 127.0.0.1)
//   port:    destination UDP port (default: 8001)
//   bpm:     tempo in beats per minute (default: 120)
//   address: OSC address to send to (default: /beat)

#include <iostream>
#include <cstdlib>

#include "madronalib.h"
#include "MLOSCSender.h"
#include "MLAudioTask.h"

using namespace ml;

constexpr int kInputChannels = 0;
constexpr int kOutputChannels = 1;
constexpr int kSampleRate = 48000;

struct OSCSenderState
{
  OSCSender sender;
  TickGen tickGen;
  Path oscAddress;
  int counter{0};
  float cyclesPerSample{0};
};

void oscSenderProcess(AudioContext* ctx, void* state)
{
  auto* s = static_cast<OSCSenderState*>(state);

  // Generate ticks at quarter-note rate
  DSPVector ticks = s->tickGen(DSPVector(s->cyclesPerSample));

  // Scan for triggers and send OSC
  for (int i = 0; i < kFloatsPerDSPVector; ++i)
  {
    if (ticks[i] > 0.5f)
    {
      s->sender.send(s->oscAddress, Value(s->counter));
      std::cout << "/" << s->oscAddress.toText() << " " << s->counter << std::endl;
      s->counter++;
    }
  }

  // Output silence (AudioTask requires output, but we don't use it)
  ctx->outputs[0] = DSPVector(0.f);
}

int main(int argc, char* argv[])
{
  const char* host = "127.0.0.1";
  int port = 8001;
  float bpm = 120.0f;
  const char* address = "/beat";

  // Parse command line arguments
  if (argc > 1) host = argv[1];
  if (argc > 2)
  {
    port = std::atoi(argv[2]);
    if (port <= 0 || port > 65535)
    {
      std::cerr << "Invalid port number: " << argv[2] << std::endl;
      return 1;
    }
  }
  if (argc > 3)
  {
    bpm = std::atof(argv[3]);
    if (bpm <= 0 || bpm > 1000)
    {
      std::cerr << "Invalid BPM (must be 1-1000): " << argv[3] << std::endl;
      return 1;
    }
  }
  if (argc > 4) address = argv[4];

  // Set up state
  OSCSenderState state;

  if (!state.sender.open(host, port))
  {
    std::cerr << "Failed to open OSC sender to " << host << ":" << port << std::endl;
    return 1;
  }

  // Skip leading '/' from address
  state.oscAddress = runtimePath(address + 1);

  // Calculate cycles per sample for quarter notes at given BPM
  // BPM / 60 = beats per second
  // beats per second / sample rate = cycles per sample
  state.cyclesPerSample = (bpm / 60.0f) / kSampleRate;

  std::cout << "Sending OSC to " << host << ":" << port << std::endl;
  std::cout << "Address: " << address << std::endl;
  std::cout << "Tempo: " << bpm << " BPM" << std::endl;
  std::cout << "Press any key to quit" << std::endl;
  std::cout << "---" << std::endl;

  // Create audio context and task
  AudioContext ctx(kInputChannels, kOutputChannels, kSampleRate);
  AudioTask task(&ctx, oscSenderProcess, &state);

  // Run until user presses a key
  return task.runConsoleApp();
}
