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

namespace ml {


// ----------------------------------------------------------------
// solveLinearSystem<N>
// Solve Ax = b for a small dense system via Gaussian elimination
// with partial pivoting. A is modified in place.

template<int N>
bool solveLinearSystem(float A[N][N], float b[N], float x[N])
{
  // augmented matrix
  float aug[N][N + 1];
  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < N; ++j) aug[i][j] = A[i][j];
    aug[i][N] = b[i];
  }
  
  for (int col = 0; col < N; ++col)
  {
    // partial pivot
    int maxRow = col;
    float maxVal = fabsf(aug[col][col]);
    for (int row = col + 1; row < N; ++row)
    {
      if (fabsf(aug[row][col]) > maxVal)
      {
        maxVal = fabsf(aug[row][col]);
        maxRow = row;
      }
    }
    if (maxVal < 1e-12f) return false; // singular
    
    if (maxRow != col)
    {
      for (int j = 0; j <= N; ++j)
        std::swap(aug[col][j], aug[maxRow][j]);
    }
    
    for (int row = col + 1; row < N; ++row)
    {
      float factor = aug[row][col] / aug[col][col];
      for (int j = col; j <= N; ++j)
        aug[row][j] -= factor * aug[col][j];
    }
  }
  
  for (int i = N - 1; i >= 0; --i)
  {
    x[i] = aug[i][N];
    for (int j = i + 1; j < N; ++j)
      x[i] -= aug[i][j] * x[j];
    x[i] /= aug[i][i];
  }
  return true;
}

// ----------------------------------------------------------------
// fitMagnitudeResponse<N_BASIS, N_TARGETS>
// Given complex basis functions B[k][i] evaluated at N_TARGETS frequencies,
// and a target magnitude spectrum, find real gains g[i] that minimize
// the dB error between |sum_i g_i * B_i(f_k)| and target[k].
// Uses iterative phase retrieval with linear least squares.

template<int N_BASIS, int N_TARGETS>
void fitMagnitudeResponse(
                          const float Br[N_TARGETS][N_BASIS],  // real part of basis
                          const float Bi[N_TARGETS][N_BASIS],  // imaginary part of basis
                          const float targetMag[N_TARGETS],     // target magnitude
                          float g[N_BASIS],                     // output gains (also used as initial guess)
                          int nIters = 10)
{
  constexpr int N = N_BASIS;
  constexpr int M = N_TARGETS;
  
  // precompute B^T * B (constant across iterations)
  float BtB[N][N] = {};
  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < N; ++j)
    {
      float sum = 0.f;
      for (int k = 0; k < M; ++k)
        sum += Br[k][i] * Br[k][j] + Bi[k][i] * Bi[k][j];
      BtB[i][j] = sum;
    }
  }
  
  for (int iter = 0; iter < nIters; ++iter)
  {
    // 1. evaluate H(f) with current gains, project target onto phase
    float Tr[M], Ti[M];
    for (int k = 0; k < M; ++k)
    {
      float hr = 0.f, hi = 0.f;
      for (int i = 0; i < N; ++i)
      {
        hr += g[i] * Br[k][i];
        hi += g[i] * Bi[k][i];
      }
      float mag = sqrtf(hr * hr + hi * hi);
      if (mag > 1e-12f)
      {
        float scale = targetMag[k] / mag;
        Tr[k] = hr * scale;
        Ti[k] = hi * scale;
      }
      else
      {
        Tr[k] = targetMag[k];
        Ti[k] = 0.f;
      }
    }
    
    // 2. form B^T * T and solve
    float BtT[N] = {};
    for (int i = 0; i < N; ++i)
    {
      for (int k = 0; k < M; ++k)
        BtT[i] += Br[k][i] * Tr[k] + Bi[k][i] * Ti[k];
    }
    
    float A[N][N];
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        A[i][j] = BtB[i][j];
    
    solveLinearSystem<N>(A, BtT, g);
  }
}

}  // namespace ml

