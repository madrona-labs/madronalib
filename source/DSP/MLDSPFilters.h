// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// DSP filters: functor objects implementing an operator()(Block<T> input, ...).
//
// Filter cutoffs are set by a parameter omega, equal to frequency / sample
// rate. This lets filter objects be unaware of the sample rate, resulting in
// less code overall.

#pragma once

#include <vector>

#include "MLDSPOps.h"
#include "MLDSPSolvers.h"

namespace ml
{

template<typename T, size_t COEFFS_SIZE>
SignalBlockArrayBase<T, COEFFS_SIZE> interpolateCoeffsLinear(
                                                             const std::array<T, COEFFS_SIZE>& c0,
                                                             const std::array<T, COEFFS_SIZE>& c1)
{
  SignalBlockArrayBase<T, COEFFS_SIZE> vy;
  for (size_t i = 0; i < COEFFS_SIZE; ++i)
  {
    vy.setRow(i, interpolateBlockLinear(c0[i], c1[i]));
  }
  return vy;
}

template<typename T, typename Derived>
struct Filter
{
  // Block processing with parameter interpolation (one Params per block)
  template<typename Params>
  Block<T> operator()(const Block<T>& input, Params nextParams)
  {
    auto& self = *static_cast<Derived*>(this);
    Block<T> output;
    
    auto nextCoeffs = Derived::makeCoeffs(nextParams);
    auto coeffsBlock = interpolateCoeffsLinear(self.coeffs, nextCoeffs);
    self.coeffs = nextCoeffs;
    
    for (size_t t = 0; t < kFramesPerBlock; ++t)
    {
      typename Derived::Coeffs c;
      for (size_t i = 0; i < Derived::nCoeffs; ++i)
      {
        c[i] = coeffsBlock.rowPtr(i)[t];
      }
      output[t] = self.nextFrame(input[t], c);
    }
    return output;
  }
  
  // Block processing with signal-rate params (makeCoeffs called per frame)
  template<size_t N_PARAMS>
  Block<T> operator()(const Block<T>& input,
                      const SignalBlockArrayBase<T, N_PARAMS>& paramBlock)
  {
    static_assert(N_PARAMS == Derived::nParams, "paramBlock row count must match nParams");
    auto& self = *static_cast<Derived*>(this);
    Block<T> output;
    
    for (size_t t = 0; t < kFramesPerBlock; ++t)
    {
      typename Derived::Params p;
      for (size_t i = 0; i < Derived::nParams; ++i)
      {
        p[i] = paramBlock.rowPtr(i)[t];
      }
      self.coeffs = Derived::makeCoeffs(p);
      output[t] = self.nextFrame(input[t], self.coeffs);
    }
    return output;
  }
  
  // Block processing with constant stored coefficients
  Block<T> operator()(const Block<T>& input)
  {
    auto& self = *static_cast<Derived*>(this);
    Block<T> output;
    for (size_t t = 0; t < kFramesPerBlock; ++t)
    {
      output[t] = self.nextFrame(input[t], self.coeffs);
    }
    return output;
  }
};

// --------------------------------------------------------------------------------
// utility filters implemented as SVF variations
// Thanks to Andrew Simper [www.cytomic.com] for sharing his work over the years.
//
// For all these filters, k is a damping parameter equal to 1/Q
// where Q is the analog filter "quality." Maximum resonance is at k=0.
// For bell and shelf filters, gain is specified as an output / input ratio A.

template<typename T>
struct Lopass : Filter<T, Lopass<T>>
{
  enum { omega, k, nParams };
  enum { g0, g1, g2, nCoeffs };
  enum { ic1eq, ic2eq, nStateVars };
  
  using Params = std::array<T, nParams>;
  using Coeffs = std::array<T, nCoeffs>;
  using State = std::array<T, nStateVars>;
  
  Coeffs coeffs{};
  State state{};
  
  void clear() {
    const Params kDefaultParams{0.f, 0.5f};
    coeffs = makeCoeffs(kDefaultParams);
    state.fill(T{0.f});
  }

  static Coeffs makeCoeffs(Params p)
  {
    T piOmega = T{kPi} * p[omega];
    T s1 = sin(piOmega);
    T s2 = sin(T{2.0f} * piOmega);
    T nrm = T{1.0f} / (T{2.f} + p[k] * s2);
    T cg0 = s2 * nrm;
    T cg1 = (T{-2.f} * s1 * s1 - p[k] * s2) * nrm;
    T cg2 = (T{2.0f} * s1 * s1) * nrm;
    return {cg0, cg1, cg2};
  }
  
  T nextFrame(T x, Coeffs c)
  {
    T t0 = x - state[ic2eq];
    T t1 = c[g0] * t0 + c[g1] * state[ic1eq];
    T t2 = c[g2] * t0 + c[g0] * state[ic1eq];
    T v2 = t2 + state[ic2eq];
    state[ic1eq] += T{2.0f} * t1;
    state[ic2eq] += T{2.0f} * t2;
    return v2;
  }
};

template<typename T>
struct Hipass : Filter<T, Hipass<T>>
{
  enum { omega, k, nParams };
  enum { g0, g1, g2, gk, nCoeffs };
  enum { ic1eq, ic2eq, nStateVars };
  
  using Params = std::array<T, nParams>;
  using Coeffs = std::array<T, nCoeffs>;
  using State = std::array<T, nStateVars>;
  
  Coeffs coeffs{};
  State state{};
  
  void clear() {
    const Params kDefaultParams{0.f, 0.5f};
    coeffs = makeCoeffs(kDefaultParams);
    state.fill(T{0.f});
  }
  
  static Coeffs makeCoeffs(Params p)
  {
    T piOmega = T{kPi} * p[omega];
    T s1 = sin(piOmega);
    T s2 = sin(T{2.0f} * piOmega);
    T nrm = T{1.0f} / (T{2.f} + p[k] * s2);
    T cg0 = s2 * nrm;
    T cg1 = (T{-2.f} * s1 * s1 - p[k] * s2) * nrm;
    T cg2 = (T{2.0f} * s1 * s1) * nrm;
    return {cg0, cg1, cg2, p[k]};
  }
  
  T nextFrame(T x, Coeffs c)
  {
    T t0 = x - state[ic2eq];
    T t1 = c[g0] * t0 + c[g1] * state[ic1eq];
    T t2 = c[g2] * t0 + c[g0] * state[ic1eq];
    T v1 = t1 + state[ic1eq];
    T v2 = t2 + state[ic2eq];
    state[ic1eq] += T{2.0f} * t1;
    state[ic2eq] += T{2.0f} * t2;
    return x - c[gk] * v1 - v2;
  }
};

template<typename T>
struct Bandpass : Filter<T, Bandpass<T>>
{
  enum { omega, k, nParams };
  enum { g0, g1, g2, nCoeffs };
  enum { ic1eq, ic2eq, nStateVars };
  
  using Params = std::array<T, nParams>;
  using Coeffs = std::array<T, nCoeffs>;
  using State = std::array<T, nStateVars>;
  
  Coeffs coeffs{};
  State state{};
  
  void clear() {
    const Params kDefaultParams{0.f, 0.5f};
    coeffs = makeCoeffs(kDefaultParams);
    state.fill(T{0.f});
  }
  
  static Coeffs makeCoeffs(Params p)
  {
    T piOmega = T{kPi} * p[omega];
    T s1 = sin(piOmega);
    T s2 = sin(T{2.0f} * piOmega);
    T nrm = T{1.0f} / (T{2.f} + p[k] * s2);
    T cg0 = s2 * nrm;
    T cg1 = (T{-2.f} * s1 * s1 - p[k] * s2) * nrm;
    T cg2 = (T{2.0f} * s1 * s1) * nrm;
    return {cg0, cg1, cg2};
  }
  
  T nextFrame(T x, Coeffs c)
  {
    T t0 = x - state[ic2eq];
    T t1 = c[g0] * t0 + c[g1] * state[ic1eq];
    T v1 = t1 + state[ic1eq];
    T t2 = c[g2] * t0 + c[g0] * state[ic1eq];
    state[ic1eq] += T{2.0f} * t1;
    state[ic2eq] += T{2.0f} * t2;
    return v1;
  }
};

template<typename T>
struct LoShelf : Filter<T, LoShelf<T>>
{
  enum { omega, k, A, nParams };
  enum { a1, a2, a3, m1, m2, nCoeffs };
  enum { ic1eq, ic2eq, nStateVars };
  
  using Params = std::array<T, nParams>;
  using Coeffs = std::array<T, nCoeffs>;
  using State = std::array<T, nStateVars>;
  
  Coeffs coeffs{};
  State state{};
  
  void clear() {
    const Params kDefaultParams{0.f, 0.5f, 1.f};
    coeffs = makeCoeffs(kDefaultParams);
    state.fill(T{0.f});
  }
  
  static Coeffs makeCoeffs(Params p)
  {
    T piOmega = T{kPi} * p[omega];
    T g = tan(piOmega) / sqrt(p[A]);
    T ca1 = T{1.f} / (T{1.f} + g * (g + p[k]));
    T ca2 = g * ca1;
    T ca3 = g * ca2;
    T cm1 = p[k] * (p[A] - T{1.f});
    T cm2 = p[A] * p[A] - T{1.f};
    return {ca1, ca2, ca3, cm1, cm2};
  }
  
  T nextFrame(T x, Coeffs c)
  {
    T v3 = x - state[ic2eq];
    T v1 = c[a1] * state[ic1eq] + c[a2] * v3;
    T v2 = state[ic2eq] + c[a2] * state[ic1eq] + c[a3] * v3;
    state[ic1eq] = T{2.f} * v1 - state[ic1eq];
    state[ic2eq] = T{2.f} * v2 - state[ic2eq];
    return x + c[m1] * v1 + c[m2] * v2;
  }
};

template<typename T>
struct HiShelf : Filter<T, HiShelf<T>>
{
  enum { omega, k, A, nParams };
  enum { a1, a2, a3, m0, m1, m2, nCoeffs };
  enum { ic1eq, ic2eq, nStateVars };
  
  using Params = std::array<T, nParams>;
  using Coeffs = std::array<T, nCoeffs>;
  using State = std::array<T, nStateVars>;
  
  Coeffs coeffs{};
  State state{};
  
  void clear() {
    const Params kDefaultParams{0.f, 0.5f, 1.f};
    coeffs = makeCoeffs(kDefaultParams);
    state.fill(T{0.f});
  }
  
  static Coeffs makeCoeffs(Params p)
  {
    T piOmega = T{kPi} * p[omega];
    T g = tan(piOmega) * sqrt(p[A]);
    T ca1 = T{1.f} / (T{1.f} + g * (g + p[k]));
    T ca2 = g * ca1;
    T ca3 = g * ca2;
    T cm0 = p[A] * p[A];
    T cm1 = p[k] * (T{1.f} - p[A]) * p[A];
    T cm2 = T{1.f} - p[A] * p[A];
    return {ca1, ca2, ca3, cm0, cm1, cm2};
  }
  
  T nextFrame(T x, Coeffs c)
  {
    T v3 = x - state[ic2eq];
    T v1 = c[a1] * state[ic1eq] + c[a2] * v3;
    T v2 = state[ic2eq] + c[a2] * state[ic1eq] + c[a3] * v3;
    state[ic1eq] = T{2.f} * v1 - state[ic1eq];
    state[ic2eq] = T{2.f} * v2 - state[ic2eq];
    return c[m0] * x + c[m1] * v1 + c[m2] * v2;
  }
};

template<typename T>
struct Bell : Filter<T, Bell<T>>
{
  enum { omega, k, A, nParams };
  enum { a1, a2, a3, m1, nCoeffs };
  enum { ic1eq, ic2eq, nStateVars };
  
  using Params = std::array<T, nParams>;
  using Coeffs = std::array<T, nCoeffs>;
  using State = std::array<T, nStateVars>;
  
  Coeffs coeffs{};
  State state{};
  
  void clear() {
    const Params kDefaultParams{0.f, 0.5f, 1.f};
    coeffs = makeCoeffs(kDefaultParams);
    state.fill(T{0.f});
  }
  
  static Coeffs makeCoeffs(Params p)
  {
    T kc = p[k] / p[A];
    T piOmega = T{kPi} * p[omega];
    T g = tan(piOmega);
    T ca1 = T{1.f} / (T{1.f} + g * (g + kc));
    T ca2 = g * ca1;
    T ca3 = g * ca2;
    T cm1 = kc * (p[A] * p[A] - T{1.f});
    return {ca1, ca2, ca3, cm1};
  }
  
  T nextFrame(T x, Coeffs c)
  {
    T v3 = x - state[ic2eq];
    T v1 = c[a1] * state[ic1eq] + c[a2] * v3;
    T v2 = state[ic2eq] + c[a2] * state[ic1eq] + c[a3] * v3;
    state[ic1eq] = T{2.f} * v1 - state[ic1eq];
    state[ic2eq] = T{2.f} * v2 - state[ic2eq];
    return x + c[m1] * v1;
  }
};

// --------------------------------------------------------------------------------
// A one pole filter. see https://ccrma.stanford.edu/~jos/fp/One_Pole.html

template<typename T>
struct OnePole : Filter<T, OnePole<T>>
{
  enum { omega, nParams };
  enum { a0, b1, nCoeffs };
  enum { y1, nStateVars };
  
  using Params = std::array<T, nParams>;
  using Coeffs = std::array<T, nCoeffs>;
  using State = std::array<T, nStateVars>;
  
  Coeffs coeffs{};
  State state{};
  
  static Coeffs passthru() { return {1.f, 0.f}; }
  
  void clear() {
    const Params kDefaultParams{0.f};
    coeffs = makeCoeffs(kDefaultParams);
    state.fill(T{0.f});
  }
  
  static Coeffs makeCoeffs(Params p)
  {
    T x = exp(T{-kTwoPi} * p[omega]);
    return {T{1.f} - x, x};
  }
  
  T nextFrame(T x, Coeffs c)
  {
    state[y1] = c[a0] * x + c[b1] * state[y1];
    return state[y1];
  }
};

// A one-pole, one-zero filter to attenuate DC.
// see https://ccrma.stanford.edu/~jos/fp/DC_Blocker.html

template<typename T>
struct DCBlocker : Filter<T, DCBlocker<T>>
{
  enum { omega, nParams };
  enum { c0, nCoeffs };
  enum { x1, y1, nStateVars };
  
  using Params = std::array<T, nParams>;
  using Coeffs = std::array<T, nCoeffs>;
  using State = std::array<T, nStateVars>;
  
  Coeffs coeffs{};
  State state{};
  
  void clear() {
    const Params kDefaultParams{0.045f};
    coeffs = makeCoeffs(kDefaultParams);
    state.fill(T{0.f});
  }
  
  static Coeffs makeCoeffs(Params p)
  {
    return {cos(p[omega])};
  }
  
  T nextFrame(T x, Coeffs c)
  {
    T y0 = x - state[x1] + c[c0] * state[y1];
    state[y1] = y0;
    state[x1] = x;
    return y0;
  }
};

// First order allpass section with a single sample of delay.
// One-multiply form, see
// https://ccrma.stanford.edu/~jos/pasp/One_Multiply_Scattering_Junctions.html

template<typename T>
struct Allpass1 : Filter<T, Allpass1<T>>
{
  enum { d, nParams };
  enum { c0, nCoeffs };
  enum { x1, y1, nStateVars };
  
  using Params = std::array<T, nParams>;
  using Coeffs = std::array<T, nCoeffs>;
  using State = std::array<T, nStateVars>;
  
  Coeffs coeffs{};
  State state{};
  
  Allpass1() = default;
  
  // construct with a fixed coefficient value (for HalfBandFilter etc.)
  Allpass1(float a) : coeffs{T{a}}, state{} {}
  
  void clear()
  {
    state.fill(T{0.f});
  }
  
  // get allpass coefficient from a delay fraction d.
  // to minimize modulation noise, d should be in the range [0.618 - 1.618].
  static Coeffs makeCoeffs(Params p)
  {
    T xm1 = p[d] - T{1.f};
    return {T{-0.53f} * xm1 + T{0.24f} * xm1 * xm1};
  }
  
  // per-sample with explicit coefficients (used by Filter base)
  T nextFrame(T x, Coeffs c)
  {
    T y = state[x1] + (x - state[y1]) * c[c0];
    state[x1] = x;
    state[y1] = y;
    return y;
  }
  
  // per-sample with stored coefficient (used by HalfBandFilter)
  T nextFrame(T x)
  {
    return nextFrame(x, coeffs);
  }
};

// ----------------------------------------------------------------
// PinkFilter
// Pink noise filter: parallel one-pole bank approximating -3 dB/octave.
// Call init(sampleRate) before use; gains are computed analytically
// so the response tracks the ideal 1/f slope at any sample rate.
// Apply to white noise to produce pink noise.
// Based on Paul Kellet's parallel one-pole approximation.

template<typename T>
struct PinkFilter
{
  static constexpr int kNumPoles = 6;
  static constexpr int kNumTargets = 32;
  static constexpr float kPoleFreqs[kNumPoles] = {
    1.5f, 42.0f, 220.0f, 950.0f, 3300.0f, 9500.0f
  };
  
  std::array<T, kNumPoles> a{};
  std::array<T, kNumPoles> g{};
  std::array<T, kNumPoles> state{};
  
  void clear() { state.fill(T{0.f}); }
  
  void init(float sr)
  {
    // pole coefficients from absolute frequencies
    float af[kNumPoles];
    for (int i = 0; i < kNumPoles; ++i)
      af[i] = expf(-kTwoPi * kPoleFreqs[i] / sr);
    
    // log-spaced target frequencies
    float fTargets[kNumTargets];
    float logMin = logf(5.0f);
    float logMax = logf(sr * 0.45f);
    for (int k = 0; k < kNumTargets; ++k)
      fTargets[k] = expf(logMin + (logMax - logMin) * k / (kNumTargets - 1));
    
    // target: 1/sqrt(f), normalized at midpoint
    float targetMag[kNumTargets];
    float midMag = 1.0f / sqrtf(fTargets[kNumTargets / 2]);
    for (int k = 0; k < kNumTargets; ++k)
      targetMag[k] = (1.0f / sqrtf(fTargets[k])) / midMag;
    
    // complex basis: B[k][i] = 1 / (1 - a_i * e^{-jw_k})
    float Br[kNumTargets][kNumPoles], Bi[kNumTargets][kNumPoles];
    for (int k = 0; k < kNumTargets; ++k)
    {
      float w = kTwoPi * fTargets[k] / sr;
      float cw = cosf(w), sw = sinf(w);
      for (int i = 0; i < kNumPoles; ++i)
      {
        float dr = 1.0f - af[i] * cw;
        float di = af[i] * sw;
        float denom = dr * dr + di * di;
        Br[k][i] = dr / denom;
        Bi[k][i] = -di / denom;
      }
    }
    
    // initial guess
    float gf[kNumPoles];
    float gSum = 0.f;
    for (int i = 0; i < kNumPoles; ++i)
    {
      gf[i] = (1.0f - af[i]) / sqrtf(kPoleFreqs[i]);
      gSum += fabsf(gf[i]);
    }
    for (int i = 0; i < kNumPoles; ++i) gf[i] /= gSum;
    
    // fit
    fitMagnitudeResponse<kNumPoles, kNumTargets>(Br, Bi, targetMag, gf);
    
    // store as T
    for (int i = 0; i < kNumPoles; ++i)
    {
      a[i] = T{af[i]};
      g[i] = T{gf[i]};
    }
  }
  
  T nextFrame(T white)
  {
    T sum = T{0.f};
    for (int i = 0; i < kNumPoles; ++i)
    {
      state[i] = a[i] * state[i] + g[i] * white;
      sum += state[i];
    }
    return sum;
  }
  
  Block<T> operator()(const Block<T>& input)
  {
    Block<T> output;
    for (size_t t = 0; t < kFramesPerBlock; ++t)
      output[t] = nextFrame(input[t]);
    return output;
  }
};
}  // namespace ml

