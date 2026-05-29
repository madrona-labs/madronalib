//
// Created by Randy Jones on 2/21/25.
//

#include <csignal>
#include <atomic>
#include <thread>
#include <chrono>

#include "MLPlatform.h"
#include "MLAudioContext.h"
#include "MLAudioTask.h"

namespace ml
{

static std::atomic<bool> gQuitFlag{false};

static void signalHandler(int)
{
  gQuitFlag = true;
}

AudioTask::~AudioTask()
{
}

int AudioTask::startAudio()
{
  auto deviceSampleRate = devs.startAudio(processData);
  processData.processContext->setSampleRate(deviceSampleRate);
  
  return 1;
}

void AudioTask::stopAudio()
{
  devs.stopAudio();
  processData.hasQuit = true;
}

bool AudioTask::hasQuit() const
{
  return processData.hasQuit;
}

int AudioTask::runConsoleApp()
{
  if (startAudio())
  {
    std::cout << "\nStream latency = " << devs.getStreamLatency() << " frames" << std::endl;
    std::cout << "sample rate: " << processData.processContext->getSampleRate() << "\n";
    
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    gQuitFlag = false;

    std::cout << "\nRunning ... press Ctrl+C to quit.\n";

    while (!gQuitFlag)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    stopAudio();
  }
  
  return 0;
}


}  // namespace ml
