// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// This module contains the SignalBlockArray / SignalBlock class and basic
// operations on it. Any stateless operations on SignalBlocks should also be added
// here.

#pragma once

#ifdef _WIN32
#include <memory>
#else
// #include <tr1/memory>
#endif

#ifdef __INTEL_COMPILER
#include <mathimf.h>
#endif

#if (defined(_MSC_VER) && _MSC_VER < 1900)
#define snprintf _snprintf
#endif

#include <array>
#include <cstddef>
#include <functional>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <vector>
#include <utility>

#include "MLDSPMath.h"
#include "MLDSPMathApprox.h"
#include "MLDSPScalarMath.h"


namespace ml {


// Here is the signal block size, an important constant. All processing is done in
// chunks of this block size so that loops can be unrolled at compile time.
constexpr size_t kFramesPerBlockBits = 6;
constexpr size_t kFramesPerBlock = 1 << kFramesPerBlockBits;
static_assert((kFramesPerBlockBits <= 8),
              "We count on kFramesPerBlockBits to be 8 or less.");

constexpr size_t kSIMDAlignBytes{16};
constexpr size_t kSIMDVectorsPerBlock{kFramesPerBlock / kSIMDVectorElems};
static_assert((kFramesPerBlock % kSIMDVectorElems == 0),
              "Block size must be a multiple of SIMD vectors.");

constexpr size_t roundUpToMultiple(size_t value, size_t multiple) {
  return ((value + multiple - 1) / multiple) * multiple;
}


// make_array, used in constructors
template <class Function, std::size_t... Indices>
constexpr auto make_array_helper(Function f, std::index_sequence<Indices...>)
-> std::array<typename std::invoke_result<Function, std::size_t>::type, sizeof...(Indices)>
{
  return {{f(Indices)...}};
}
template <int N, class Function>
constexpr auto make_array(Function f)
-> std::array<typename std::invoke_result<Function, std::size_t>::type, N>
{
  return make_array_helper(f, std::make_index_sequence<N>{});
}

// ----------------------------------------------------------------
// AlignedArray
//
template<typename T, size_t N>
struct alignas(kSIMDAlignBytes) AlignedArray
{
  std::array<T, N> dataAligned;
  static_assert(sizeof(T) * N % sizeof(float4) == 0,
                "AlignedArray size must be a multiple of float4 size (16 bytes)");
  
  constexpr AlignedArray<T, N>(std::array<T, N> data) : dataAligned(data) {}
  constexpr AlignedArray<T, N>(const T* dataPtr) { std::copy(dataPtr, dataPtr + N, dataAligned.data() );}
  AlignedArray<T, N>(T val) { fill(val); }
  AlignedArray<T, N>() { fill(T(0.f)); } // TODO: find bugs and remove default fill!
  
  // Constructor from index->value function
  template<typename FN>
  constexpr AlignedArray(FN f) : dataAligned(make_array<N>(f)) {}

  const T& operator[](size_t i) const { return dataAligned[i]; }
  T& operator[](size_t i) { return dataAligned[i]; }
  const T* data() const { return dataAligned.data(); }
  const T* begin() const { return dataAligned.data(); }
  const T* end() const { return dataAligned.data() + N; }
  T* data() { return dataAligned.data(); }
  
  void fill(T f) {dataAligned.fill(f);}
  
  inline AlignedArray& operator+=(const AlignedArray& x1)
  {
    *this = add(*this, x1);
    return *this;
  }
  inline AlignedArray& operator-=(const AlignedArray& x1)
  {
    *this = subtract(*this, x1);
    return *this;
  }
  inline AlignedArray& operator*=(const AlignedArray& x1)
  {
    *this = multiply(*this, x1);
    return *this;
  }
  inline AlignedArray& operator/=(const AlignedArray& x1)
  {
    *this = divide(*this, x1);
    return *this;
  }
  friend inline AlignedArray operator+(const AlignedArray& x1, const AlignedArray& x2)
  {
    return add(x1, x2);
  }
  friend inline AlignedArray operator-(const AlignedArray& x1, const AlignedArray& x2)
  {
    return subtract(x1, x2);
  }
  friend inline AlignedArray operator*(const AlignedArray& x1, const AlignedArray& x2)
  {
    return multiply(x1, x2);
  }
  friend inline AlignedArray operator/(const AlignedArray& x1, const AlignedArray& x2)
  {
    return divide(x1, x2);
  }
};

template <typename T, size_t N>
inline std::ostream& operator<<(std::ostream& out, const AlignedArray<T, N>& aa)
{
  out << "[";
  for (int i = 0; i < N; ++i)
  {
      out << aa[i] << " ";
  }
  out << "] ";

  return out;
}


// ----------------------------------------------------------------
// SignalBlockArray<N>
// N rows of kFramesPerBlock floats, time -> horizontal
// T::scalar_type will let us write templates on SignalBlock, SignalBlock4, ...
// and T::time_step or similar for writing filter functions
// we can also add things like using upsampler_type = Upsampler, and so on

template<size_t N>
struct SignalBlockArray : public AlignedArray<float, N * kFramesPerBlock>
{
  using Base = AlignedArray<float, N * kFramesPerBlock>;
  using scalar_type = float;
  static constexpr size_t height = N;
  static constexpr size_t strideInElems = 1;
  
  SignalBlockArray() : Base() {}
  SignalBlockArray(float val) : Base(val) {}
  constexpr SignalBlockArray(const Base& b) : Base(b) {}
  
  SignalBlockArray<1> getRow(size_t i) const {
    return SignalBlockArray<1>(this->data() + i * kFramesPerBlock);
  }
  void setRow(size_t i, const SignalBlockArray<1>& block) {
    std::copy(block.begin(), block.end(), this->data() + i * kFramesPerBlock);
  }
  
  float* rowPtr(size_t i) {return(this->data() + i * kFramesPerBlock);}
  const float* rowPtr(size_t i) const {return(this->data() + i * kFramesPerBlock);}

};

using SignalBlock = SignalBlockArray<1>;

template<size_t N>
struct SignalBlockArrayInt : public AlignedArray<int32_t, N * kFramesPerBlock>
{
  using Base = AlignedArray<int32_t, N * kFramesPerBlock>;
  using scalar_type = int32_t;
  static constexpr size_t height = N;
  static constexpr size_t strideInElems = 1;
  
  SignalBlockArrayInt() : Base() {}
  SignalBlockArrayInt(int32_t val) : Base(val) {}
  constexpr SignalBlockArrayInt(const Base& b) : Base(b) {}
  
  SignalBlockArrayInt<1> getRow(size_t i) const {
    return SignalBlockArrayInt<1>(this->data() + i * kFramesPerBlock);
  }
  void setRow(size_t i, const SignalBlockArrayInt<1>& block) {
    std::copy(block.begin(), block.end(), this->data() + i * kFramesPerBlock);
  }
  
  int32_t* rowPtr(size_t i) {return(this->data() + i * kFramesPerBlock);}
  const int32_t* rowPtr(size_t i) const {return(this->data() + i * kFramesPerBlock);}
};

using SignalBlockInt = SignalBlockArrayInt<1>;


// ----------------------------------------------------------------
// SignalBlock4Array<N>
// N big "rows" of kFramesPerBlock/4 4x4 blocks of samples containing float4 signals.
// time -> vertical within each 4x4 block, then to next block every 4 frames.
// A0 B0 C0 D0 A4 B4 C4 D4 ...
// A1 B1 C1 D1 A5 B5 C5 D5
// A2 B2 C2 D2 A6 B6 C6 D6
// A3 B3 C3 D3 A7 B7 C7 D7
//
// the data are arranged like this so that transposing each 4x4 block gives
// us four SignalBlocks, and vice versa.

template<size_t N>
struct SignalBlock4Array : public AlignedArray<float4, N * kFramesPerBlock>
{
  using Base = AlignedArray<float4, N * kFramesPerBlock>;
  using scalar_type = float4;
  static constexpr size_t height = 4;
  static constexpr size_t strideInElems = kFramesPerBlock/height;
  
  SignalBlock4Array() : Base() {}
  SignalBlock4Array(float4 val) : Base(val) {}
  SignalBlock4Array(const Base& b) : Base(b) {}
    
  SignalBlock4Array<1> getRow(size_t i) const {
    return SignalBlock4Array<1>(this->data() + i * kFramesPerBlock);
  }
  void setRow(size_t i, const SignalBlock4Array<1>& block) {
    std::copy(block.begin(), block.end(), this->data() + i * kFramesPerBlock);
  }
  float4* rowPtr(size_t i) {return(this->data() + i * kFramesPerBlock);}
  const float4* rowPtr(size_t i) const {return(this->data() + i * kFramesPerBlock);}
};


using SignalBlock4 = SignalBlock4Array<1>;


// ----------------------------------------------------------------
// SignalBlockDynamic: for holding a number of SignalBlocks only known at runtime.

class SignalBlockDynamic final
{
  std::vector<SignalBlock> data_;
  
public:
  SignalBlockDynamic() = default;
  ~SignalBlockDynamic() = default;
  
  explicit SignalBlockDynamic(size_t rows) { data_.resize(rows); }
  void resize(size_t rows) { data_.resize(rows); }
  size_t size() const { return data_.size(); }
  SignalBlock& operator[](int j) { return data_[j]; }
  const SignalBlock& operator[](int j) const { return data_[j]; }
};


// ----------------------------------------------------------------
// Unary operations, (float) -> float

template<typename T, size_t N, typename OP_F2F>
inline AlignedArray<T, N> OpF2F(const AlignedArray<T, N>& a, OP_F2F op) {
  AlignedArray<T, N> result;
  
  constexpr size_t numFloat4s = sizeof(AlignedArray<T, N>) / sizeof(float4);
  const float4* a4 = reinterpret_cast<const float4*>(a.data());
  float4* r4 = reinterpret_cast<float4*>(result.data());
  
  for (size_t i = 0; i < numFloat4s; ++i) {
    r4[i] = op(a4[i]);
  }
  return result;
}

#define DEFINE_OP_F2F(name, expr) \
template<typename T, size_t N> \
inline AlignedArray<T, N> name(const AlignedArray<T, N>& a) { \
return OpF2F(a, [](float4 x) { return (expr); }); \
}

DEFINE_OP_F2F(recipApprox, rcp(x))
DEFINE_OP_F2F(sqrt, sqrt(x))
DEFINE_OP_F2F(sqrtApprox, x * rsqrt(x))
DEFINE_OP_F2F(abs, andNotBits(set1Float(-0.0f), x))

// float sign: -1, 0, or 1
DEFINE_OP_F2F(sign, andBits(orBits(andBits(set1Float(-0.0f), x), set1Float(1.0f)),
                            compareNotEqual(set1Float(-0.0f), x)))

// up/down sign: -1 or 1
DEFINE_OP_F2F(signBit, orBits(andBits(set1Float(-0.0f), x), set1Float(1.0f)))

// Trig, log and exp, using accurate cephes-derived library
DEFINE_OP_F2F(sin, vecSin(x))
DEFINE_OP_F2F(cos, vecCos(x))
DEFINE_OP_F2F(log, vecLog(x))
DEFINE_OP_F2F(exp, vecExp(x))

// Lazy log2 and exp2 from natural log / exp
static const float4 kLogTwoVec{0.69314718055994529f};
static const float4 kLogTwoRVec{1.4426950408889634f};
DEFINE_OP_F2F(log2, vecLog(x) * kLogTwoRVec)
DEFINE_OP_F2F(exp2, vecExp(kLogTwoVec * x))

// Trig, log and exp, using polynomial approximations
DEFINE_OP_F2F(sinApprox, vecSinApprox(x))
DEFINE_OP_F2F(cosApprox, vecCosApprox(x))
DEFINE_OP_F2F(expApprox, vecExpApprox(x))
DEFINE_OP_F2F(logApprox, vecLogApprox(x))

// Lazy log2 and exp2 approximations
DEFINE_OP_F2F(log2Approx, vecLogApprox(x) * kLogTwoRVec)
DEFINE_OP_F2F(exp2Approx, vecExpApprox(kLogTwoVec * x))

// Cubic tanh approx
DEFINE_OP_F2F(tanhApprox, vecTanhApprox(x))

// Fractional part
DEFINE_OP_F2F(fractionalPart, x - intToFloat(floatToIntTruncate(x)))


// ----------------------------------------------------------------
// Binary operations, (float, float) -> float

template<typename T, size_t N, typename OP_FF2F>
inline AlignedArray<T, N> OpFF2F(const AlignedArray<T, N>& a, const AlignedArray<T, N>& b, OP_FF2F op) {
  AlignedArray<T, N> result;
  
  constexpr size_t numFloat4s = sizeof(AlignedArray<T, N>) / sizeof(float4);
  const float4* a4 = reinterpret_cast<const float4*>(a.data());
  const float4* b4 = reinterpret_cast<const float4*>(b.data());
  float4* r4 = reinterpret_cast<float4*>(result.data());
  
  for (size_t i = 0; i < numFloat4s; ++i) {
    r4[i] = op(a4[i], b4[i]);
  }
  return result;
}

#define DEFINE_OP_FF2F(name, expr) \
template<typename T, size_t N> \
inline AlignedArray<T, N> name(const AlignedArray<T, N>& a, const AlignedArray<T, N>& b) { \
return OpFF2F(a, b, [](float4 x, float4 y) { return (expr); }); \
}

DEFINE_OP_FF2F(add, x + y)
DEFINE_OP_FF2F(subtract, x - y)
DEFINE_OP_FF2F(multiply, x * y)
DEFINE_OP_FF2F(divide, x / y)

DEFINE_OP_FF2F(divideApprox, x * rcp(y))
DEFINE_OP_FF2F(pow, vecExp(vecLog(x) * y))
DEFINE_OP_FF2F(powApprox, vecExpApprox(vecLogApprox(x) * y))
DEFINE_OP_FF2F(min, min(x, y))
DEFINE_OP_FF2F(max, max(x, y))


// ----------------------------------------------------------------
// Ternary operation, (float, float, float) -> float

template<typename T, size_t N, typename OP_FFF2F>
inline AlignedArray<T, N> OpFFF2F(const AlignedArray<T, N>& a, const AlignedArray<T, N>& b,
                                  const AlignedArray<T, N>& c, OP_FFF2F op) {
  AlignedArray<T, N> result;
  
  constexpr size_t numFloat4s = sizeof(AlignedArray<T, N>) / sizeof(float4);
  const float4* a4 = reinterpret_cast<const float4*>(a.data());
  const float4* b4 = reinterpret_cast<const float4*>(b.data());
  const float4* c4 = reinterpret_cast<const float4*>(c.data());
  float4* r4 = reinterpret_cast<float4*>(result.data());
  
  for (size_t i = 0; i < numFloat4s; ++i) {
    r4[i] = op(a4[i], b4[i], c4[i]);
  }
  return result;
}

#define DEFINE_OP_FFF2F(name, expr) \
template<typename T, size_t N> \
inline AlignedArray<T, N> name(const AlignedArray<T, N>& a, const AlignedArray<T, N>& b, \
const AlignedArray<T, N>& c) { \
return OpFFF2F(a, b, c, [](float4 x, float4 y, float4 z) { return (expr); }); \
}

DEFINE_OP_FFF2F(lerp, x + (z * (y - x)))        // x = lerp(a, b, mix)
DEFINE_OP_FFF2F(inverseLerp, (z - x) / (y - x)) // mix = inverseLerp(a, b, x)
DEFINE_OP_FFF2F(clamp, min(max(x, y), z))       // clamp(x, minBound, maxBound)
DEFINE_OP_FFF2F(within, andBits(compareGreaterThanOrEqual(x, y), compareLessThan(x, z))) // is x in [y, z)?

// ----------------------------------------------------------------
// Binary operation, (int32, int32) -> int32

template<typename T, size_t N, typename OP_II2I>
inline AlignedArray<T, N> OpII2I(const AlignedArray<T, N>& a, const AlignedArray<T, N>& b, OP_II2I op) {
  AlignedArray<T, N> result;
  
  constexpr size_t numInt4s = sizeof(AlignedArray<T, N>) / sizeof(int4);
  const int4* a4 = reinterpret_cast<const int4*>(a.data());
  const int4* b4 = reinterpret_cast<const int4*>(b.data());
  int4* r4 = reinterpret_cast<int4*>(result.data());
  
  for (size_t i = 0; i < numInt4s; ++i) {
    r4[i] = op(a4[i], b4[i]);
  }
  return result;
}

#define DEFINE_OP_II2I(name, expr) \
template<typename T, size_t N> \
inline AlignedArray<T, N> name(const AlignedArray<T, N>& a, const AlignedArray<T, N>& b) { \
return OpII2I(a, b, [](int4 x, int4 y) { return (expr); }); \
}

DEFINE_OP_II2I(addInt32, x + y)
DEFINE_OP_II2I(subtractInt32, x - y)

// ----------------------------------------------------------------
// Binary operation (float, float) -> float
// Multiple-row and single-row operands

template<size_t ROWS, typename OP_FF2F>
inline SignalBlockArray<ROWS> OpFF2F_MS(const SignalBlockArray<ROWS>& a,
                                        const SignalBlock& b,
                                        OP_FF2F op) {
  SignalBlockArray<ROWS> result;
  
  for (size_t row = 0; row < ROWS; ++row) {
    result[row] = OpFF2F(a[row], b[0], op);
  }
  
  return result;
}

#define DEFINE_OP_FF2F_MS(name, expr) \
template<size_t ROWS> \
inline SignalBlockArray<ROWS> name(const SignalBlockArray<ROWS>& a, \
const SignalBlockArray<1>& b) { \
return OpFF2F_MS(a, b, [](float4 x, float4 y) { return (expr); }); \
}

DEFINE_OP_FF2F_MS(add1, x + y)
DEFINE_OP_FF2F_MS(subtract1, x - y)
DEFINE_OP_FF2F_MS(multiply1, x * y)
DEFINE_OP_FF2F_MS(divide1, x / y)
DEFINE_OP_FF2F_MS(divideApprox1, x * rcp(y))
DEFINE_OP_FF2F_MS(pow1, vecExp(vecLog(x) * y))
DEFINE_OP_FF2F_MS(powApprox1, vecExpApprox(vecLogApprox(x) * y))
DEFINE_OP_FF2F_MS(min1, min(x, y))
DEFINE_OP_FF2F_MS(max1, max(x, y))


// ----------------------------------------------------------------
// Unary operation, (float) -> int

template<typename T, size_t N, typename OP_F2I>
inline AlignedArray<int32_t, N> OpF2I(const AlignedArray<T, N>& a, OP_F2I op) {
  AlignedArray<int32_t, N> result;
  
  constexpr size_t numFloat4s = sizeof(AlignedArray<T, N>) / sizeof(float4);
  const float4* a4 = reinterpret_cast<const float4*>(a.data());
  int4* r4 = reinterpret_cast<int4*>(result.data());
  
  for (size_t i = 0; i < numFloat4s; ++i) {
    r4[i] = op(a4[i]);
  }
  return result;
}

#define DEFINE_OP_F2I(name, expr) \
template<typename T, size_t N> \
inline AlignedArray<int32_t, N> name(const AlignedArray<T, N>& a) { \
return OpF2I(a, [](float4 x) { return (expr); }); \
}

DEFINE_OP_F2I(roundFloatToInt, floatToIntRound(x))
DEFINE_OP_F2I(truncateFloatToInt, floatToIntTruncate(x))


// ----------------------------------------------------------------
// Unary operation, (int) -> float

template<typename T, size_t N, typename OP_I2F>
inline AlignedArray<float, N> OpI2F(const AlignedArray<T, N>& a, OP_I2F op) {
  AlignedArray<float, N> result;
  
  constexpr size_t numInt4s = sizeof(AlignedArray<T, N>) / sizeof(int4);
  const int4* a4 = reinterpret_cast<const int4*>(a.data());
  float4* r4 = reinterpret_cast<float4*>(result.data());
  
  for (size_t i = 0; i < numInt4s; ++i) {
    r4[i] = op(a4[i]);
  }
  return result;
}

#define DEFINE_OP_I2F(name, expr) \
template<typename T, size_t N> \
inline AlignedArray<float, N> name(const AlignedArray<T, N>& a) { \
return OpI2F(a, [](int4 x) { return (expr); }); \
}

DEFINE_OP_I2F(intToFloat, intToFloat(x))
DEFINE_OP_I2F(unsignedIntToFloat, unsignedIntToFloat(x))


// ----------------------------------------------------------------
// Binary operation, (float, float) -> int

template<typename T, size_t N, typename OP_FF2I>
inline AlignedArray<int32_t, N> OpFF2I(const AlignedArray<T, N>& a, const AlignedArray<T, N>& b, OP_FF2I op) {
  AlignedArray<int32_t, N> result;
  
  constexpr size_t numFloat4s = sizeof(AlignedArray<T, N>) / sizeof(float4);
  const float4* a4 = reinterpret_cast<const float4*>(a.data());
  const float4* b4 = reinterpret_cast<const float4*>(b.data());
  int4* r4 = reinterpret_cast<int4*>(result.data());
  
  for (size_t i = 0; i < numFloat4s; ++i) {
    r4[i] = op(a4[i], b4[i]);
  }
  return result;
}

#define DEFINE_OP_FF2I(name, expr) \
template<typename T, size_t N> \
inline AlignedArray<int32_t, N> name(const AlignedArray<T, N>& a, const AlignedArray<T, N>& b) { \
return OpFF2I(a, b, [](float4 x, float4 y) { return castFloatToInt(expr); }); \
}

DEFINE_OP_FF2I(equal, compareEqual(x, y))
DEFINE_OP_FF2I(notEqual, compareNotEqual(x, y))
DEFINE_OP_FF2I(greaterThan, compareGreaterThan(x, y))
DEFINE_OP_FF2I(greaterThanOrEqual, compareGreaterThanOrEqual(x, y))
DEFINE_OP_FF2I(lessThan, compareLessThan(x, y))
DEFINE_OP_FF2I(lessThanOrEqual, compareLessThanOrEqual(x, y))


// ----------------------------------------------------------------
// Ternary operation, (float, float, int) -> float

template<typename T, size_t N, typename OP_FFI2F>
inline AlignedArray<float, N> OpFFI2F(const AlignedArray<T, N>& a, const AlignedArray<T, N>& b,
                                      const AlignedArray<int32_t, N>& c, OP_FFI2F op) {
  AlignedArray<float, N> result;
  
  constexpr size_t numFloat4s = sizeof(AlignedArray<T, N>) / sizeof(float4);
  const float4* a4 = reinterpret_cast<const float4*>(a.data());
  const float4* b4 = reinterpret_cast<const float4*>(b.data());
  const int4* c4 = reinterpret_cast<const int4*>(c.data());
  float4* r4 = reinterpret_cast<float4*>(result.data());
  
  for (size_t i = 0; i < numFloat4s; ++i) {
    r4[i] = op(a4[i], b4[i], c4[i]);
  }
  return result;
}

#define DEFINE_OP_FFI2F(name, expr) \
template<typename T, size_t N> \
inline AlignedArray<float, N> name(const AlignedArray<T, N>& a, const AlignedArray<T, N>& b, \
const AlignedArray<int32_t, N>& c) { \
return OpFFI2F(a, b, c, [](float4 x, float4 y, int4 z) { return (expr); }); \
}

DEFINE_OP_FFI2F(select, vecSelectFFI(x, y, z))  // select(resultIfTrue, resultIfFalse, conditionMask)


// ----------------------------------------------------------------
// Ternary operation, (int, int, int) -> int

template<typename T, size_t N, typename OP_III2I>
inline AlignedArray<int32_t, N> OpIII2I(const AlignedArray<T, N>& a, const AlignedArray<T, N>& b,
                                        const AlignedArray<T, N>& c, OP_III2I op) {
  AlignedArray<int32_t, N> result;
  
  constexpr size_t numInt4s = sizeof(AlignedArray<T, N>) / sizeof(int4);
  const int4* a4 = reinterpret_cast<const int4*>(a.data());
  const int4* b4 = reinterpret_cast<const int4*>(b.data());
  const int4* c4 = reinterpret_cast<const int4*>(c.data());
  int4* r4 = reinterpret_cast<int4*>(result.data());
  
  for (size_t i = 0; i < numInt4s; ++i) {
    r4[i] = op(a4[i], b4[i], c4[i]);
  }
  return result;
}

#define DEFINE_OP_III2I(name, expr) \
template<typename T, size_t N> \
inline AlignedArray<int32_t, N> name(const AlignedArray<T, N>& a, const AlignedArray<T, N>& b, \
const AlignedArray<T, N>& c) { \
return OpIII2I(a, b, c, [](int4 x, int4 y, int4 z) { return (expr); }); \
}

DEFINE_OP_III2I(selectInt, vecSelectIII(x, y, z))  // select(resultIfTrue, resultIfFalse, conditionMask)


// ----------------------------------------------------------------
// load and store

template <size_t ROWS>
inline void load(SignalBlockArray<ROWS>& vecDest, const float* pSrc)
{
  std::copy(pSrc, pSrc + ROWS * kFramesPerBlock, vecDest.data());
}

template <size_t ROWS>
inline void store(const SignalBlockArray<ROWS>& vecSrc, float* pDest)
{
  std::copy(vecSrc.data(), vecSrc.data() + ROWS * kFramesPerBlock, pDest);
}

// ----------------------------------------------------------------
// single-vector horizontal operators returning float

inline float sum(const SignalBlock& x)
{
  const float4* x4 = reinterpret_cast<const float4*>(x.data());
  float sum = 0;
  for (size_t i = 0; i < kFramesPerBlock / 4; ++i)
  {
    sum += vecSumH(x4[i]);
  }
  return sum;
}

inline float max(const SignalBlock& x)
{
  const float4* x4 = reinterpret_cast<const float4*>(x.data());
  float fmax = FLT_MIN;
  for (size_t i = 0; i < kFramesPerBlock / 4; ++i)
  {
    fmax = std::max(fmax, vecMaxH(x4[i]));
  }
  return fmax;
}

inline float min(const SignalBlock& x)
{
  const float4* x4 = reinterpret_cast<const float4*>(x.data());
  float fmin = FLT_MAX;
  for (size_t i = 0; i < kFramesPerBlock / 4; ++i)
  {
    fmin = std::min(fmin, vecMinH(x4[i]));
  }
  return fmin;
}

// ----------------------------------------------------------------
// normalize

template <size_t ROWS>
inline SignalBlockArray<ROWS> normalize(const SignalBlockArray<ROWS>& x)
{
  SignalBlockArray<ROWS> result;
  for (size_t j = 0; j < ROWS; ++j)
  {
    auto inputRow = x.getRow(j);
    result.setRow(j, inputRow / sum(inputRow));
  }
  return result;
}


// ----------------------------------------------------------------
// row-wise operations and conversions

// for the given output ROWS and given an input SignalBlockArray with N rows,
// repeat all the input rows enough times to fill the output SignalBlockArray.
template <size_t ROWS, size_t N>
inline SignalBlockArray<ROWS * N> repeatRows(const SignalBlockArray<N>& x)
{
  SignalBlockArray<ROWS * N> result;
  for (size_t j = 0, k = 0; j < ROWS * N; ++j)
  {
    result.setRow(j, x.getRow(k));
    if (++k >= N) k = 0;
  }
  return result;
}

// Overload for repeating a single SignalBlock
template <size_t ROWS>
inline SignalBlockArray<ROWS> repeatRows(const SignalBlock& x)
{
  SignalBlockArray<ROWS> result;
  for (size_t j = 0; j < ROWS; ++j)
  {
    result.setRow(j, x);
  }
  return result;
}


// for the given ROWS and given an input SignalBlockArray x with N rows,
// stretch x by repeating rows as necessary to make an output SignalBlockArray
// with ROWS rows.
template <size_t ROWS, size_t N>
inline SignalBlockArray<ROWS> stretchRows(const SignalBlockArray<N>& x)
{
  SignalBlockArray<ROWS> result;
  for (size_t j = 0; j < ROWS; ++j)
  {
    size_t k = roundf((j * (N - 1.f)) / (ROWS - 1.f));
    result.setRow(j, x.getRow(k));
  }
  return result;
}

// for the given ROWS and given an input SignalBlockArray x with N rows,
// fill an output array by copying rows of the input, then adding rows of zeros as
// necessary.
template <size_t ROWS, size_t N>
inline SignalBlockArray<ROWS> zeroPadRows(const SignalBlockArray<N>& x)
{
  // default constructor currently zero-fills
  SignalBlockArray<ROWS> result;
  constexpr size_t rowsToCopy = (ROWS < N) ? ROWS : N;
  for (size_t j = 0; j < rowsToCopy; ++j)
  {
    result.setRow(j, x.getRow(j));
  }
  return result;
}

// Shift the array down by the number of rows given in rowsToShift.
// Any rows shifted in from outside the range [0, ROWS) are zeroed. Negative
// shifts are OK.
template <size_t ROWS>
inline SignalBlockArray<ROWS> shiftRows(const SignalBlockArray<ROWS>& x, int rowsToShift)
{
  SignalBlockArray<ROWS> result;
  int k = -rowsToShift;
  for (size_t j = 0; j < ROWS; ++j)
  {
    if (k >= 0 && k < static_cast<int>(ROWS))
    {
      result.setRow(j, x.getRow(k));
    }
    else
    {
      result.setRow(j, SignalBlock(0.f));
    }
    ++k;
  }
  return result;
}

// Rotate the array down by the number of rows given in rowsToRotate.
// Any rows rotated in from outside the range [0, ROWS) are wrapped. Negative
// rotations are OK.
template <size_t ROWS>
inline SignalBlockArray<ROWS> rotateRows(const SignalBlockArray<ROWS>& x, int rowsToRotate)
{
  SignalBlockArray<ROWS> result;
  
  // get start index k to which row 0 is mapped
  int k = modulo(-rowsToRotate, ROWS);
  for (size_t j = 0; j < ROWS; ++j)
  {
    result.setRow(j, x.getRow(k));
    if (++k >= static_cast<int>(ROWS)) k = 0;
  }
  return result;
}

// ----------------------------------------------------------------
// row-wise combining

// Variadic concatRows - concatenate any number of SignalBlockArrays
template<size_t... Ns>
inline SignalBlockArray<(Ns + ...)> concatRows(const SignalBlockArray<Ns>&... arrays)
{
  SignalBlockArray<(Ns + ...)> result;
  size_t offset = 0;
  
  auto copyArray = [&](const auto& arr, size_t rowCount) {
    for (size_t j = 0; j < rowCount; ++j) {
      result.setRow(offset + j, arr.getRow(j));
    }
    offset += rowCount;
  };
  
  // Fold expression to process each array
  (copyArray(arrays, Ns), ...);
  
  return result;
}

// Rotate the elements of each row of a SignalBlockArray by one element left.
// The first element of each row is moved to the end
template <size_t ROWS>
inline SignalBlockArray<ROWS> rotateLeft(const SignalBlockArray<ROWS>& x)
{
  SignalBlockArray<ROWS> result;
  
  for (size_t row = 0; row < ROWS; ++row)
  {
    SignalBlock xRow = x.getRow(row);
    SignalBlock rRow;
    
    const float4* x4 = reinterpret_cast<const float4*>(xRow.data());
    float4* r4 = reinterpret_cast<float4*>(rRow.data());
    
    // Process all but the last float4
    for (size_t n = 0; n < kFramesPerBlock / 4 - 1; ++n)
    {
      r4[n] = vecShuffleLeft(x4[n], x4[n + 1]);
    }
    
    // Wrap around: last float4 uses first float4 for right neighbor
    r4[kFramesPerBlock / 4 - 1] = vecShuffleLeft(x4[kFramesPerBlock / 4 - 1], x4[0]);
    
    result.setRow(row, rRow);
  }
  
  return result;
}

// Rotate the elements of each row of a SignalBlockArray by one element right.
// The last element of each row is moved to the start
template <size_t ROWS>
inline SignalBlockArray<ROWS> rotateRight(const SignalBlockArray<ROWS>& x)
{
  SignalBlockArray<ROWS> result;
  
  for (size_t row = 0; row < ROWS; ++row)
  {
    SignalBlock xRow = x.getRow(row);
    SignalBlock rRow;
    
    const float4* x4 = reinterpret_cast<const float4*>(xRow.data());
    float4* r4 = reinterpret_cast<float4*>(rRow.data());
    
    // First output float4 wraps around with last input float4
    r4[0] = vecShuffleRight(x4[kFramesPerBlock / 4 - 1], x4[0]);
    
    // Process remaining float4s
    for (size_t n = 0; n < kFramesPerBlock / 4 - 1; ++n)
    {
      r4[n + 1] = vecShuffleRight(x4[n], x4[n + 1]);
    }
    
    result.setRow(row, rRow);
  }
  
  return result;
}

// ----------------------------------------------------------------
// separating rows

template <size_t ROWS>
inline SignalBlockArray<(ROWS + 1) / 2> evenRows(const SignalBlockArray<ROWS>& x)
{
  SignalBlockArray<(ROWS + 1) / 2> result;
  for (size_t j = 0; j < (ROWS + 1) / 2; ++j)
  {
    result.setRow(j, x.getRow(j * 2));
  }
  return result;
}

template <size_t ROWS>
inline SignalBlockArray<ROWS / 2> oddRows(const SignalBlockArray<ROWS>& x)
{
  SignalBlockArray<ROWS / 2> result;
  for (size_t j = 0; j < ROWS / 2; ++j)
  {
    result.setRow(j, x.getRow(j * 2 + 1));
  }
  return result;
}

// return the SignalBlockArray consisting of rows [A, B) of the input.
template <size_t A, size_t B, size_t ROWS>
inline SignalBlockArray<B - A> separateRows(const SignalBlockArray<ROWS>& x)
{
  static_assert(B <= ROWS, "separateRows: range out of bounds!");
  static_assert(A < ROWS, "separateRows: range out of bounds!");
  SignalBlockArray<B - A> result;
  for (size_t j = A; j < B; ++j)
  {
    result.setRow(j - A, x.getRow(j));
  }
  return result;
}

// ----------------------------------------------------------------
// add rows to get row-wise sum

template <size_t ROWS>
inline SignalBlock addRows(const SignalBlockArray<ROWS>& x)
{
  SignalBlock result(0.f);
  for (size_t j = 0; j < ROWS; ++j)
  {
    result = add(result, x.getRow(j));
  }
  return result;
}

// ----------------------------------------------------------------
// rowIndex - returns a SignalBlockArray of ROWS rows, each row filled
// with the index of its row

template <size_t ROWS>
inline SignalBlockArray<ROWS> rowIndex()
{
  SignalBlockArray<ROWS> result;
  for (size_t j = 0; j < ROWS; ++j)
  {
    result.setRow(j, SignalBlock(static_cast<float>(j)));
  }
  return result;
}

// ----------------------------------------------------------------
// for testing

inline bool validate(const SignalBlock& x)
{
  for (size_t n = 0; n < kFramesPerBlock; ++n)
  {
    constexpr float maxUsefulValue = 1e12f;
    if (ml::isNaN(x[n]) || (fabs(x[n]) > maxUsefulValue))
    {
      std::cout << "error: " << x[n] << " at index " << n << "\n";
      std::cout << x << "\n";
      return false;
    }
  }
  return true;
}


// ----------------------------------------------------------------
// constexpr array construction helpers

constexpr float intToFloatCastFn(int i) { return static_cast<float>(i); }
constexpr int indexFn(int i) { return i; }

constexpr SignalBlock columnIndex() {
  return SignalBlock(std::array<float, kFramesPerBlock>{
    make_array<kFramesPerBlock>(intToFloatCastFn)
  });
}

template<size_t N>
constexpr SignalBlockArray<N> columnIndex() {
  return repeatRows<N>(columnIndex());
}

constexpr SignalBlockInt columnIndexInt() {
  return SignalBlockInt(std::array<int32_t, kFramesPerBlock>{
    make_array<kFramesPerBlock>(indexFn)
  });
}

// These can't easily be constexpr because they use arithmetic operators
// which rely on SIMD. Keep them as runtime functions:
inline SignalBlock rangeOpen(float start, float end)
{
  float interval = (end - start) / kFramesPerBlock;
  return columnIndex() * SignalBlock(interval) + SignalBlock(start);
}

inline SignalBlock rangeClosed(float start, float end)
{
  float interval = (end - start) / (kFramesPerBlock - 1.f);
  return columnIndex() * SignalBlock(interval) + SignalBlock(start);
}

inline SignalBlock interpolateDSPVectorLinear(float start, float end)
{
  float interval = (end - start) / kFramesPerBlock;
  return columnIndex() * SignalBlock(interval) + SignalBlock(start + interval);
}



}  // namespace ml
