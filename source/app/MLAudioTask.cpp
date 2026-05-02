//
// Created by Randy Jones on 2/21/25.
//

#include "MLPlatform.h"
#include "MLAudioContext.h"
#include "MLAudioTask.h"

namespace ml
{

#ifdef ML_MAC
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

void waitForConsoleKeyPress()
{
  char ch{EOF};
  struct termios oldt, newt;
  int oldf;
  
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  
  while (ch == EOF)
  {
    ch = getchar();
    std::this_thread::sleep_for(10ms);
  }
  
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
}

#endif  // ML_MAC
#ifdef ML_WINDOWS
#include <conio.h>

char keyPressedAsync()
{
  for (int i = 0x07; i < 256; ++i)
  {
    if (GetAsyncKeyState(i) & 0x8000)
    {
      return i;
    }
  }
  return false;
}

void waitForConsoleKeyPress()
{
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursorInfo;
  GetConsoleCursorInfo(hConsole, &cursorInfo);
  cursorInfo.bVisible = false;
  SetConsoleCursorInfo(hConsole, &cursorInfo);
  
  while (true)
  {
    if (_kbhit())
    {
      int ch = _getch();
      break;
    }
    std::this_thread::sleep_for(10ms);
  }
  
  cursorInfo.bVisible = true;
  SetConsoleCursorInfo(hConsole, &cursorInfo);
}

#endif  // ML_WINDOWS

#if !defined(ML_MAC) && !defined(ML_WINDOWS)
void waitForConsoleKeyPress()
{
  std::cout << "Press Enter to continue...";
  std::cin.get();
}
#endif

constexpr int kRtAudioCallbackFrames{512};

int RtAudioCallbackFn(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames,
                      double /*streamTime*/, RtAudioStreamStatus status, void* callbackData)
{
  constexpr size_t kMaxIOChannels{64};
  
  auto pData = reinterpret_cast<AudioProcessData*>(callbackData);
  
  if (status) std::cout << "Stream over/underflow detected." << std::endl;
  
  const float* inputs[kMaxIOChannels];
  float* outputs[kMaxIOChannels];
  
  const float* pInputBuffer = reinterpret_cast<const float*>(inputBuffer);
  float* pOutputBuffer = reinterpret_cast<float*>(outputBuffer);
  size_t nIns = std::min(kMaxIOChannels, pData->processContext->inputs.size());
  size_t nOuts = std::min(kMaxIOChannels, pData->processContext->outputs.size());
  for (int i = 0; i < nIns; ++i)
  {
    inputs[i] = pInputBuffer + i * nBufferFrames;
  }
  for (int i = 0; i < nOuts; ++i)
  {
    outputs[i] = pOutputBuffer + i * nBufferFrames;
  }
  
  pData->processContext->process(inputs, outputs, nBufferFrames, pData->processFn);
  return 0;
}

int AudioTask::startAudio()
{
  if (adac.getDeviceCount() < 1)
  {
    std::cout << "\nNo audio devices found!\n";
    return 0;
  }
  
  RtAudio::DeviceInfo info;
  uint32_t devices = adac.getDeviceCount();
  auto ids = adac.getDeviceIds();
  std::cout << "[AudioTask] Found: " << devices << " device(s)\n";
  for (uint32_t i = 0; i < devices; ++i)
  {
    info = adac.getDeviceInfo(ids[i]);
    std::cout << "\tDevice " << i << ": " << info.name << std::endl;
    std::cout << "\t\tinputs: " << info.inputChannels << " outputs: " << info.outputChannels
    << std::endl;
  }
  
  adac.showWarnings(true);
  
  auto nInputs = processData.processContext->inputs.size();
  auto nOutputs = processData.processContext->outputs.size();
  int sampleRate = processData.processContext->getSampleRate();
  unsigned int bufferFrames = kRtAudioCallbackFrames;
  
  RtAudio::StreamParameters iParams, oParams;
  iParams.deviceId = adac.getDefaultInputDevice();
  iParams.nChannels = static_cast<unsigned int>(nInputs);
  iParams.firstChannel = 0;
  oParams.deviceId = adac.getDefaultOutputDevice();
  oParams.nChannels = static_cast<unsigned int>(nOutputs);
  oParams.firstChannel = 0;
  
  RtAudio::StreamOptions options;
  options.flags |= RTAUDIO_NONINTERLEAVED;
  
  auto pInputParams = (nInputs ? &iParams : nullptr);
  
  if (RTAUDIO_NO_ERROR != adac.openStream(&oParams, pInputParams, RTAUDIO_FLOAT32,
                                          sampleRate, &bufferFrames, &RtAudioCallbackFn,
                                          &processData, &options))
  {
    std::cout << adac.getErrorText() << std::endl;
    return 0;
  }
  
  if (RTAUDIO_NO_ERROR != adac.startStream())
  {
    std::cout << adac.getErrorText() << std::endl;
    return 0;
  }
  
  return 1;
}

void AudioTask::stopAudio()
{
  processData.hasQuit = true;
  
  if (RTAUDIO_NO_ERROR != adac.stopStream())
  {
    std::cout << adac.getErrorText() << std::endl;
  }
  
  if (adac.isStreamOpen()) adac.closeStream();
}

bool AudioTask::hasQuit() const
{
  return processData.hasQuit;
}

int AudioTask::runConsoleApp()
{
  if (startAudio())
  {
    std::cout << "\nStream latency = " << adac.getStreamLatency() << " frames" << std::endl;
    std::cout << "sample rate: " << processData.processContext->getSampleRate() << "\n";
    
    std::cout << "\nRunning ... press any key to quit.\n";
    waitForConsoleKeyPress();
    
    stopAudio();
  }
  
  return 0;
}

AudioTask::~AudioTask() = default;

}  // namespace ml
