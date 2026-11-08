// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

#include "MLAudioTask.h"
#include "MLConsoleApp.h"

namespace ml
{

static std::atomic<bool> gQuitFlag{false};

static void signalHandler(int)
{
  gQuitFlag = true;
}

void waitForQuitSignal()
{
  gQuitFlag = false;
  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);

  std::cout << "\nRunning ... press Ctrl+C to quit.\n";

  while (!gQuitFlag)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

int runConsoleApp(AudioTask& task)
{
  AudioContext* ctx = task.processData.processContext;
  if (!ctx || !ctx->getSampleRate()) return 1;

  task.devs.startAudioDevice();

  std::cout << "\nStream latency = " << task.devs.getStreamLatency() << " frames" << std::endl;
  std::cout << "sample rate: " << ctx->getSampleRate() << "\n";

  waitForQuitSignal();

  task.devs.stopAudioDevice();
  return 0;
}

}  // namespace ml
