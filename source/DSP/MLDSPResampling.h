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
#include "MLDSPFilters.h"

namespace ml
{

// Half-block type for rate-changed data
template<typename T>
using HalfBlock = AlignedArray<T, kFramesPerBlock / 2>;

// ----------------------------------------------------------------
// HalfBandFilter
// Polyphase allpass filter for 2x up/downsampling.
// Structure due to fred harris, A. G. Constantinides and Valenzuela.
// Order=4, rejection=70dB, transition band=0.1.

template<typename T>
struct HalfBandFilter
{
  // TEMP
  Allpass1<T> apa0{{0.07986642623635751f}};
  Allpass1<T> apa1{{0.5453536510711322f}};
  Allpass1<T> apb0{{0.28382934487410993f}};
  Allpass1<T> apb1{{0.8344118914807379f}};
  T b1{0.f};
  
  // 32 in → 64 out
  Block<T> upsample(const HalfBlock<T>& in)
  {
    Block<T> out;
    size_t i2 = 0;
    for (size_t i = 0; i < kFramesPerBlock / 2; ++i)
    {
      out[i2++] = apa1.nextFrame(apa0.nextFrame(in[i]));
      out[i2++] = apb1.nextFrame(apb0.nextFrame(in[i]));
    }
    return out;
  }
  
  // 64 in → 32 out
  HalfBlock<T> downsample(const Block<T>& in)
  {
    HalfBlock<T> out;
    size_t i2 = 0;
    for (size_t i = 0; i < kFramesPerBlock / 2; ++i)
    {
      T a0 = apa1.nextFrame(apa0.nextFrame(in[i2]));
      T b0 = apb1.nextFrame(apb0.nextFrame(in[i2 + 1]));
      out[i] = (a0 + b1) * T{0.5f};
      b1 = b0;
      i2 += 2;
    }
    return out;
  }
  
  void clear()
  {
    apa0.clear(); apa1.clear();
    apb0.clear(); apb1.clear();
    b1 = T{0.f};
  }
};

// ----------------------------------------------------------------
// Upsampler2x: one block in → two blocks out

template<typename T>
struct Upsampler2x
{
  HalfBandFilter<T> filter;
  
  std::pair<Block<T>, Block<T>> operator()(const Block<T>& in)
  {
    const auto* half = reinterpret_cast<const HalfBlock<T>*>(&in);
    return { filter.upsample(half[0]), filter.upsample(half[1]) };
  }
  
  void clear() { filter.clear(); }
};

// ----------------------------------------------------------------
// Downsampler2x: two blocks in → one block out

template<typename T>
struct Downsampler2x
{
  HalfBandFilter<T> filter;
  
  Block<T> operator()(const Block<T>& in1, const Block<T>& in2)
  {
    Block<T> out;
    auto lo = filter.downsample(in1);
    auto hi = filter.downsample(in2);
    auto* half = reinterpret_cast<HalfBlock<T>*>(&out);
    half[0] = lo;
    half[1] = hi;
    return out;
  }
  
  void clear() { filter.clear(); }
};

// ----------------------------------------------------------------
// Multi-octave Downsampler

class Downsampler
{
  std::vector<Downsampler2x<float>> _filters;
  std::vector<SignalBlock> _buffers;
  int _octaves;
  uint32_t _counter{0};
  
  SignalBlock& buf(int idx) { return _buffers[idx]; }
  
public:
  Downsampler(int octavesDown) : _octaves(octavesDown)
  {
    if (_octaves)
    {
      _filters.resize(_octaves);
      // one pair of buffers per octave + one output
      _buffers.resize(2 * _octaves + 1);
      clear();
    }
    else
    {
      _buffers.resize(1);
    }
  }
  
  bool write(SignalBlock v)
  {
    if (_octaves)
    {
      buf(_counter & 1) = v;
      
      uint32_t mask = 1;
      for (int h = 0; h < _octaves; ++h)
      {
        bool b0 = _counter & mask;
        if (!b0) break;
        mask <<= 1;
        bool b1 = _counter & mask;
        
        buf(h * 2 + 2 + b1) = _filters[h](buf(h * 2), buf(h * 2 + 1));
      }
      
      uint32_t counterMask = (1 << _octaves) - 1;
      _counter = (_counter + 1) & counterMask;
      return (_counter == 0);
    }
    else
    {
      buf(0) = v;
      return true;
    }
  }
  
  SignalBlock read() { return _buffers.back(); }
  
  void clear()
  {
    for (auto& f : _filters) f.clear();
    for (auto& b : _buffers) b = SignalBlock{0.f};
    _counter = 0;
  }
};

// ----------------------------------------------------------------
// Multi-octave Upsampler

struct Upsampler
{
  std::vector<Upsampler2x<float>> _filters;
  std::vector<SignalBlock> _buffers;
  int _octaves;
  int readIdx_{0};
  
  SignalBlock& buf(int idx) { return _buffers[idx]; }
  
  Upsampler(int octavesUp) : _octaves(octavesUp)
  {
    if (_octaves)
    {
      _filters.resize(_octaves);
      _buffers.resize(1 << _octaves);
      clear();
    }
  }
  
  void write(SignalBlock x)
  {
    int numBufs = 1 << _octaves;
    buf(numBufs - 1) = x;
    
    for (int j = 0; j < _octaves; ++j)
    {
      int sourceBufs = 1 << j;
      int destBufs = sourceBufs << 1;
      int srcStart = numBufs - sourceBufs;
      int destStart = numBufs - destBufs;
      
      for (int i = 0; i < sourceBufs; ++i)
      {
        auto [first, second] = _filters[j](buf(srcStart + i));
        buf(destStart + i * 2) = first;
        buf(destStart + i * 2 + 1) = second;
      }
    }
    readIdx_ = 0;
  }
  
  SignalBlock read() { return buf(readIdx_++); }
  
  void clear()
  {
    for (auto& f : _filters) f.clear();
    for (auto& b : _buffers) b = SignalBlock{0.f};
    readIdx_ = 0;
  }
};
// ----------------------------------------------------------------
// higher-order functions with DSP

// Upsample2xFunction is a function object that given a process function f,
// upsamples the input x by 2, applies f, downsamples and returns the result.
// The total delay from the resampling filters used is about 3 samples.

template <typename T, int IN_ROWS>
class Upsample2xFunction
{
  static constexpr int OUT_ROWS = 1;
  
  using inputType = const SignalBlockArrayBase<T, IN_ROWS>;
  using outputType = SignalBlockArrayBase<T, OUT_ROWS>;
  using ProcessFn = std::function<outputType(inputType)>;
  
public:
  inline outputType operator()(ProcessFn fn, inputType vx)
  {
    // upsample each row of input to 2x buffers
    for (int j = 0; j < IN_ROWS; ++j)
    {
      auto [first, second] = mUppers[j](vx.constRow(j));
      mUpsampledInput1.row(j) = first;
      mUpsampledInput2.row(j) = second;
    }
    
    // process upsampled input
    mUpsampledOutput1 = fn(mUpsampledInput1);
    mUpsampledOutput2 = fn(mUpsampledInput2);
    
    // downsample each processed row to 1x output
    outputType vy;
    for (int j = 0; j < OUT_ROWS; ++j)
    {
      vy.setRow(j, mDowners[j](mUpsampledOutput1.getRow(j), mUpsampledOutput2.getRow(j)));
    }
    return vy;
  }
  
  void clear()
  {
    for (auto& u : mUppers) u.clear();
    for (auto& d : mDowners) d.clear();
  }
  
private:
  std::array<Upsampler2x<T>, IN_ROWS> mUppers;
  std::array<Downsampler2x<T>, OUT_ROWS> mDowners;
  SignalBlockArrayBase<T, IN_ROWS> mUpsampledInput1, mUpsampledInput2;
  SignalBlockArrayBase<T, OUT_ROWS> mUpsampledOutput1, mUpsampledOutput2;
};

// Downsample2xFunction is a function object that given a process function f,
// downsamples the input x by 2, applies f, upsamples and returns the result.
// Since two Blocks of input are needed to create a single block of
// downsampled input to the wrapped function, this function has an entire
// Block of delay in addition to the group delay of the allpass
// interpolation (about 6 samples).

template <typename T, int IN_ROWS>
class Downsample2xFunction
{
  static constexpr int OUT_ROWS = 1;
  
  using inputType = const SignalBlockArrayBase<T, IN_ROWS>;
  using outputType = SignalBlockArrayBase<T, OUT_ROWS>;
  using ProcessFn = std::function<outputType(inputType)>;
  
public:
  inline outputType operator()(ProcessFn fn, inputType vx)
  {
    outputType vy;
    if (mPhase)
    {
      // downsample each row of input to 1/2x buffers
      for (int j = 0; j < IN_ROWS; ++j)
      {
        mDownsampledInput.row(j) = mDowners[j](mInputBuffer.constRow(j), vx.constRow(j));
      }
      
      // process downsampled input
      mDownsampledOutput = fn(mDownsampledInput);
      
      // upsample each processed row to output
      for (int j = 0; j < OUT_ROWS; ++j)
      {
        auto [first, second] = mUppers[j](mDownsampledOutput.getRow(j));
        vy.setRow(j, first);
        mOutputBuffer.setRow(j, second);
      }
    }
    else
    {
      mInputBuffer = vx;
      vy = mOutputBuffer;
    }
    mPhase = !mPhase;
    return vy;
  }
  
  void clear()
  {
    for (auto& d : mDowners) d.clear();
    for (auto& u : mUppers) u.clear();
    mPhase = false;
  }
  
private:
  std::array<Downsampler2x<T>, IN_ROWS> mDowners;
  std::array<Upsampler2x<T>, OUT_ROWS> mUppers;
  SignalBlockArrayBase<T, IN_ROWS> mInputBuffer;
  SignalBlockArrayBase<T, OUT_ROWS> mOutputBuffer;
  SignalBlockArrayBase<T, IN_ROWS> mDownsampledInput;
  SignalBlockArrayBase<T, OUT_ROWS> mDownsampledOutput;
  bool mPhase{false};
};

}  // namespace ml

