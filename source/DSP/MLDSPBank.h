// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// Here are some function objects that take DSP functions as parameters to
// operator() and apply the function in a different context, such as upsampled,
// overlap-added or in the frequency domain.


#pragma once

#include <functional>

#include "MLDSPOps.h"
#include "MLDSPFilters.h"
#include "MLDSPDelays.h"
#include "MLDSPResampling.h"

namespace ml
{

// Bank: a bank of float4 processors.
// FN must be a processor capable of handing float4 (e.g. Lopass) with the standard
// Filter interface: operator() with 0, 1 (Params), or 1 (signal-rate) args
// plus input, and a clear() method.
// ROWS is the number of voices. Internally, ceil(ROWS/4) float4 processors
// handle groups of 4 voices each.

template <template<typename> class FN, int ROWS>
class Bank
{
public:
  static constexpr int kNumFloat4Procs = (ROWS + 3) / 4;
  using Processor = FN<float4>;
  
  using BankBlock = SignalBlockArrayBase<float4, kNumFloat4Procs>;
  using Params = typename Processor::Params;
  using ParamBlock = SignalBlockArrayBase<float4, Processor::nParams>;
  
  // Constant stored coefficients
  BankBlock operator()(const BankBlock& input)
  {
    BankBlock output;
    for (int p = 0; p < kNumFloat4Procs; ++p)
    {
      output.row(p) = _processors[p](input.constRow(p));
    }
    return output;
  }
  
  // Per-block params (interpolated), one Params per processor
  BankBlock operator()(const BankBlock& input,
                       const std::array<Params, kNumFloat4Procs>& params)
  {
    BankBlock output;
    for (int p = 0; p < kNumFloat4Procs; ++p)
    {
      output.row(p) = _processors[p](input.constRow(p), params[p]);
    }
    return output;
  }
  
  // Signal-rate params, one param block per processor
  BankBlock operator()(const BankBlock& input,
                       const std::array<ParamBlock, kNumFloat4Procs>& paramBlocks)
  {
    BankBlock output;
    for (int p = 0; p < kNumFloat4Procs; ++p)
    {
      output.row(p) = _processors[p](input.constRow(p), paramBlocks[p]);
    }
    return output;
  }
  
  void clear()
  {
    for (auto& p : _processors) p.clear();
  }
  
  Processor& operator[](size_t n) { return _processors[n]; }
  
private:
  std::array<Processor, kNumFloat4Procs> _processors;
};

}  // namespace ml
