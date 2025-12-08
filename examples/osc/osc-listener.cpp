// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2025 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// Simple OSC listener example - prints received OSC messages to console.
// Usage: osc-listener [port]
//   port: UDP port to listen on (default: 8002)

#include <iostream>
#include <csignal>
#include <atomic>

#include "MLOSCReceiver.h"

using namespace ml;

std::atomic<bool> running{true};

void signalHandler(int)
{
  running = false;
}

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

  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);

  OSCReceiver receiver;

  receiver.setMessageCallback([](Path address, std::vector<Value> args) {
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

  if (!receiver.open(port))
  {
    std::cerr << "Failed to open OSC receiver on port " << port << std::endl;
    return 1;
  }

  std::cout << "Listening for OSC messages on port " << port << std::endl;
  std::cout << "Press Ctrl+C to quit" << std::endl;
  std::cout << "---" << std::endl;

  while (running)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  std::cout << "\nShutting down..." << std::endl;
  receiver.close();

  return 0;
}
