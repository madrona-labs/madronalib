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


// Here is the signal block size, an important constant. All processing is done in
// chunks of this block size so that loops can be unrolled at compile time.
constexpr size_t kFramesPerBlockBits = 6;
constexpr size_t kFramesPerBlock = 1 << kFramesPerBlockBits;
static_assert((kFramesPerBlockBits <= 8),
              "We count on kFramesPerBlockBits to be 8 or less.");


constexpr size_t kSIMDAlignBytes{16};

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

template<typename T, size_t N>
struct alignas(kSIMDAlignBytes) AlignedArray
{
  std::array<T, N> dataAligned;
  
  constexpr AlignedArray<T, N>(std::array<T, N> data) : dataAligned(data) {}
  constexpr AlignedArray<T, N>(const T* dataPtr) { std::copy(dataPtr, dataPtr + N, dataAligned.data() );}
  AlignedArray<T, N>(T val) { fill(val); }
  AlignedArray<T, N>() { fill(T(0.f)); }
  
  const T& operator[](size_t i) const { return dataAligned[i]; }
  T& operator[](size_t i) { return dataAligned[i]; }
  const T* data() const { return dataAligned.data(); }
  const T* begin() const { return dataAligned.data(); }
  const T* end() const { return dataAligned.data() + N; }
  T* data() { return dataAligned.data(); }
  
  void fill(T f) {dataAligned.fill(f);}
};


// ----------------------------------------------------------------
// AlignedArray2D


template<typename T, size_t COLS>
static constexpr size_t calculatePaddedCols() {
  constexpr size_t bytes_needed = COLS * sizeof(T);
  constexpr size_t aligned_bytes = ((bytes_needed + kSIMDAlignBytes - 1) / kSIMDAlignBytes) * kSIMDAlignBytes;
  return aligned_bytes / sizeof(T);
}

template<typename T, size_t COLS, size_t ROWS>
struct AlignedArray2D : public AlignedArray<T, calculatePaddedCols<T, COLS>()*ROWS>
{
  static_assert(kSIMDAlignBytes % sizeof(T) == 0, "Type size must evenly divide alignment boundary");
  static constexpr size_t kRowStride = calculatePaddedCols<T, COLS>();
  using Base = AlignedArray<T, kRowStride*ROWS>;
  
  // Constructors
  AlignedArray2D() : Base() {}
  AlignedArray2D(T val) : Base(val) {}
  
  // get data ptr
  T* data() { return Base::data(); }
  
  // 2D access - row major order
  T& operator()(size_t row, size_t col) {
    assert(row < ROWS && col < COLS);
    return this->dataAligned[row * kRowStride + col];
  }
  
  const T& operator()(size_t row, size_t col) const {
    assert(row < ROWS && col < COLS);
    return this->dataAligned[row * kRowStride + col];
  }
  
  // Get pointer to start of a row (will be 16-byte aligned)
  T* row(size_t r) {
    assert(r < ROWS);
    return &this->dataAligned[r * kRowStride];
  }
  
  const T* row(size_t r) const {
    assert(r < ROWS);
    return &this->dataAligned[r * kRowStride];
  }
  
  AlignedArray<T, COLS> getRow(size_t r) const {
    assert(r < ROWS);
    AlignedArray<T, COLS> result;
    std::copy(row(r), row(r) + COLS, result.data());
    return result;
  }
  
  void setRow(size_t r, const AlignedArray<T, COLS>& src)  {
    assert(r < ROWS);
    std::copy(src.begin(), src.end(), this->row(r));
  }
  

  AlignedArray<T, ROWS> getColumn(size_t i)
  {
    AlignedArray<T, ROWS> r;
    for(int j=0; j<ROWS; ++j)
    {
      r[j] = row(j)[i];
    }
    return r;
  }
  
  void setColumn(size_t i, AlignedArray<T, ROWS> x)
  {
    for(int j=0; j<ROWS; ++j)
    {
      row(j)[i] = x[j];
    }
  }
  
  // Get actual dimensions
  constexpr size_t rows() const { return ROWS; }
  constexpr size_t cols() const { return COLS; }
  constexpr size_t rowStride() const { return kRowStride; }
};














// ----------------------------------------------------------------
// SignalBlockArray
//
// An array of SignalBlocks. Each SignalBlock holds kFramesPerBlock float32 samples.
// A SignalBlockArray< ROWS > holds kFramesPerBlock*ROWS samples.
// Knowing the array size at compile time helps efficiency by allowing the compiler to
// unroll loops.


namespace ml
{

constexpr size_t kSIMDVectorsPerDSPVector = kFramesPerBlock / 4;
constexpr size_t kBytesPerSIMDVector = 4 * sizeof(float);
constexpr size_t kSIMDVectorMask = ~(kBytesPerSIMDVector - 1);

inline bool isSIMDAligned(float* p) {
  uintptr_t pM = (uintptr_t)p;
  return ((pM & kSIMDVectorMask) == 0);
}


template <size_t ROWS>
class SignalBlockArray
{
  union Data
  {
    float4 _align[kSIMDVectorsPerDSPVector * ROWS];   // unused except to force alignment
    std::array<float, kFramesPerBlock * ROWS> arrayData_;  // for constexpr ctor
    float asFloat[kFramesPerBlock * ROWS];
    
    Data() {}
    constexpr Data(std::array<float, kFramesPerBlock * ROWS> a) : arrayData_(a) {}
  };
  Data data_;
  
public:
  // getBuffer, getConstBuffer
  inline float* getBuffer() { return data_.asFloat; }
  inline const float* getConstBuffer() const { return data_.asFloat; }
  
  // constexpr constructor taking a std::array. Use with make_array
  constexpr SignalBlockArray(std::array<float, kFramesPerBlock * ROWS> a) : data_(a) {}
  
  // constexpr constructor taking a function(int -> float)
  constexpr SignalBlockArray(float (*fn)(int))
  : SignalBlockArray(make_array<kFramesPerBlock * ROWS>(fn))
  {
  }
  
  // TODO constexpr constructor taking a Projection - requires Projection
  // rewrite without std::function
  
  // default constructor: zeroes the data.
  // TODO this seems to be taking a lot of time! investigate
  constexpr SignalBlockArray() { data_.arrayData_.fill(0.f); }
  
  // conversion constructor to float.  This keeps the syntax of common DSP code
  // shorter: "va + SignalBlock(1.f)" becomes just "va + 1.f".
  constexpr SignalBlockArray(float k) { operator=(k); }
  
  // unaligned data * ctors
  explicit SignalBlockArray(float* pData) { load(*this, pData); }
  explicit SignalBlockArray(const float* pData) { load(*this, pData); }
  
  // aligned data * ctors
  explicit SignalBlockArray(SignalBlockArray* pData) { loadAligned(*this, pData); }
  explicit SignalBlockArray(const SignalBlockArray* pData) { loadAligned(*this, pData); }
  
  inline float& operator[](size_t i) { return getBuffer()[i]; }
  inline const float operator[](size_t i) const { return getConstBuffer()[i]; }
  
  // = float: set each element of the SignalBlockArray to the float value k.
  inline SignalBlockArray operator=(float k)
  {
    const float4 vk(k);
    float* py1 = getBuffer();
    
    for (int n = 0; n < kSIMDVectorsPerDSPVector * ROWS; ++n)
    {
      storeFloat4(py1, vk);
      py1 += 4;
    }
    return *this;
  }
  
  // default copy and = constructors.
  
  SignalBlockArray(const SignalBlockArray& x1) noexcept = default;
  SignalBlockArray& operator=(const SignalBlockArray& x1) noexcept = default;
  
  // equality by value
  bool operator==(const SignalBlockArray& x1) const
  {
    const float* px1 = x1.getConstBuffer();
    const float* py1 = getConstBuffer();
    
    for (int n = 0; n < kFramesPerBlock * ROWS; ++n)
    {
      if (py1[n] != px1[n]) return false;
    }
    return true;
  }
  
  // TODO compare the performance of getting rid of everything between here and row(), and
  // just using row() and constRow() for reading and writing
  
  // return row J from this SignalBlockArray, when J is known at compile time.
  template <int J>
  inline SignalBlockArray<1> getRowVector() const
  {
    static_assert((J >= 0) && (J < ROWS), "getRowVector index out of bounds");
    return getRowVectorUnchecked(J);
  }
  
  // set row J of this SignalBlockArray to x1, when J is known at compile time.
  template <int J>
  inline void setRowVector(const SignalBlockArray<1> x1)
  {
    static_assert((J >= 0) && (J < ROWS), "setRowVector index out of bounds");
    setRowVectorUnchecked(J, x1);
  }
  
  // get a row vector j when j is not known at compile time.
  inline SignalBlockArray<1> getRowVectorUnchecked(size_t j) const
  {
    SignalBlockArray<1> vy;
    const float* px1 = getConstBuffer() + kFramesPerBlock * j;
    float* py1 = vy.getBuffer();
    
    for (int n = 0; n < kSIMDVectorsPerDSPVector; ++n)
    {
      storeFloat4(py1, loadFloat4(px1));
      px1 += 4;
      py1 += 4;
    }
    return vy;
  }
  
  // set a row vector j when j is not known at compile time.
  inline void setRowVectorUnchecked(size_t j, const SignalBlockArray<1> x1)
  {
    const float* px1 = x1.getConstBuffer();
    float* py1 = getBuffer() + kFramesPerBlock * j;
    
    for (int n = 0; n < kSIMDVectorsPerDSPVector; ++n)
    {
      storeFloat4(py1, loadFloat4(px1));
      px1 += 4;
      py1 += 4;
    }
  }
  
  // return a const pointer to the first element in row J of this
  // SignalBlockArray.
  inline const float* getRowDataConst(int j) const
  {
    const float* py1 = getConstBuffer() + kFramesPerBlock * j;
    return py1;
  }
  
  // return a pointer to the first element in row J of this SignalBlockArray.
  inline float* getRowData(int j)
  {
    float* py1 = getBuffer() + kFramesPerBlock * j;
    return py1;
  }
  
  // return a reference to a row of this SignalBlockArray.
  inline SignalBlockArray<1>& row(int j)
  {
    float* py1 = getBuffer() + kFramesPerBlock * j;
    SignalBlockArray<1>* pRow = reinterpret_cast<SignalBlockArray<1>*>(py1);
    return *pRow;
  }
  
  // return a const reference to a row of this SignalBlockArray.
  inline const SignalBlockArray<1>& constRow(int j) const
  {
    const float* py1 = getConstBuffer() + kFramesPerBlock * j;
    const SignalBlockArray<1>* pRow = reinterpret_cast<const SignalBlockArray<1>*>(py1);
    return *pRow;
  }
  
  inline SignalBlockArray& operator+=(const SignalBlockArray& x1)
  {
    *this = add(*this, x1);
    return *this;
  }
  inline SignalBlockArray& operator-=(const SignalBlockArray& x1)
  {
    *this = subtract(*this, x1);
    return *this;
  }
  inline SignalBlockArray& operator*=(const SignalBlockArray& x1)
  {
    *this = multiply(*this, x1);
    return *this;
  }
  inline SignalBlockArray& operator/=(const SignalBlockArray& x1)
  {
    *this = divide(*this, x1);
    return *this;
  }
  
  // binary operators - defining these here inside the class as non-template
  // functions enables the compiler to call implicit conversions on either
  // argument.
  
  friend inline SignalBlockArray operator+(const SignalBlockArray& x1, const SignalBlockArray& x2)
  {
    return add(x1, x2);
  }
  friend inline SignalBlockArray operator-(const SignalBlockArray& x1)
  {
    // TODO should be able to declare this constexpr!
    SignalBlockArray zero(0.f);
    
    return subtract(zero, x1);
  }
  friend inline SignalBlockArray operator-(const SignalBlockArray& x1, const SignalBlockArray& x2)
  {
    return subtract(x1, x2);
  }
  friend inline SignalBlockArray operator*(const SignalBlockArray& x1, const SignalBlockArray& x2)
  {
    return multiply(x1, x2);
  }
  friend inline SignalBlockArray operator/(const SignalBlockArray& x1, const SignalBlockArray& x2)
  {
    return divide(x1, x2);
  }
};  // class SignalBlockArray

// ----------------------------------------------------------------
// SignalBlock
//
// Special case of the SignalBlockArray with one row.
// The most common data type in a typical DSP function.

typedef SignalBlockArray<1> SignalBlock;

// ----------------------------------------------------------------
// SignalBlockArrayInt
//
// DSP Vector holding int32 values.

constexpr size_t kIntsPerDSPVector = kFramesPerBlock;

template <size_t ROWS>
class SignalBlockArrayInt
{
  union Data
  {
    int4 _align[kSIMDVectorsPerDSPVector * ROWS];     // unused except to force alignment
    std::array<int32_t, kIntsPerDSPVector * ROWS> arrayData_;  // for constexpr ctor
    int32_t asInt[kIntsPerDSPVector * ROWS];
    float asFloat[kFramesPerBlock * ROWS];
    
    Data() {}
    constexpr Data(std::array<int32_t, kIntsPerDSPVector * ROWS> a) : arrayData_(a) {}
  };
  
public:
  Data data_;
  
  // getBuffer, getConstBuffer
  inline float* getBuffer() { return data_.asFloat; }
  inline const float* getConstBuffer() const { return data_.asFloat; }
  inline int32_t* getBufferInt() { return data_.asInt; }
  inline const int32_t* getConstBufferInt() const { return data_.asInt; }
  
  explicit SignalBlockArrayInt() { operator=(0); }
  explicit SignalBlockArrayInt(int32_t k) { operator=(k); }
  
  inline int32_t& operator[](int i) { return getBufferInt()[i]; }
  inline const int32_t operator[](int i) const { return getConstBufferInt()[i]; }
  
  // set each element of the SignalBlockArray to the int32_t value k.
  inline SignalBlockArrayInt operator=(int32_t k)
  {
    int4 i4 = set1Int(k);
    float4 vk = castIntToFloat(i4);
    int32_t* py1 = getBufferInt();
    
    for (int n = 0; n < kSIMDVectorsPerDSPVector * ROWS; ++n)
    {
      storeFloat4(reinterpret_cast<float*>(py1), vk);
      py1 += 4;
    }
    return *this;
  }
  
  // constexpr constructor taking a std::array. Use with make_array
  constexpr SignalBlockArrayInt(std::array<int32_t, kFramesPerBlock * ROWS> a) : data_(a) {}
  
  // constexpr constructor taking a function(int -> int)
  constexpr SignalBlockArrayInt(int (*fn)(int))
  : SignalBlockArrayInt(make_array<kFramesPerBlock * ROWS>(fn))
  {
  }
  
  SignalBlockArrayInt(const SignalBlockArrayInt& x1) noexcept = default;
  SignalBlockArrayInt& operator=(const SignalBlockArrayInt& x1) noexcept = default;
  
  // equality by value
  bool operator==(const SignalBlockArrayInt& x1)
  {
    const int* px1 = x1.getConstBufferInt();
    const int* py1 = getConstBufferInt();
    
    for (int n = 0; n < kIntsPerDSPVector * ROWS; ++n)
    {
      if (py1[n] != px1[n]) return false;
    }
    return true;
  }
  
  // return a reference to a row of this SignalBlockArrayInt.
  inline SignalBlockArrayInt<1>& row(int j)
  {
    float* py1 = getBuffer() + kIntsPerDSPVector * j;
    SignalBlockArrayInt<1>* pRow = reinterpret_cast<SignalBlockArrayInt<1>*>(py1);
    return *pRow;
  }
  
  // return a reference to a row of this SignalBlockArrayInt.
  inline const SignalBlockArrayInt<1>& constRow(int j) const
  {
    const float* py1 = getConstBuffer() + kIntsPerDSPVector * j;
    const SignalBlockArrayInt<1>* pRow = reinterpret_cast<const SignalBlockArrayInt<1>*>(py1);
    return *pRow;
  }
  
  friend inline SignalBlockArrayInt operator+(const SignalBlockArrayInt& x1,
                                            const SignalBlockArrayInt& x2)
  {
    return addInt32(x1, x2);
  }
  
  friend inline SignalBlockArrayInt operator-(const SignalBlockArrayInt& x1)
  {
    SignalBlockArrayInt zero(0);
    return subtractInt32(zero, x1);
  }
  
  friend inline SignalBlockArrayInt operator-(const SignalBlockArrayInt& x1,
                                            const SignalBlockArrayInt& x2)
  {
    return subtractInt32(x1, x2);
  }
  
};  // class SignalBlockArrayInt

typedef SignalBlockArrayInt<1> SignalBlockInt;

// ----------------------------------------------------------------
// SignalBlockDynamic: for holding a number of SignalBlocks only known at runtime.

class SignalBlockDynamic final
{
public:
  SignalBlockDynamic() = default;
  ~SignalBlockDynamic() = default;
  
  explicit SignalBlockDynamic(size_t rows) { data_.resize(rows); }
  void resize(size_t rows) { data_.resize(rows); }
  size_t size() const { return data_.size(); }
  SignalBlock& operator[](int j) { return data_[j]; }
  const SignalBlock& operator[](int j) const { return data_[j]; }
  
private:
  std::vector<SignalBlock> data_;
};

// ----------------------------------------------------------------
// load and store

// some loads and stores may be unaligned, let std::copy handle this
template <size_t ROWS>
inline void load(SignalBlockArray<ROWS>& vecDest, const float* pSrc)
{
  std::copy(pSrc, pSrc + kFramesPerBlock * ROWS, vecDest.getBuffer());
}

template <size_t ROWS>
inline void store(const SignalBlockArray<ROWS>& vecSrc, float* pDest)
{
  std::copy(vecSrc.getConstBuffer(), vecSrc.getConstBuffer() + kFramesPerBlock * ROWS, pDest);
}

// if the pointers are known to be aligned, copy as SIMD vectors
template <size_t ROWS>
inline void loadAligned(SignalBlockArray<ROWS>& vecDest, const float* pSrc)
{
  const float* px1 = pSrc;
  float* py1 = vecDest.getBuffer();
  
  for (int n = 0; n < kSIMDVectorsPerDSPVector * ROWS; ++n)
  {
    storeFloat4(py1, loadFloat4(px1));
    px1 += 4;
    py1 += 4;
  }
}

template <size_t ROWS>
inline void storeAligned(const SignalBlockArray<ROWS>& vecSrc, float* pDest)
{
  const float* px1 = vecSrc.getConstBuffer();
  float* py1 = pDest;
  
  for (int n = 0; n < kSIMDVectorsPerDSPVector * ROWS; ++n)
  {
    storeFloat4(py1, loadFloat4(px1));
    px1 += 4;
    py1 += 4;
  }
}

// ----------------------------------------------------------------
// unary vector operators (float4) -> float4

#define DEFINE_OP1(opName, opComputation)                              \
template <size_t ROWS>                                               \
inline SignalBlockArray<ROWS>(opName)(const SignalBlockArray<ROWS>& vx1) \
{                                                                    \
SignalBlockArray<ROWS> vy;                                           \
const float* px1 = vx1.getConstBuffer();                           \
float* py1 = vy.getBuffer();                                       \
for (int n = 0; n < kSIMDVectorsPerDSPVector * ROWS; ++n)          \
{                                                                  \
float4 x = loadFloat4(px1);                                \
float4 y = (opComputation); \
storeFloat4(py1, y);                                  \
px1 += 4;                                     \
py1 += 4;                                     \
}                                                                  \
return vy;                                                         \
}
DEFINE_OP1(recipApprox, (rcp(x)));
DEFINE_OP1(sqrt, (sqrt(x)));
DEFINE_OP1(sqrtApprox, (x * rsqrt(x)));
DEFINE_OP1(abs, (andNotBits(set1Float(-0.0f), x)));

// float sign: -1, 0, or 1
DEFINE_OP1(sign, (andBits(orBits(andBits(set1Float(-0.0f), x), set1Float(1.0f)),
                          compareNotEqual(set1Float(-0.0f), x))));

// up/down sign: -1 or 1
DEFINE_OP1(signBit, (orBits(andBits(set1Float(-0.0f), x), set1Float(1.0f))));

// trig, log and exp, using accurate cephes-derived library
DEFINE_OP1(sin, (vecSin(x)));
DEFINE_OP1(cos, (vecCos(x)));
DEFINE_OP1(log, (vecLog(x)));
DEFINE_OP1(exp, (vecExp(x)));

// lazy log2 and exp2 from natural log / exp
static const float4 kLogTwoVec{0.69314718055994529f};
static const float4 kLogTwoRVec{1.4426950408889634f};
DEFINE_OP1(log2, (vecLog(x) * kLogTwoRVec));
DEFINE_OP1(exp2, (vecExp(kLogTwoVec * x)));

// trig, log and exp, using polynomial approximations
DEFINE_OP1(sinApprox, (vecSinApprox(x)));
DEFINE_OP1(cosApprox, (vecCosApprox(x)));
DEFINE_OP1(expApprox, (vecExpApprox(x)));
DEFINE_OP1(logApprox, (vecLogApprox(x)));

// lazy log2 and exp2 approximations from log / exp approximations
DEFINE_OP1(log2Approx, (vecLogApprox(x) * kLogTwoRVec));
DEFINE_OP1(exp2Approx, (vecExpApprox(kLogTwoVec * x)));

// cubic tanh approx
DEFINE_OP1(tanhApprox, (vecTanhApprox(x)));

// ----------------------------------------------------------------
// binary vector operators (float, float) -> float

#define DEFINE_OP2(opName, opComputation)                              \
template <size_t ROWS>                                               \
inline SignalBlockArray<ROWS>(opName)(const SignalBlockArray<ROWS>& vx1, \
const SignalBlockArray<ROWS>& vx2) \
{                                                                    \
SignalBlockArray<ROWS> vy;                                           \
const float* px1 = vx1.getConstBuffer();                           \
const float* px2 = vx2.getConstBuffer();                           \
float* py1 = vy.getBuffer();                                       \
for (int n = 0; n < kSIMDVectorsPerDSPVector * ROWS; ++n)          \
{                                                                  \
float4 x1 = loadFloat4(px1);                               \
float4 x2 = loadFloat4(px2);                               \
auto y = (opComputation); \
storeFloat4(py1, y);                                  \
px1 += 4;                                     \
px2 += 4;                                     \
py1 += 4;                                     \
}                                                                  \
return vy;                                                         \
}

DEFINE_OP2(add, (x1 + x2));
DEFINE_OP2(subtract, (x1 - x2));
DEFINE_OP2(multiply, (x1 * x2));
DEFINE_OP2(divide, (x1 / x2));

DEFINE_OP2(divideApprox, (x1 * rcp(x2)));
DEFINE_OP2(pow, (vecExp(vecLog(x1) * x2)));
DEFINE_OP2(powApprox, (vecExpApprox(vecLogApprox(x1) * x2)));
DEFINE_OP2(min, (min(x1, x2)));
DEFINE_OP2(max, (max(x1, x2)));

// ----------------------------------------------------------------
// binary vector operators (float, float) -> float
// from multiple-row and single-row operands

#define DEFINE_OP2_MS(opName, opComputation)                           \
template <size_t ROWS>                                               \
inline SignalBlockArray<ROWS>(opName)(const SignalBlockArray<ROWS>& vx1, \
const SignalBlockArray<1>& vx2)    \
{                                                                    \
SignalBlockArray<ROWS> vy;                                           \
const float* px1 = vx1.getConstBuffer();                           \
const float* px2 = vx2.getConstBuffer();                           \
float* py1 = vy.getBuffer();                                       \
size_t px2Offset = 0;                                              \
for (int n = 0; n < kSIMDVectorsPerDSPVector * ROWS; ++n)          \
{                                                                  \
float4 x1 = loadFloat4(px1);                               \
float4 x2 = loadFloat4(px2 + px2Offset);                   \
auto y = (opComputation); \
storeFloat4(py1, y);                                  \
px1 += 4;                                     \
px2Offset += 4;                               \
px2Offset &= kFramesPerBlock - 1;                            \
py1 += 4;                                     \
}                                                                  \
return vy;                                                         \
}

DEFINE_OP2_MS(add1, (x1 + x2));
DEFINE_OP2_MS(subtract1, (x1 - x2));
DEFINE_OP2_MS(multiply1, (x1 * x2));
DEFINE_OP2_MS(divide1, (x1 / x2));

DEFINE_OP2_MS(divideApprox1, (x1 * rcp(x2)));
DEFINE_OP2_MS(pow1, (vecExp(vecLog(x1) * x2)));
DEFINE_OP2_MS(powApprox1, (vecExpApprox(vecLogApprox(x1) * x2)));
DEFINE_OP2_MS(min1, (min(x1, x2)));
DEFINE_OP2_MS(max1, (max(x1, x2)));


// ----------------------------------------------------------------
// binary vector operators (int32, int32) -> int32

#define DEFINE_OP2_INT32(opName, opComputation)                              \
template <size_t ROWS>                                                     \
inline SignalBlockArrayInt<ROWS>(opName)(const SignalBlockArrayInt<ROWS>& vx1, \
const SignalBlockArrayInt<ROWS>& vx2) \
{                                                                          \
SignalBlockArrayInt<ROWS> vy;                                              \
const float* px1 = vx1.getConstBuffer();                                 \
const float* px2 = vx2.getConstBuffer();                                 \
float* py1 = vy.getBuffer();                                             \
for (int n = 0; n < kSIMDVectorsPerDSPVector * ROWS; ++n)                \
{                                                                        \
int4 x1 = castFloatToInt(loadFloat4(px1));                               \
int4 x2 = castFloatToInt(loadFloat4(px2));                               \
auto y = castIntToFloat(opComputation); \
storeFloat4(py1, y);                                  \
px1 += 4;                                             \
px2 += 4;                                             \
py1 += 4;                                             \
}                                                                        \
return vy;                                                               \
}

DEFINE_OP2_INT32(subtractInt32, (x1 - x2));
DEFINE_OP2_INT32(addInt32, (x1 + x2));

// ----------------------------------------------------------------
// ternary vector operators (float, float, float) -> float

#define DEFINE_OP3(opName, opComputation)                              \
template <size_t ROWS>                                               \
inline SignalBlockArray<ROWS>(opName)(const SignalBlockArray<ROWS>& vx1, \
const SignalBlockArray<ROWS>& vx2, \
const SignalBlockArray<ROWS>& vx3) \
{                                                                    \
SignalBlockArray<ROWS> vy;                                           \
const float* px1 = vx1.getConstBuffer();                           \
const float* px2 = vx2.getConstBuffer();                           \
const float* px3 = vx3.getConstBuffer();                           \
float* py1 = vy.getBuffer();                                       \
for (int n = 0; n < kSIMDVectorsPerDSPVector * ROWS; ++n)          \
{                                                                  \
float4 x1 = loadFloat4(px1);                               \
float4 x2 = loadFloat4(px2);                               \
float4 x3 = loadFloat4(px3);                               \
auto y = (opComputation); \
storeFloat4(py1, y);                                  \
px1 += 4;                                     \
px2 += 4;                                     \
px3 += 4;                                     \
py1 += 4;                                     \
}                                                                  \
return vy;                                                         \
}

DEFINE_OP3(lerp, (x1 + (x3 * (x2 - x1))));       // x = lerp(a, b, mix)
DEFINE_OP3(inverseLerp, ((x3 - x1) / (x2 - x1)));  // mix = inverseLerp(a, b, x)

DEFINE_OP3(clamp, (min(max(x1, x2), x3)));    // clamp(x, minBound, maxBound)
DEFINE_OP3(within, (andBits(compareGreaterThanOrEqual(x1, x2), compareLessThan(x1, x3))));  // is x in the open interval [x2, x3) ?


// ----------------------------------------------------------------
// lerp two vectors with scalar float mixture (constant over each vector)

template <size_t ROWS>
inline SignalBlockArray<ROWS> lerp(const SignalBlockArray<ROWS>& vx1, const SignalBlockArray<ROWS>& vx2,
                                 float m)
{
  SignalBlockArray<ROWS> vy;
  const float* px1 = vx1.getConstBuffer();
  const float* px2 = vx2.getConstBuffer();
  SignalBlock vmix(m);
  float* py1 = vy.getBuffer();
  const float4 vConstMix = set1Float(m);
  
  for (int n = 0; n < kSIMDVectorsPerDSPVector * ROWS; ++n)
  {
    float4 x1 = loadFloat4(px1);
    float4 x2 = loadFloat4(px2);
    float4 mix = x1 + (vConstMix * (x2 - x1));
    storeFloat4(py1, mix);
    px1 += 4;
    px2 += 4;
    py1 += 4;
  }
  return vy;
}


// ----------------------------------------------------------------
// vector operators (float) -> int

#define DEFINE_OP1_F2I(opName, opComputation)                             \
template <size_t ROWS>                                                  \
inline SignalBlockArrayInt<ROWS>(opName)(const SignalBlockArray<ROWS>& vx1) \
{                                                                       \
SignalBlockArrayInt<ROWS> vy;                                           \
const float* px1 = vx1.getConstBuffer();                              \
float* py1 = vy.getBuffer();                                          \
for (int n = 0; n < kSIMDVectorsPerDSPVector * ROWS; ++n)             \
{                                                                     \
float4 x = loadFloat4(px1);                                   \
auto y = (opComputation); \
storeFloat4((py1), y);                                   \
px1 += 4;                                        \
py1 += 4;                                          \
}                                                                     \
return vy;                                                            \
}

DEFINE_OP1_F2I(roundFloatToInt, (castIntToFloat(floatToIntRound(x))));
DEFINE_OP1_F2I(truncateFloatToInt, (castIntToFloat(floatToIntTruncate(x))));

// ----------------------------------------------------------------
// vector operators (int) -> float

#define DEFINE_OP1_I2F(opName, opComputation)                             \
template <size_t ROWS>                                                  \
inline SignalBlockArray<ROWS>(opName)(const SignalBlockArrayInt<ROWS>& vx1) \
{                                                                       \
SignalBlockArray<ROWS> vy;                                              \
const float* px1 = vx1.getConstBuffer();                              \
float* py1 = vy.getBuffer();                                          \
for (int n = 0; n < kSIMDVectorsPerDSPVector * ROWS; ++n)             \
{                                                                     \
int4 x = castFloatToInt(loadFloat4(px1));                             \
auto y = (opComputation); \
storeFloat4((py1), y);                                   \
px1 += 4;                                          \
py1 += 4;                                        \
}                                                                     \
return vy;                                                            \
}

DEFINE_OP1_I2F(intToFloat, (intToFloat(x)));
DEFINE_OP1_I2F(unsignedIntToFloat, (unsignedIntToFloat(x)));

// ----------------------------------------------------------------
// using the conversions above, define fractionalPart

DEFINE_OP1(fractionalPart, (x - intToFloat(floatToIntTruncate(x))));

// ----------------------------------------------------------------
// binary float vector, float vector -> int vector operators

#define DEFINE_OP2_FF2I(opName, opComputation)                            \
template <size_t ROWS>                                                  \
inline SignalBlockArrayInt<ROWS>(opName)(const SignalBlockArray<ROWS>& vx1, \
const SignalBlockArray<ROWS>& vx2) \
{                                                                       \
SignalBlockArrayInt<ROWS> vy;                                           \
const float* px1 = vx1.getConstBuffer();                              \
const float* px2 = vx2.getConstBuffer();                              \
float* py1 = vy.getBuffer();                                          \
for (int n = 0; n < kSIMDVectorsPerDSPVector * ROWS; ++n)             \
{                                                                     \
float4 x1 = loadFloat4(px1);                                  \
float4 x2 = loadFloat4(px2);                                  \
auto y = (opComputation); \
storeFloat4((py1), y);                                   \
px1 += 4;                                        \
px2 += 4;                                        \
py1 += 4;                                          \
}                                                                     \
return vy;                                                            \
}

DEFINE_OP2_FF2I(equal, (compareEqual(x1, x2)));
DEFINE_OP2_FF2I(notEqual, (compareNotEqual(x1, x2)));
DEFINE_OP2_FF2I(greaterThan, (compareGreaterThan(x1, x2)));
DEFINE_OP2_FF2I(greaterThanOrEqual, (compareGreaterThanOrEqual(x1, x2)));
DEFINE_OP2_FF2I(lessThan, (compareLessThan(x1, x2)));
DEFINE_OP2_FF2I(lessThanOrEqual, (compareLessThanOrEqual(x1, x2)));

// ----------------------------------------------------------------
// ternary operators float vector, float vector, int vector -> float vector

#define DEFINE_OP3_FFI2F(opName, opComputation)                           \
template <size_t ROWS>                                                  \
inline SignalBlockArray<ROWS>(opName)(const SignalBlockArray<ROWS>& vx1,    \
const SignalBlockArray<ROWS>& vx2,    \
const SignalBlockArrayInt<ROWS>& vx3) \
{                                                                       \
SignalBlockArray<ROWS> vy;                                              \
const float* px1 = vx1.getConstBuffer();                              \
const float* px2 = vx2.getConstBuffer();                              \
const float* px3 = vx3.getConstBuffer();                              \
float* py1 = vy.getBuffer();                                          \
for (int n = 0; n < kSIMDVectorsPerDSPVector * ROWS; ++n)             \
{                                                                     \
float4 x1 = loadFloat4(px1);                                  \
float4 x2 = loadFloat4(px2);                                  \
int4 x3 = castFloatToInt(loadFloat4(px3));                            \
auto y = (opComputation); \
storeFloat4((py1), y);                                   \
px1 += 4;                                        \
px2 += 4;                                        \
px3 += 4;                                        \
py1 += 4;                                        \
}                                                                     \
return vy;                                                            \
}

DEFINE_OP3_FFI2F(select, (vecSelectFFI(x1, x2, x3)));  // bitwise select(resultIfTrue,
// resultIfFalse, conditionMask)

// ----------------------------------------------------------------
// ternary operators int vector, int vector, int vector -> int vector

#define DEFINE_OP3_III2I(opName, opComputation)                              \
template <size_t ROWS>                                                     \
inline SignalBlockArrayInt<ROWS>(opName)(const SignalBlockArrayInt<ROWS>& vx1, \
const SignalBlockArrayInt<ROWS>& vx2, \
const SignalBlockArrayInt<ROWS>& vx3) \
{                                                                          \
SignalBlockArrayInt<ROWS> vy;                                              \
const float* px1 = vx1.getConstBuffer();                                 \
const float* px2 = vx2.getConstBuffer();                                 \
const float* px3 = vx3.getConstBuffer();                                 \
float* py1 = vy.getBuffer();                                             \
for (int n = 0; n < kSIMDVectorsPerDSPVector * ROWS; ++n)                \
{                                                                        \
int4 x1 = castFloatToInt(loadFloat4(px1));                               \
int4 x2 = castFloatToInt(loadFloat4(px2));                               \
int4 x3 = castFloatToInt(loadFloat4(px3));                               \
auto y = castIntToFloat(opComputation); \
storeFloat4((py1), y);                                   \
px1 += 4;                                             \
px2 += 4;                                             \
px3 += 4;                                             \
py1 += 4;                                             \
}                                                                        \
return vy;                                                               \
}

DEFINE_OP3_III2I(select, (vecSelectIII(x1, x2, x3)));  // bitwise select(resultIfTrue,
// resultIfFalse, conditionMask)

// ----------------------------------------------------------------
// n-ary operators

// add (a, b, c, ...)

template <size_t ROWS>
SignalBlockArray<ROWS> add(SignalBlockArray<ROWS> a)
{
  return a;
}

template <size_t ROWS, typename... Args>
SignalBlockArray<ROWS> add(SignalBlockArray<ROWS> first, Args... args)
{
  // the + here is the operator defined using vecAdd() above
  return first + add(args...);
}

// ----------------------------------------------------------------
// constexpr definitions

#define ConstSignalBlock constexpr SignalBlock
#define ConstSignalBlockArray constexpr SignalBlockArray
#define ConstSignalBlockInt constexpr SignalBlockInt
#define ConstSignalBlockArrayInt constexpr SignalBlockArrayInt

// ----------------------------------------------------------------
// single-vector index and sequence generators

constexpr float intToFloatCastFn(int i) { return (float)i; }
constexpr int indexFn(int i) { return i; }

inline ConstSignalBlock columnIndex() { return (make_array<kFramesPerBlock>(intToFloatCastFn)); }
inline ConstSignalBlockInt columnIndexInt() { return (make_array<kIntsPerDSPVector>(indexFn)); }

// return a linear sequence from start to end, where end will fall on the first
// index of the next vector.
inline SignalBlock rangeOpen(float start, float end)
{
  float interval = (end - start) / (kFramesPerBlock);
  return columnIndex() * SignalBlock(interval) + SignalBlock(start);
}

// return a linear sequence from start to end, where end falls on the last index
// of this vector.
inline SignalBlock rangeClosed(float start, float end)
{
  float interval = (end - start) / (kFramesPerBlock - 1.f);
  return columnIndex() * SignalBlock(interval) + SignalBlock(start);
}

// return a linear sequence from start to end, where start falls one sample
// "before" this vector and end falls on the last index of this vector.
inline SignalBlock interpolateDSPVectorLinear(float start, float end)
{
  float interval = (end - start) / (kFramesPerBlock);
  return columnIndex() * SignalBlock(interval) + SignalBlock(start + interval);
}

// ----------------------------------------------------------------
// single-vector horizontal operators returning float

inline float sum(const SignalBlock& x)
{
  const float* px1 = x.getConstBuffer();
  float sum = 0;
  for (int n = 0; n < kSIMDVectorsPerDSPVector; ++n)
  {
    sum += vecSumH(loadFloat4(px1));
    px1 += 4;
  }
  return sum;
}

inline float mean(const SignalBlock& x)
{
  constexpr float kGain = 1.0f / kFramesPerBlock;
  return sum(x) * kGain;
}

inline float max(const SignalBlock& x)
{
  const float* px1 = x.getConstBuffer();
  float fmax = FLT_MIN;
  for (int n = 0; n < kSIMDVectorsPerDSPVector; ++n)
  {
    fmax = ml::max(fmax, vecMaxH(loadFloat4(px1)));
    px1 += 4;
  }
  return fmax;
}

inline float min(const SignalBlock& x)
{
  const float* px1 = x.getConstBuffer();
  float fmin = FLT_MAX;
  for (int n = 0; n < kSIMDVectorsPerDSPVector; ++n)
  {
    fmin = ml::min(fmin, vecMinH(loadFloat4(px1)));
    px1 += 4;
  }
  return fmin;
}

// ----------------------------------------------------------------
// normalize

template <size_t ROWS>
inline SignalBlockArray<ROWS> normalize(const SignalBlockArray<ROWS>& x1)
{
  SignalBlockArray<ROWS> vy;
  for (int j = 0; j < ROWS; ++j)
  {
    auto inputRow = x1.getRowVectorUnchecked(j);
    vy.setRowVectorUnchecked(j, inputRow / sum(inputRow));
  }
  return vy;
}

// ----------------------------------------------------------------
// row-wise operations and conversions

// for the given output ROWS and given an input SignalBlockArray with N rows,
// repeat all the input rows enough times to fill the output SignalBlockArray.
template <size_t ROWS, size_t N>
inline SignalBlockArray<ROWS * N> repeatRows(const SignalBlockArray<N>& x1)
{
  SignalBlockArray<ROWS * N> vy;
  for (int j = 0, k = 0; j < ROWS * N; ++j)
  {
    vy.setRowVectorUnchecked(j, x1.getRowVectorUnchecked(k));
    if (++k >= N) k = 0;
  }
  return vy;
}

// for the given ROWS and given an input SignalBlockArray x with N rows,
// stretch x by repeating rows as necessary to make an output SignalBlockArray
// with ROWS rows.
template <size_t ROWS, size_t N>
inline SignalBlockArray<ROWS> stretchRows(const SignalBlockArray<N>& x)
{
  SignalBlockArray<ROWS> vy;
  for (int j = 0; j < ROWS; ++j)
  {
    int k = roundf((j * (N - 1.f)) / (ROWS - 1.f));
    vy.setRowVectorUnchecked(j, x.getRowVectorUnchecked(k));
  }
  return vy;
}

// for the given ROWS and given an input SignalBlockArray x with N rows,
// fill an output array by copying rows of the input, then adding rows of zeros as
// necessary.
template <size_t ROWS, size_t N>
inline SignalBlockArray<ROWS> zeroPadRows(const SignalBlockArray<N>& x)
{
  // default constructor currently zero-fills
  SignalBlockArray<ROWS> vy;
  constexpr size_t rowsToCopy = min(ROWS, N);
  for (int j = 0; j < rowsToCopy; ++j)
  {
    vy.setRowVectorUnchecked(j, x.getRowVectorUnchecked(j));
  }
  return vy;
}

// Shift the array down by the number of rows given in rowsToShift.
// Any rows shifted in from outside the range [0, ROWS) are zeroed. Negative
// shifts are OK.
template <size_t ROWS>
inline SignalBlockArray<ROWS> shiftRows(const SignalBlockArray<ROWS>& x, int rowsToShift)
{
  SignalBlockArray<ROWS> vy;
  int k = -rowsToShift;
  for (int j = 0; j < ROWS; ++j)
  {
    if (within(k, 0, static_cast<int>(ROWS)))
    {
      vy.setRowVectorUnchecked(j, x.getRowVectorUnchecked(k));
    }
    else
    {
      vy.setRowVectorUnchecked(j, 0.f);
    }
    ++k;
  }
  return vy;
}

// Rotate the array down by the number of rows given in rowsToRotate.
// Any rows rotated in from outside the range [0, ROWS) are wrapped. Negative
// rotations are OK.
template <size_t ROWS>
inline SignalBlockArray<ROWS> rotateRows(const SignalBlockArray<ROWS>& x, int rowsToRotate)
{
  SignalBlockArray<ROWS> vy;
  
  // get start index k to which row 0 is mapped
  int k = modulo(-rowsToRotate, ROWS);
  for (int j = 0; j < ROWS; ++j)
  {
    vy.setRowVectorUnchecked(j, x.getRowVectorUnchecked(k));
    if (++k >= ROWS) k = 0;
  }
  return vy;
}

// ----------------------------------------------------------------
// row-wise combining

// concatRows with two arguments: append one SignalBlockArray after another.
template <size_t ROWSA, size_t ROWSB>
inline SignalBlockArray<ROWSA + ROWSB> concatRows(const SignalBlockArray<ROWSA>& x1,
                                                const SignalBlockArray<ROWSB>& x2)
{
  SignalBlockArray<ROWSA + ROWSB> vy;
  for (int j = 0; j < ROWSA; ++j)
  {
    vy.setRowVectorUnchecked(j, x1.getRowVectorUnchecked(j));
  }
  for (int j = 0; j < ROWSB; ++j)
  {
    vy.setRowVectorUnchecked(j + ROWSA, x2.getRowVectorUnchecked(j));
  }
  return vy;
}

// concatRows with three arguments.
template <size_t ROWSA, size_t ROWSB, size_t ROWSC>
inline SignalBlockArray<ROWSA + ROWSB + ROWSC> concatRows(const SignalBlockArray<ROWSA>& x1,
                                                        const SignalBlockArray<ROWSB>& x2,
                                                        const SignalBlockArray<ROWSC>& x3)
{
  SignalBlockArray<ROWSA + ROWSB + ROWSC> vy;
  for (int j = 0; j < ROWSA; ++j)
  {
    vy.setRowVectorUnchecked(j, x1.getRowVectorUnchecked(j));
  }
  for (int j = 0; j < ROWSB; ++j)
  {
    vy.setRowVectorUnchecked(j + ROWSA, x2.getRowVectorUnchecked(j));
  }
  for (int j = 0; j < ROWSC; ++j)
  {
    vy.setRowVectorUnchecked(j + ROWSA + ROWSB, x3.getRowVectorUnchecked(j));
  }
  return vy;
}

// NOTE: of course all this repeated code looks bad, but
// i'm not sure there is a reasonable way to generalize this in C++11. The problem is determining
// the number of rows in the output type. It should be possible with return type deduction (auto) in
// C++14.
// TODO variadic templates can expand into a template argument list, so maybe this IS possible
// by wrapping the concatRows template in another template - give it a try.

// concatRows with four arguments.
template <size_t ROWSA, size_t ROWSB, size_t ROWSC, size_t ROWSD>
inline SignalBlockArray<ROWSA + ROWSB + ROWSC + ROWSD> concatRows(const SignalBlockArray<ROWSA>& x1,
                                                                const SignalBlockArray<ROWSB>& x2,
                                                                const SignalBlockArray<ROWSC>& x3,
                                                                const SignalBlockArray<ROWSD>& x4)
{
  SignalBlockArray<ROWSA + ROWSB + ROWSC + ROWSD> vy;
  for (int j = 0; j < ROWSA; ++j)
  {
    vy.setRowVectorUnchecked(j, x1.getRowVectorUnchecked(j));
  }
  for (int j = 0; j < ROWSB; ++j)
  {
    vy.setRowVectorUnchecked(j + ROWSA, x2.getRowVectorUnchecked(j));
  }
  for (int j = 0; j < ROWSC; ++j)
  {
    vy.setRowVectorUnchecked(j + ROWSA + ROWSB, x3.getRowVectorUnchecked(j));
  }
  for (int j = 0; j < ROWSD; ++j)
  {
    vy.setRowVectorUnchecked(j + ROWSA + ROWSB + ROWSC, x4.getRowVectorUnchecked(j));
  }
  return vy;
}
// Rotate the elements of each row of a SignalBlockArray by one element left.
// The first element of each row is moved to the end
template <size_t ROWS>
inline ml::SignalBlockArray<ROWS> rotateLeft(const ml::SignalBlockArray<ROWS>& x)
{
  ml::SignalBlockArray<ROWS> vy;
  
  for (size_t row = 0; row < ROWS; row++)
  {
    const float* px1 = x.getConstBuffer() + (row * kFramesPerBlock);
    const float* px2 = px1 + 4;
    float* py1 = vy.getBuffer() + (row * kFramesPerBlock);
    
    for (int n = 0; n < kSIMDVectorsPerDSPVector - 1; ++n)
    {
      auto y = vecShuffleLeft(loadFloat4(px1), loadFloat4(px2));
      storeFloat4(py1, y);
      
      px1 += 4;
      px2 += 4;
      py1 += 4;
    }
    
    px2 = x.getConstBuffer() + (row * kFramesPerBlock);
    
    auto y = vecShuffleLeft(loadFloat4(px1), loadFloat4(px2));
    storeFloat4(py1, y);
  }
  
  return vy;
}

// Rotate the elements of each row of a SignalBlockArray by one element right.
// The last element of each row is moved to the start
template <size_t ROWS>
inline ml::SignalBlockArray<ROWS> rotateRight(const ml::SignalBlockArray<ROWS>& x)
{
  ml::SignalBlockArray<ROWS> vy;
  
  for (size_t row = 0; row < ROWS; row++)
  {
    const float* px1 = x.getConstBuffer() + (row * kFramesPerBlock);
    const float* px2 = px1 + 4;
    float* py1 = vy.getBuffer() + (row * kFramesPerBlock) + 4;
    
    for (int n = 0; n < kSIMDVectorsPerDSPVector - 1; ++n)
    {
      auto y = vecShuffleRight(loadFloat4(px1), loadFloat4(px2));
      storeFloat4(py1, y);
      
      px1 += 4;
      px2 += 4;
      py1 += 4;
    }
    
    px2 = x.getConstBuffer() + (row * kFramesPerBlock);
    py1 = vy.getBuffer() + (row * kFramesPerBlock);
    
    auto y = vecShuffleRight(loadFloat4(px1), loadFloat4(px2));
    storeFloat4(py1, y);
  }
  
  return vy;
}
// shuffle two DSPVectorArrays, alternating x1 to even rows of result and x2 to
// odd rows. if the sources are different sizes, the excess rows are all
// appended to the destination after shuffling is done.
template <size_t ROWSA, size_t ROWSB>
inline SignalBlockArray<ROWSA + ROWSB> shuffleRows(const SignalBlockArray<ROWSA> x1,
                                                 const SignalBlockArray<ROWSB> x2)
{
  SignalBlockArray<ROWSA + ROWSB> vy;
  int ja = 0;
  int jb = 0;
  int jy = 0;
  while ((ja < ROWSA) || (jb < ROWSB))
  {
    if (ja < ROWSA)
    {
      vy.setRowVectorUnchecked(jy, x1.getRowVectorUnchecked(ja));
      ja++;
      jy++;
    }
    if (jb < ROWSB)
    {
      vy.setRowVectorUnchecked(jy, x2.getRowVectorUnchecked(jb));
      jb++;
      jy++;
    }
  }
  return vy;
}

// ----------------------------------------------------------------
// separating rows

template <size_t ROWS>
inline SignalBlockArray<(ROWS + 1) / 2> evenRows(const SignalBlockArray<ROWS>& x1)
{
  SignalBlockArray<(ROWS + 1) / 2> vy;
  for (int j = 0; j < (ROWS + 1) / 2; ++j)
  {
    vy.setRowVectorUnchecked(j, x1.getRowVectorUnchecked(j * 2));
  }
  return vy;
}

template <size_t ROWS>
inline SignalBlockArray<ROWS / 2> oddRows(const SignalBlockArray<ROWS>& x1)
{
  SignalBlockArray<ROWS / 2> vy;
  for (int j = 0; j < ROWS / 2; ++j)
  {
    vy.setRowVectorUnchecked(j, x1.getRowVectorUnchecked(j * 2 + 1));
  }
  return vy;
}

// return the SignalBlockArray consisting of rows [A-B) of the input.
template <size_t A, size_t B, size_t ROWS>
inline SignalBlockArray<B - A> separateRows(const SignalBlockArray<ROWS>& x)
{
  static_assert(B <= ROWS, "separateRows: range out of bounds!");
  static_assert(A < ROWS, "separateRows: range out of bounds!");
  SignalBlockArray<B - A> vy;
  for (int j = A; j < B; ++j)
  {
    vy.setRowVectorUnchecked(j - A, x.getRowVectorUnchecked(j));
  }
  return vy;
}

// ----------------------------------------------------------------
// add rows to get row-wise sum

template <size_t ROWS>
inline SignalBlock addRows(const SignalBlockArray<ROWS>& x)
{
  SignalBlock vy{0.f};
  
  for (int j = 0; j < ROWS; ++j)
  {
    vy = add(vy, x.getRowVectorUnchecked(j));
  }
  return vy;
}

// ----------------------------------------------------------------
// rowIndex - returns a SignalBlock of j rows, each row filled
// with the index of its row

template <size_t ROWS>
inline SignalBlockArray<ROWS> rowIndex()
{
  SignalBlockArray<ROWS> y;
  for (int j = 0; j < ROWS; ++j)
  {
    y.setRowVectorUnchecked(j, SignalBlock(j));
  }
  return y;
}

// ----------------------------------------------------------------
// columnIndex<n> - shorthand for repeatRows<n>(columnIndex())

template <size_t ROWS>
inline SignalBlockArray<ROWS> columnIndex()
{
  return repeatRows<ROWS>(SignalBlock(make_array<kFramesPerBlock>(intToFloatCastFn)));
}

// TODO variadic splitRows(bundleSIg, outputRow1, outputRow2, ... )

// ----------------------------------------------------------------
// for testing

template <size_t ROWS>
inline std::ostream& operator<<(std::ostream& out, const SignalBlockArray<ROWS>& vecArray)
{
  //    if(ROWS > 1) out << "[   ";
  for (int v = 0; v < ROWS; ++v)
  {
    //  if(ROWS > 1) if(v > 0) out << "\n    ";
    if (ROWS > 1) out << "\n    v" << v << ": ";
    out << "[";
    for (int i = 0; i < kFramesPerBlock; ++i)
    {
      out << vecArray[v * kFramesPerBlock + i] << " ";
    }
    out << "] ";
  }
  //    if(ROWS > 1) out << "]";
  return out;
}

template <size_t ROWS>
inline std::ostream& operator<<(std::ostream& out, const SignalBlockArrayInt<ROWS>& vecArray)
{
  out << "@" << std::hex << reinterpret_cast<unsigned long>(&vecArray) << std::dec << "\n ";
  //    if(ROWS > 1) out << "[   ";
  for (int v = 0; v < ROWS; ++v)
  {
    if (ROWS > 1)
      if (v > 0) out << "\n    ";
    if (ROWS > 1) out << "v" << v << ": ";
    out << "[";
    for (int i = 0; i < kIntsPerDSPVector; ++i)
    {
      out << vecArray[v * kIntsPerDSPVector + i] << " ";
    }
    out << "] ";
  }
  //    if(ROWS > 1) out << "]";
  return out;
}

inline bool validate(const SignalBlock& x)
{
  bool r = true;
  for (int n = 0; n < kFramesPerBlock; ++n)
  {
    const float maxUsefulValue = 1e8;
    if (ml::isNaN(x[n]) || (fabs(x[n]) > maxUsefulValue))
    {
      std::cout << "error: " << x[n] << " at index " << n << "\n";
      std::cout << x << "\n";
      r = false;
      break;
    }
  }
  return r;
}
}  // namespace ml
