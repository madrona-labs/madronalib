// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2022 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#pragma once

// Here is the DSP vector size, an important constant.
constexpr size_t kFloatsPerDSPVectorBits = 6;
constexpr size_t kFloatsPerDSPVector = 1 << kFloatsPerDSPVectorBits;
static_assert((kFloatsPerDSPVectorBits <= 8),
              "We count on kFloatsPerDSPVectorBits to be 8 or less.");

// Load definitions for low-level SIMD math.
// These must define SIMDVectorFloat, SIMDVectorInt, their sizes, and a bunch of
// operations on them. We are currently only using 4-element vectors on both SSE
// and NEON.

#if (defined __ARM_NEON) || (defined __ARM_NEON__)

// NEON

#define ML_SSE_TO_NEON
#include "MLDSPMathNEON.h"

#else

// SSE2

#include "MLDSPMathSSE.h"

#endif
