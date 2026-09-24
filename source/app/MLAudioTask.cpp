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

int AudioTask::runConsoleApp()
{
  if (processData.processContext->getSampleRate())
  {
    devs.startAudioDevice();
    
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
    
    devs.stopAudioDevice();
  }
  
  return 0;
}

bool AudioTask::hasQuit() const
{
  return processData.hasQuit;
}

}  // namespace ml
