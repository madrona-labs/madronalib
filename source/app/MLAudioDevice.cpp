//
// Created by Randy Jones on 2/21/25.
//

#include "MLPlatform.h"
#include "MLAudioContext.h"
#include "MLAudioTask.h"
#include "rtaudio/RtAudio.h"

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

struct AudioDevice::Impl
{
  std::unique_ptr<RtAudio> devices_;

};


AudioDevice::AudioDevice() : pImpl(std::make_unique<Impl>())
{
  try { pImpl->devices_ = std::make_unique<RtAudio>(); }
  catch (...) { std::cout << "RtAudio creation error! \n"; }
  // TODO handle with RtAudioErrorCallback
}

int AudioDevice::getOutputSampleRate()
{
  if(!pImpl) return 0;
  if(!pImpl->devices_) return 0;
  auto rtAudio = *(pImpl->devices_);

  int rate{0};
  if (rtAudio.getDeviceCount() > 0)
  {
    auto id = rtAudio.getDefaultOutputDevice();
    auto info = rtAudio.getDeviceInfo(id);
    rate = info.currentSampleRate;
  }
  return rate;
}


}  // namespace ml
