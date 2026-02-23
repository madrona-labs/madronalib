// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// Banks of float4 processors for polyphonic DSP.
// GenBank: a bank of generators (no audio input, params → audio).
// FilterBank: a bank of filters (audio + optional params → audio).
// Internally, ceil(ROWS/4) float4 processors handle groups of 4 voices each.

#pragma once

#include <functional>

#include "MLDSPOps.h"
#include "MLDSPFilters.h"
#include "MLDSPDelays.h"
#include "MLDSPResampling.h"

namespace ml
{

// ----------------------------------------------------------------
// GenBank: a bank of generator processors (no audio input).
// FN must be a generator with the Gen<T, Derived> interface.
// Input rows are stacked param signals: kNumFloat4Procs * nParams rows total,
// with each processor's nParams rows grouped together.

template <template<typename> class FN, int ROWS>
class GenBank
{
public:
  static constexpr int kNumFloat4Procs = (ROWS + 3) / 4;
  using Processor = FN<float4>;

  static constexpr int nParams  = Processor::nParams;
  static constexpr int IN_ROWS  = kNumFloat4Procs * nParams;
  static constexpr int OUT_ROWS = kNumFloat4Procs;

  using inputType  = SignalBlockArrayBase<float4, IN_ROWS>;
  using outputType = SignalBlockArrayBase<float4, OUT_ROWS>;
  using Params     = typename Processor::Params;

  // Signal-rate params: IN_ROWS rows of param signals, nParams per processor.
  outputType operator()(const inputType& input)
  {
    outputType output;
    for (int p = 0; p < kNumFloat4Procs; ++p)
    {
      SignalBlockArrayBase<float4, nParams> paramSlice;
      for (int r = 0; r < nParams; ++r)
        paramSlice.setRow(r, input.getRow(p * nParams + r));
      output.setRow(p, _processors[p](paramSlice));
    }
    return output;
  }

  // Per-block params (interpolated), one Params per processor.
  outputType operator()(const std::array<Params, kNumFloat4Procs>& params)
  {
    outputType output;
    for (int p = 0; p < kNumFloat4Procs; ++p)
      output.setRow(p, _processors[p](params[p]));
    return output;
  }

  // Constant stored coefficients.
  outputType operator()()
  {
    outputType output;
    for (int p = 0; p < kNumFloat4Procs; ++p)
      output.setRow(p, _processors[p]());
    return output;
  }

  void clear() { for (auto& p : _processors) p.clear(); }
  Processor& operator[](size_t n) { return _processors[n]; }

private:
  std::array<Processor, kNumFloat4Procs> _processors;
};


// ----------------------------------------------------------------
// FilterBank: a bank of filter processors (audio input → audio output).
// FN must be a filter with the Filter<T, Derived> interface.
// Audio input and output are OUT_ROWS = kNumFloat4Procs rows.
// Signal-rate params use PARAM_ROWS = kNumFloat4Procs * nParams rows,
// with each processor's nParams rows grouped together.

template <template<typename> class FN, int ROWS>
class FilterBank
{
public:
  static constexpr int kNumFloat4Procs = (ROWS + 3) / 4;
  using Processor = FN<float4>;

  static constexpr int nParams    = Processor::nParams;
  static constexpr int IN_ROWS    = kNumFloat4Procs;
  static constexpr int OUT_ROWS   = kNumFloat4Procs;
  static constexpr int PARAM_ROWS = kNumFloat4Procs * nParams;

  using inputType  = SignalBlockArrayBase<float4, IN_ROWS>;
  using outputType = SignalBlockArrayBase<float4, OUT_ROWS>;
  using paramType  = SignalBlockArrayBase<float4, PARAM_ROWS>;
  using Params     = typename Processor::Params;

  // Constant stored coefficients.
  outputType operator()(const inputType& input)
  {
    outputType output;
    for (int p = 0; p < kNumFloat4Procs; ++p)
      output.setRow(p, _processors[p](input.getRow(p)));
    return output;
  }

  // Per-block params (interpolated), one Params per processor.
  outputType operator()(const inputType& input,
                        const std::array<Params, kNumFloat4Procs>& params)
  {
    outputType output;
    for (int p = 0; p < kNumFloat4Procs; ++p)
      output.setRow(p, _processors[p](input.getRow(p), params[p]));
    return output;
  }

  // Signal-rate params: PARAM_ROWS rows, nParams per processor.
  outputType operator()(const inputType& input, const paramType& paramSignals)
  {
    outputType output;
    for (int p = 0; p < kNumFloat4Procs; ++p)
    {
      SignalBlockArrayBase<float4, nParams> paramSlice;
      for (int r = 0; r < nParams; ++r)
        paramSlice.setRow(r, paramSignals.getRow(p * nParams + r));
      output.setRow(p, _processors[p](input.getRow(p), paramSlice));
    }
    return output;
  }

  void clear() { for (auto& p : _processors) p.clear(); }
  Processor& operator[](size_t n) { return _processors[n]; }

private:
  std::array<Processor, kNumFloat4Procs> _processors;
};

}  // namespace ml
