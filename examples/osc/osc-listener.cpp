// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2025 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// Simple OSC listener example - prints received OSC messages to console.
// Uses madronalib's AudioTask for consistent structure with other examples.
//
// Usage: osc-listener [port]
//   port: UDP port to listen on (default: 8002)

#include <iostream>
#include <cstdlib>

#include "madronalib.h"
#include "MLOSCReceiver.h"
#include "MLAudioTask.h"

using namespace ml;

constexpr int kInputChannels = 0;
constexpr int kOutputChannels = 1;
constexpr int kSampleRate = 48000;

void printValue(const Value& v)
{
  switch (v.getType())
  {
    case Value::kFloat:
      std::cout << v.getFloatValue() << " (float)";
      break;
    case Value::kInt:
      std::cout << v.getIntValue() << " (int)";
      break;
    case Value::kText:
      std::cout << "\"" << v.getTextValue() << "\" (string)";
      break;
    case Value::kBlob:
      std::cout << "[blob " << v.size() << " bytes]";
      break;
    case Value::kFloatArray:
      {
        std::cout << "[";
        const float* arr = v.getFloatArrayPtr();
        size_t len = v.getFloatArraySize();
        for (size_t i = 0; i < len; ++i)
        {
          if (i > 0) std::cout << ", ";
          std::cout << arr[i];
        }
        std::cout << "] (float array)";
      }
      break;
    default:
      std::cout << "(unknown)";
      break;
  }
}

struct OSCListenerState
{
  OSCReceiver receiver;
};

void oscListenerProcess(AudioContext* ctx, void* state)
{
  // OSC messages are handled asynchronously by the receiver's callback.
  // This process function just keeps the AudioTask running.
  // Output silence (AudioTask requires output, but we don't use it)
  ctx->outputs[0] = DSPVector(0.f);
}

int main(int argc, char* argv[])
{
  int port = 8002;

  if (argc > 1)
  {
    port = std::atoi(argv[1]);
    if (port <= 0 || port > 65535)
    {
      std::cerr << "Invalid port number: " << argv[1] << std::endl;
      return 1;
    }
  }

  // Set up state
  OSCListenerState state;

  // Set up message callback to print received messages
  state.receiver.setMessageCallback([](Path address, std::vector<Value> args) {
    std::cout << "/" << address.toText();
    if (!args.empty())
    {
      std::cout << " : ";
      for (size_t i = 0; i < args.size(); ++i)
      {
        if (i > 0) std::cout << ", ";
        printValue(args[i]);
      }
    }
    std::cout << std::endl;
  });

  if (!state.receiver.open(port))
  {
    std::cerr << "Failed to open OSC receiver on port " << port << std::endl;
    return 1;
  }

  std::cout << "Listening for OSC messages on port " << port << std::endl;
  std::cout << "Press any key to quit" << std::endl;
  std::cout << "---" << std::endl;

  // Create audio context and task
  AudioContext ctx(kInputChannels, kOutputChannels, kSampleRate);
  AudioTask task(&ctx, oscListenerProcess, &state);

  // Run until user presses a key
  int result = task.runConsoleApp();

  std::cout << "\nShutting down..." << std::endl;
  state.receiver.close();

  return result;
}
