//
// Created by Randy Jones on 2/21/25.
//

#include "MLPlatform.h"
#include "MLAudioContext.h"
#include "MLAudioTask.h"
#include "rtaudio/RtAudio.h"

namespace ml
{

constexpr int kRtAudioCallbackFrames{512};

struct AudioDevice::Impl
{
  std::unique_ptr<RtAudio> deviceController_;
};

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

AudioDevice::AudioDevice() : pImpl(std::make_unique<Impl>())
{
  try { pImpl->deviceController_ = std::make_unique<RtAudio>(); }
  catch (...) { std::cout << "AudioDevice: RtAudio creation error! \n"; }
  // TODO handle errors with RtAudioErrorCallback
}

AudioDevice::~AudioDevice()
{
  stopAudio();
}

unsigned int AudioDevice::startAudio(const AudioProcessData& processData)
{
  if (!pImpl->deviceController_) return 0;
  RtAudio& ctrl = *pImpl->deviceController_;
  
  uint32_t nDevices = ctrl.getDeviceCount();
  if (nDevices < 1)
  {
    std::cout << "AudioDevice: No audio devices found!\n";
    return 0;
  }
  
  auto ids = ctrl.getDeviceIds();
  
  // TEMP
  std::cout << "AudioDevice: Found: " << nDevices << " device(s)\n";
  for (uint32_t i = 0; i < nDevices; ++i)
  {
    auto info = ctrl.getDeviceInfo(ids[i]);
    std::cout << "\tDevice " << i << ": " << info.name << std::endl;
    std::cout << "\t\tinputs: " << info.inputChannels << " outputs: " << info.outputChannels << " ID: " << info.ID << std::endl;
    
  }
  
  ctrl.showWarnings(true);
  
  auto nInputs = processData.processContext->inputs.size();
  auto nOutputs = processData.processContext->outputs.size();
  unsigned int bufferFrames = kRtAudioCallbackFrames;
  
  RtAudio::StreamParameters iParams, oParams;
  iParams.deviceId = ctrl.getDefaultInputDevice();
  iParams.nChannels = static_cast<unsigned int>(nInputs);
  iParams.firstChannel = 0;
  oParams.deviceId = ctrl.getDefaultOutputDevice();
  oParams.nChannels = static_cast<unsigned int>(nOutputs);
  oParams.firstChannel = 0;
  
  RtAudio::StreamOptions options;
  options.flags |= RTAUDIO_NONINTERLEAVED;
  
  auto pInputParams = (nInputs ? &iParams : nullptr);
  
  auto outputDeviceInfo = ctrl.getDeviceInfo(oParams.deviceId);
  unsigned int deviceSampleRate = outputDeviceInfo.currentSampleRate;
  
  auto processDataAsConst = const_cast< AudioProcessData* >(&processData);
  auto processDataAsVoidPtr = static_cast< void * >(processDataAsConst);
  
  if (RTAUDIO_NO_ERROR != ctrl.openStream(&oParams, pInputParams, RTAUDIO_FLOAT32,
                                          deviceSampleRate, &bufferFrames, &RtAudioCallbackFn,
                                          processDataAsVoidPtr, &options))
  {
    std::cout << ctrl.getErrorText() << std::endl;
    return 0;
  }
  
  if (RTAUDIO_NO_ERROR != ctrl.startStream())
  {
    std::cout << ctrl.getErrorText() << std::endl;
    return 0;
  }
  
  return deviceSampleRate;
}

void AudioDevice::stopAudio()
{
  if (!pImpl->deviceController_) return 0;
  RtAudio& ctrl = *pImpl->deviceController_;

  if (RTAUDIO_NO_ERROR != ctrl.stopStream())
  {
    std::cout << ctrl.getErrorText() << std::endl;
  }
  
  if (ctrl.isStreamOpen()) ctrl.closeStream();
}

int AudioDevice::getOutputSampleRate()
{
  if (!pImpl->deviceController_) return 0;
  RtAudio& ctrl = *pImpl->deviceController_;

  int rate{0};
  if (ctrl.getDeviceCount() > 0)
  {
    auto id = ctrl.getDefaultOutputDevice();
    auto info = ctrl.getDeviceInfo(id);
    rate = info.currentSampleRate;
  }
  return rate;
}

}  // namespace ml
