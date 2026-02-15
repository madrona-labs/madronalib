// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#include "catch.hpp"
#include "MLDSPResampling.h"
#include "MLTestUtils.h"

// a unit test made using the Catch framework in catch.hpp / tests.cpp.
using namespace ml;



namespace {

// Helper: check if two SignalBlocks are nearly equal
bool nearlyEqual(const SignalBlock& a, const SignalBlock& b, float eps = 1e-4f) {
  for (int i = 0; i < kFramesPerBlock; ++i) {
    if (std::fabs(a[i] - b[i]) >= eps) return false;
  }
  return true;
}

// Helper: RMS of a SignalBlock
float rms(const SignalBlock& v) {
  float sum = 0.f;
  for (int i = 0; i < kFramesPerBlock; ++i) {
    sum += v[i] * v[i];
  }
  return sqrtf(sum / kFramesPerBlock);
}

// Helper: generate a sine block at a given normalized frequency
SignalBlock makeSine(float omega, float& phase) {
  SignalBlock v;
  for (int i = 0; i < kFramesPerBlock; ++i) {
    v[i] = sinf(phase);
    phase += omega * kTwoPi;
    if (phase > kTwoPi) phase -= kTwoPi;
  }
  return v;
}

}

// ================================================================
// HalfBandFilter tests
// ================================================================

TEST_CASE("madronalib/resampling/halfband_types", "[resampling]")
{
  SECTION("HalfBlock is half the size of Block")
  {
    REQUIRE(sizeof(HalfBlock<float>) == sizeof(Block<float>) / 2);
    REQUIRE(sizeof(HalfBlock<float4>) == sizeof(Block<float4>) / 2);
  }
}

TEST_CASE("madronalib/resampling/halfband_clear", "[resampling]")
{
  HalfBandFilter<float> hbf;
  
  // process some signal to dirty the state
  HalfBlock<float> halfIn;
  for (int i = 0; i < kFramesPerBlock / 2; ++i) halfIn[i] = 1.0f;
  hbf.upsample(halfIn);
  
  // clear and verify silence in, silence out
  hbf.clear();
  for (int i = 0; i < kFramesPerBlock / 2; ++i) halfIn[i] = 0.0f;
  Block<float> out = hbf.upsample(halfIn);
  REQUIRE(rms(out) == 0.0f);
}

TEST_CASE("madronalib/resampling/halfband_dc", "[resampling]")
{
  SECTION("upsample preserves DC")
  {
    HalfBandFilter<float> hbf;
    HalfBlock<float> halfDC;
    for (int i = 0; i < kFramesPerBlock / 2; ++i) halfDC[i] = 1.0f;
    
    // run several blocks to let the filter settle
    Block<float> out;
    for (int i = 0; i < 20; ++i) {
      out = hbf.upsample(halfDC);
    }
    
    // after settling, output should be near 1.0
    for (int i = 0; i < kFramesPerBlock; ++i) {
      REQUIRE(std::fabs(out[i] - 1.0f) < 0.01f);
    }
  }
  
  SECTION("downsample preserves DC")
  {
    HalfBandFilter<float> hbf;
    Block<float> blockDC;
    for (int i = 0; i < kFramesPerBlock; ++i) blockDC[i] = 1.0f;
    
    HalfBlock<float> out;
    for (int i = 0; i < 20; ++i) {
      out = hbf.downsample(blockDC);
    }
    
    for (int i = 0; i < kFramesPerBlock / 2; ++i) {
      REQUIRE(std::fabs(out[i] - 1.0f) < 0.01f);
    }
  }
}

// ================================================================
// Upsampler2x / Downsampler2x tests
// ================================================================

TEST_CASE("madronalib/resampling/roundtrip_dc", "[resampling]")
{
  Upsampler2x<float> up;
  Downsampler2x<float> down;
  
  SignalBlock dc(1.0f);
  
  // run enough blocks to settle the filters
  SignalBlock result;
  for (int i = 0; i < 30; ++i) {
    auto [first, second] = up(dc);
    result = down(first, second);
  }
  
  REQUIRE(nearlyEqual(result, dc, 0.01f));
}

TEST_CASE("madronalib/resampling/roundtrip_low_freq", "[resampling]")
{
  Upsampler2x<float> up;
  Downsampler2x<float> down;
  
  // low frequency sine: omega = 0.01 (well within passband)
  float phaseIn = 0.f;
  float phaseRef = 0.f;
  
  // run blocks to settle
  for (int i = 0; i < 30; ++i) {
    SignalBlock input = makeSine(0.01f, phaseIn);
    auto [first, second] = up(input);
    down(first, second);
  }
  
  // now compare a settled block
  SignalBlock input = makeSine(0.01f, phaseIn);
  auto [first, second] = up(input);
  SignalBlock result = down(first, second);
  
  // the roundtrip signal should have similar RMS (passband, ~unity gain)
  float inRMS = rms(input);
  float outRMS = rms(result);
  float gainError = std::fabs(outRMS / inRMS - 1.0f);
  REQUIRE(gainError < 0.05f);
}

TEST_CASE("madronalib/resampling/downsample_rejection", "[resampling]")
{
  Downsampler2x<float> down;
  
  // sine at omega=0.49 of the 2x rate — above the 1x Nyquist
  // this is content the downsampler SHOULD reject
  float phase = 0.f;
  
  HalfBlock<float> result;
  for (int i = 0; i < 30; ++i) {
    SignalBlock input = makeSine(0.49f, phase);
    result = down.filter.downsample(input);
  }
  
  float sum = 0.f;
  for (int i = 0; i < kFramesPerBlock / 2; ++i) sum += result[i] * result[i];
  float resultRMS = sqrtf(sum / (kFramesPerBlock / 2));
  REQUIRE(resultRMS < 0.01f);
}

// ================================================================
// Upsampler2x / Downsampler2x with float4
// ================================================================

TEST_CASE("madronalib/resampling/float4_roundtrip_dc", "[resampling]")
{
  Upsampler2x<float4> up;
  Downsampler2x<float4> down;
  
  Block<float4> dc;
  for (int i = 0; i < kFramesPerBlock; ++i) dc[i] = float4(1.0f);
  
  Block<float4> result;
  for (int i = 0; i < 30; ++i) {
    auto [first, second] = up(dc);
    result = down(first, second);
  }
  
  // check all 4 lanes settled to 1.0
  for (int lane = 0; lane < 4; ++lane) {
    float v = getFloat4Lane(result[kFramesPerBlock / 2], lane);
    REQUIRE(std::fabs(v - 1.0f) < 0.01f);
  }
}

// ================================================================
// Multi-octave Downsampler tests
// ================================================================

TEST_CASE("madronalib/resampling/downsampler_1oct_dc", "[resampling]")
{
  Downsampler down(1);
  
  SignalBlock dc(1.0f);
  
  // 1 octave = every 2 writes produces output
  SignalBlock result;
  for (int i = 0; i < 40; ++i) {
    if (down.write(dc)) {
      result = down.read();
    }
  }
  
  REQUIRE(nearlyEqual(result, dc, 0.01f));
}

TEST_CASE("madronalib/resampling/downsampler_2oct_dc", "[resampling]")
{
  Downsampler down(2);
  
  SignalBlock dc(1.0f);
  
  SignalBlock result;
  for (int i = 0; i < 80; ++i) {
    if (down.write(dc)) {
      result = down.read();
    }
  }
  
  REQUIRE(nearlyEqual(result, dc, 0.01f));
}

TEST_CASE("madronalib/resampling/downsampler_write_cadence", "[resampling]")
{
  SECTION("1 octave: output every 2 writes")
  {
    Downsampler down(1);
    SignalBlock dc(0.f);
    int outputs = 0;
    for (int i = 0; i < 16; ++i) {
      if (down.write(dc)) outputs++;
    }
    REQUIRE(outputs == 8);
  }
  
  SECTION("2 octaves: output every 4 writes")
  {
    Downsampler down(2);
    SignalBlock dc(0.f);
    int outputs = 0;
    for (int i = 0; i < 16; ++i) {
      if (down.write(dc)) outputs++;
    }
    REQUIRE(outputs == 4);
  }
  
  SECTION("3 octaves: output every 8 writes")
  {
    Downsampler down(3);
    SignalBlock dc(0.f);
    int outputs = 0;
    for (int i = 0; i < 16; ++i) {
      if (down.write(dc)) outputs++;
    }
    REQUIRE(outputs == 2);
  }
}

// ================================================================
// Multi-octave Upsampler tests
// ================================================================

TEST_CASE("madronalib/resampling/upsampler_1oct_dc", "[resampling]")
{
  Upsampler up(1);
  
  SignalBlock dc(1.0f);
  
  // settle
  for (int i = 0; i < 20; ++i) {
    up.write(dc);
    up.read();
    up.read();
  }
  
  // check output
  up.write(dc);
  SignalBlock r1 = up.read();
  SignalBlock r2 = up.read();
  
  REQUIRE(nearlyEqual(r1, dc, 0.01f));
  REQUIRE(nearlyEqual(r2, dc, 0.01f));
}

TEST_CASE("madronalib/resampling/upsampler_read_count", "[resampling]")
{
  SECTION("1 octave: 2 reads per write")
  {
    Upsampler up(1);
    SignalBlock dc(0.f);
    up.write(dc);
    SignalBlock r1 = up.read();
    SignalBlock r2 = up.read();
    (void)r1; (void)r2; // just verify no crash
  }
  
  SECTION("2 octaves: 4 reads per write")
  {
    Upsampler up(2);
    SignalBlock dc(0.f);
    up.write(dc);
    for (int i = 0; i < 4; ++i) {
      SignalBlock r = up.read();
      (void)r;
    }
  }
}

// ================================================================
// Roundtrip: multi-octave down then up
// ================================================================

TEST_CASE("madronalib/resampling/multioct_roundtrip_dc", "[resampling]")
{
  Downsampler down(1);
  Upsampler up(1);
  
  SignalBlock dc(1.0f);
  
  // run enough to settle
  for (int i = 0; i < 40; ++i) {
    if (down.write(dc)) {
      SignalBlock halfRate = down.read();
      up.write(halfRate);
    }
  }
  
  // one more cycle
  down.write(dc);
  if (down.write(dc)) {
    SignalBlock halfRate = down.read();
    up.write(halfRate);
    SignalBlock r1 = up.read();
    SignalBlock r2 = up.read();
    REQUIRE(nearlyEqual(r1, dc, 0.02f));
    REQUIRE(nearlyEqual(r2, dc, 0.02f));
  }
}
