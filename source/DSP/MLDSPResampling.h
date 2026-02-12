// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// DSP filters: functor objects implementing an operator()(Block<T> input, ...).
//
// Filter cutoffs are set by a parameter omega, equal to frequency / sample
// rate. This lets filter objects be unaware of the sample rate, resulting in
// less code overall. For all filters, k is a damping parameter equal to 1/Q
// where Q is the analog filter "quality." For bell and shelf filters, gain is
// specified as an output / input ratio A.

#pragma once

#include <vector>

#include "MLDSPOps.h"
#include "MLDSPMath.h"

namespace ml
{


// Half Band Filter
// Polyphase allpass filter used to upsample or downsample a signal by 2x.
// Structure due to fred harris, A. G. Constantinides and Valenzuela.

class HalfBandFilter
{
 public:
  inline SignalBlock upsampleFirstHalf(const SignalBlock vx)
  {
    SignalBlock vy;
    int i2 = 0;
    for (int i = 0; i < kFramesPerBlock / 2; ++i)
    {
      vy[i2++] = apa1.processSample(apa0.processSample(vx[i]));
      vy[i2++] = apb1.processSample(apb0.processSample(vx[i]));
    }
    return vy;
  }

  inline SignalBlock upsampleSecondHalf(const SignalBlock vx)
  {
    SignalBlock vy;
    int i2 = 0;
    for (int i = kFramesPerBlock / 2; i < kFramesPerBlock; ++i)
    {
      vy[i2++] = apa1.processSample(apa0.processSample(vx[i]));
      vy[i2++] = apb1.processSample(apb0.processSample(vx[i]));
    }
    return vy;
  }

  inline SignalBlock downsample(const SignalBlock vx1, const SignalBlock vx2)
  {
    SignalBlock vy;
    int i2 = 0;
    for (int i = 0; i < kFramesPerBlock / 2; ++i)
    {
      float a0 = apa1.processSample(apa0.processSample(vx1[i2]));
      float b0 = apb1.processSample(apb0.processSample(vx1[i2 + 1]));
      vy[i] = (a0 + b1) * 0.5f;
      b1 = b0;
      i2 += 2;
    }
    i2 = 0;
    for (int i = kFramesPerBlock / 2; i < kFramesPerBlock; ++i)
    {
      float a0 = apa1.processSample(apa0.processSample(vx2[i2]));
      float b0 = apb1.processSample(apb0.processSample(vx2[i2 + 1]));
      vy[i] = (a0 + b1) * 0.5f;
      b1 = b0;
      i2 += 2;
    }
    return vy;
  }

  void clear()
  {
    apa0.clear();
    apa1.clear();
    apb0.clear();
    apb1.clear();
    b1 = 0;
  }

 private:
  // order=4, rejection=70dB, transition band=0.1.
  Allpass1 apa0{0.07986642623635751f}, apa1{0.5453536510711322f}, apb0{0.28382934487410993f},
      apb1{0.8344118914807379f};
  float b1{0};
};

// Downsampler
// a cascade of half band filters, one for each octave.
// TODO this is complicated. replace with single-channel version then use Bank<Downsampler> for
// multiple channels?
class Downsampler
{
  std::vector<HalfBandFilter> _filters;
  std::vector<float> _buffers;
  int _octaves;
  int _numBuffers;
  uint32_t _counter{0};

  float* bufferPtr(int idx) { return _buffers.data() + idx * kFramesPerBlock; }

 public:
  Downsampler(int octavesDown) : _octaves(octavesDown)
  {
    if (_octaves)
    {
      // one pair of buffers for each octave plus one output buffer.
      _numBuffers = 2 * _octaves + 1;

      // each octave uses one filter.
      _filters.resize(_octaves);

      // get all buffers as a single contiguous array of floats.
      _buffers.resize(kFramesPerBlock * _numBuffers);

      clear();
    }
  }
  ~Downsampler() = default;

  // write a vector of samples to the filter chain, run filters, and return
  // true if there is a new vector of output to read (every 2^octaves writes)
  bool write(SignalBlock v)
  {
    if (_octaves)
    {
      // write input to one of first two buffers
      const float* pSrc = v.data();
      float* pDest = bufferPtr(_counter & 1);
      std::copy(pSrc, pSrc + kFramesPerBlock, pDest);

      // look at the bits of the counter from lowest to highest.
      // there is one bit for each octave of downsampling.
      // each octave is run if its bit and all lesser bits are 1.
      uint32_t mask = 1;
      for (int h = 0; h < _octaves; ++h)
      {
        bool b0 = _counter & mask;
        if (!b0) break;
        mask <<= 1;
        bool b1 = _counter & mask;

        // run filter
        HalfBandFilter* f = &(_filters[h]);
        SignalBlock vSrc1(bufferPtr(h * 2));
        SignalBlock vSrc2(bufferPtr(h * 2 + 1));
        SignalBlock vDest = f->downsample(vSrc1, vSrc2);
        store(vDest, bufferPtr(h * 2 + 2 + b1));
      }

      // advance and wrap counter. If it's back to 0, we have output
      uint32_t counterMask = (1 << _octaves) - 1;
      _counter = (_counter + 1) & counterMask;
      return (_counter == 0);
    }
    else
    {
      // write input to final buffer
      const float* pSrc = v.data();
      float* pDest = bufferPtr(_numBuffers - 1);
      std::copy(pSrc, pSrc + kFramesPerBlock, pDest);
      return true;
    }
  }

  SignalBlock read() { return SignalBlock(bufferPtr(_numBuffers - 1)); }

  void clear()
  {
    for (auto& f : _filters)
    {
      f.clear();
    }
    std::fill(_buffers.begin(), _buffers.end(), 0.f);
    _counter = 0;
  }
};

struct Upsampler
{
  std::vector<HalfBandFilter> _filters;
  std::vector<float> _buffers;
  int _octaves;
  int _numBuffers;
  int readIdx_{0};

  float* bufferPtr(int idx) { return _buffers.data() + idx * kFramesPerBlock; }

  Upsampler(int octavesUp) : _octaves(octavesUp)
  {
    if (_octaves)
    {
      _numBuffers = 1 << _octaves;
      size_t numFilters = _octaves;
      _filters.resize(numFilters);

      // get all buffers as a single contiguous array of floats.
      _buffers.resize(kFramesPerBlock * _numBuffers);

      clear();
    }
  }
  ~Upsampler() = default;

  void write(SignalBlock x)
  {
    // write to last vector in buffer
    store(x, bufferPtr(_numBuffers - 1));

    // for each octave of upsampling, upsample blocks to twice as many, in place, ending at buffers
    // end
    for (int j = 0; j < _octaves; ++j)
    {
      int sourceBufs = 1 << j;
      int destBufs = sourceBufs << 1;
      int srcStart = _numBuffers - sourceBufs;
      int destStart = _numBuffers - destBufs;

      for (int i = 0; i < sourceBufs; ++i)
      {
        SignalBlock src, dest1, dest2;
        load(src, bufferPtr(srcStart + i));
        dest1 = _filters[j].upsampleFirstHalf(src);
        dest2 = _filters[j].upsampleSecondHalf(src);
        store(dest1, bufferPtr(destStart + (i * 2)));
        store(dest2, bufferPtr(destStart + (i * 2) + 1));
      }
    }
    readIdx_ = 0;
  }

  // after a write, 1 << octaves reads are available.
  SignalBlock read()
  {
    SignalBlock result;
    load(result, bufferPtr(readIdx_++));
    return result;
  }

  void clear()
  {
    for (auto& f : _filters)
    {
      f.clear();
    }
    std::fill(_buffers.begin(), _buffers.end(), 0.f);
    readIdx_ = 0;
  }
};

}  // namespace ml

