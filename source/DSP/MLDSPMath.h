// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#pragma once

#include <cmath>
#include "MLDSPMathSIMD.h"
#include "MLDSPMathApprox.h"
#include "MLDSPMathScalar.h"

namespace ml {

// operations we can use in kernels as either float4 or float:
//
// from DSPMathSIMD:
// +, -, *, /, +=, -=, *=, /=, unary-
// min, max, sqrt, rsqrt, rcp
// multiplyAdd
// andBits, andNotBits, orBits, xorBits
// clamp, intPart, fracPart
//
// from MathApprox:
// log, exp, sin, cos, sincos, tan
// sinApprox, cosApprox, expApprox, logApprox, tanhApprox




// scalar definitions in ml namespace so that templates can compile as float and float4
inline float log(float x) { return std::logf(x); }
inline float exp(float x) { return std::expf(x); }
inline float sin(float x) { return std::sinf(x); }
inline float cos(float x) { return std::cosf(x); }
inline std::pair<float, float> sincos(float x) { return{sin(x), cos(x)};
}
inline float tan(float x) { return std::tanf(x); }

}
