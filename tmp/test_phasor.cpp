// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// example of RtAudio wrapping low-level madronalib DSP code.

#include "madronalib.h"

using namespace ml;

constexpr size_t kSIMDAlignBytes{16};

constexpr size_t kPartialsPerVoice{64};

constexpr size_t kFloatsPerSIMDVectorBits{2};
constexpr size_t kFloatsPerSIMDVector{1 << kFloatsPerSIMDVectorBits};



// Function to zero an array member, and specializations for our SIMD types
template<typename T>
inline T zero_value() { return T(0); }
template<>
inline float4 zero_value<float4>() { return float4(_mm_setzero_ps()); }
template<>
inline int4 zero_value<int4>() { return int4(_mm_setzero_si128()); }


 

// AlignedArray - to move

template<typename T, size_t N>
struct alignas(kSIMDAlignBytes) AlignedArray
{
  std::array<T, N> dataAligned;
  
  constexpr AlignedArray<T, N>(std::array<T, N> data) : dataAligned(data) {}
  constexpr AlignedArray<T, N>(const T* dataPtr) { std::copy(dataPtr, dataPtr + N, dataAligned.data() );}
  AlignedArray<T, N>(T val) { fill(val); }
  AlignedArray<T, N>() { fill(zero_value<T>()); }
  
  const T& operator[](size_t i) const { return dataAligned[i]; }
  T& operator[](size_t i) { return dataAligned[i]; }
  const T* data() const { return dataAligned.data(); }
  const T* begin() const { return dataAligned.data(); }
  const T* end() const { return dataAligned.data() + N; }
  T* data() { return dataAligned.data(); }
  
  void fill(T f) {dataAligned.fill(f);}
};

template<typename T, size_t COLS>
static constexpr size_t calculatePaddedCols() {
  constexpr size_t bytes_needed = COLS * sizeof(T);
  constexpr size_t aligned_bytes = ((bytes_needed + kSIMDAlignBytes - 1) / kSIMDAlignBytes) * kSIMDAlignBytes;
  return aligned_bytes / sizeof(T);
}

// TEMP as we move from DSPVector to SignalBlock
constexpr size_t kFramesPerBlock = kFloatsPerDSPVector;



using PartialsFrame = AlignedArray<float, kPartialsPerVoice>;

inline std::ostream& operator<< (std::ostream& out, const PartialsFrame& r)
{
  out << "[";
  for(int i=0; i <kPartialsPerVoice; ++i )
  {
	out << r[i] << " ";
  }
  out << "]";
  return out;
}



namespace phasor2Sine
{
constexpr float sqrt2(static_cast<float>(const_math::sqrt(2.0f)));
constexpr float domain(sqrt2 * 4.f);
const float4 domainScaleV{float4(domain)};
const float4 domainOffsetV = float4(-sqrt2);
const float4 sqrt2v = float4(sqrt2);

constexpr float range(sqrt2 - sqrt2 * sqrt2 * sqrt2 / 6.f);
constexpr float oneOverRange(1.0f / range);
const float4 scaleV = float4(1.0f / range);
const float4 flipOffsetV = float4(sqrt2 * 2.f);
const float4 zeroV = float4(0.f);
const float4 oneV = float4(1.f);

constexpr float oneSixth(1.f / 6.f);
const float4 oneSixthV = float4(1.f / 6.f);
};

inline float4 vecPhasorToSine(float4 vx)
{
  using namespace phasor2Sine;
  // scale and offset input phasor on (0, 1) to sine approx domain (-sqrt(2), 3*sqrt(2))
  
  // TEMP
  // float4 omegaV = vx * domainScaleV + domainOffsetV;
  float4 omegaV = vx * float4(_mm_set1_ps(domain)) + float4(_mm_set1_ps(-sqrt2));

  // reverse upper half of phasor to get triangle
  // equivalent to: if (phasor > 0) x = flipOffset - fOmega; else x = fOmega;
  
  // TEMP
  float4 mask = compareGreaterThan(omegaV, float4(_mm_set1_ps(sqrt2)));
  
  
  float4 triangleV = vecSelectFFF(flipOffsetV - omegaV, omegaV, mask);
  
  // convert triangle to sine approx.
  return float4(_mm_set1_ps(oneOverRange)) * triangleV * (float4(_mm_set1_ps(1.0f)) - triangleV * triangleV * float4(_mm_set1_ps(oneSixth)));
}

// ----------------------------------------------------------------
// PartialsFrame operators (float) -> float

#define DEFINE_ARRAY_OP1(opName, opComputation)      \
inline PartialsFrame opName (const PartialsFrame& vx) \
{                                                                    \
PartialsFrame vy;                                           \
const float* px1 = vx.data();                           \
float* py1 = vy.data();                                       \
size_t vectors = kPartialsPerVoice >> kFloatsPerSIMDVectorBits; \
for (int n = 0; n < vectors; ++n)          \
{                                                                  \
float4 x1 = loadFloat4(px1);                               \
storeFloat4(py1, (opComputation));                                  \
px1 += kFloatsPerSIMDVector;                                     \
py1 += kFloatsPerSIMDVector;                                     \
}                                                                  \
return vy;                                                         \
}

DEFINE_ARRAY_OP1(phasorToSine, (vecPhasorToSine(x1)));

// Prevent optimization from eliminating the code
__attribute__((noinline))
PartialsFrame test_function(PartialsFrame input) {
  return phasorToSine(input);
}

// Force the compiler to think we use it
extern "C" void force_use(void* p);

int main() {
  PartialsFrame input;
  PartialsFrame result = test_function(input);
  force_use(&result);  // Opaque function - compiler can't eliminate
  return 0;
}
