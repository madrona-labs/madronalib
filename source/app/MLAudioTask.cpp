//
// Created by Randy Jones on 2/21/25.
//

#include "MLPlatform.h"
#include "MLAudioContext.h"
#include "MLAudioTask.h"
#include "MLTerminalDevice.h"

namespace ml
{

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
    
    std::cout << "\nRunning ... press any key to quit.\n";
    terminalDevice::waitForConsoleKeyPress();
    
    stopAudio();
  }
  
  return 0;
}


}  // namespace ml
