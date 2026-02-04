// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#pragma once

// Load definitions for low-level SIMD math.
// These define float4, int4, and a bunch of operations on them.
// We are currently only using 4-element vectors on both SSE and NEON.

#if (defined __ARM_NEON) || (defined __ARM_NEON__)
#include "MLDSPMathNEON.h" // NEON
#else
#include "MLDSPMathSSE.h" // SSE 4.1
#endif
