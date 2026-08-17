// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2022 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#include <vector>

#include "catch.hpp"
#include "madronalib.h"

using namespace ml;

namespace
{
// A value no process() run would ever produce, so "left unwritten" is
// distinguishable from "written as silence".
constexpr float kPoison = -12345.f;

// passthrough, so output content can be checked against input
void passThrough(AudioContext* c)
{
  c->outputs[0] = c->inputs[0];
  c->outputs[1] = c->inputs[1];
}

// Run one host callback of nFrames and return how many output samples the
// context left untouched.
int unwrittenSamples(AudioContext& ctx, int nFrames)
{
  std::vector<float> inL(nFrames, 0.25f), inR(nFrames, 0.25f);
  std::vector<float> outL(nFrames, kPoison), outR(nFrames, kPoison);

  const float* ins[2] = {inL.data(), inR.data()};
  float* outs[2] = {outL.data(), outR.data()};

  ctx.process(ins, outs, nFrames, passThrough);

  int n = 0;
  for (int i = 0; i < nFrames; ++i)
  {
    if (outL[i] == kPoison) n++;
    if (outR[i] == kPoison) n++;
  }
  return n;
}
}  // namespace

// process() refuses any block larger than the buffers it was given, returning
// without writing anything at all. A host that asks for more than the default
// therefore gets its output buffer back untouched -- not silence, but whatever
// was in it. clap-validator's process-varying-block-sizes catches this as
// "the sample at output port 0, channel 0, and sample index 0 was left
// unwritten" when the block reaches 16384.
TEST_CASE("madronalib/core/audiocontext/default_block_sizes", "[audiocontext]")
{
  for (int n : {1, 17, 64, 100, 512, 1024, 4095, (int)kMaxIOFramesDefault})
  {
    AudioContext ctx(2, 2);
    ctx.setSampleRate(48000);
    REQUIRE(unwrittenSamples(ctx, n) == 0);
  }
}

// resizeBuffers is the only way to raise that limit, so it has to actually
// raise it -- it used to resize the ring buffers and leave maxFrames_ alone,
// which made it look like it worked while process() kept dropping blocks.
TEST_CASE("madronalib/core/audiocontext/large_block_sizes", "[audiocontext]")
{
  for (int n : {(int)kMaxIOFramesDefault + 1, 8192, 16384})
  {
    AudioContext ctx(2, 2);
    ctx.setSampleRate(48000);
    ctx.resizeBuffers(2, 2, n + kFramesPerBlock);
    ctx.clear();

    // twice, so a ring that only survives its first pass is caught
    REQUIRE(unwrittenSamples(ctx, n) == 0);
    REQUIRE(unwrittenSamples(ctx, n) == 0);
  }
}

// clear() primes the output with one block of zeros, giving a constant
// one-block delay. If the ring fits the host's block exactly, writing a
// full-size block clobbers that primed block and the delay silently becomes
// zero -- so the plugin's latency would depend on the host's buffer size.
TEST_CASE("madronalib/core/audiocontext/delay_is_block_size_independent",
          "[audiocontext]")
{
  auto measurePrimedZeros = [](int nFrames)
  {
    AudioContext ctx(2, 2);
    ctx.setSampleRate(48000);
    ctx.resizeBuffers(2, 2, nFrames + kFramesPerBlock);
    ctx.clear();

    // ramp starting at 1, so the primed zeros are unambiguous
    std::vector<float> inL(nFrames), inR(nFrames);
    for (int i = 0; i < nFrames; ++i) inL[i] = inR[i] = (float)(i + 1);

    std::vector<float> outL(nFrames, kPoison), outR(nFrames, kPoison);
    const float* ins[2] = {inL.data(), inR.data()};
    float* outs[2] = {outL.data(), outR.data()};
    ctx.process(ins, outs, nFrames, passThrough);

    int zeros = 0;
    while (zeros < nFrames && outL[zeros] == 0.f) zeros++;
    return zeros;
  };

  REQUIRE(measurePrimedZeros(512) == (int)kFramesPerBlock);
  REQUIRE(measurePrimedZeros((int)kMaxIOFramesDefault) == (int)kFramesPerBlock);
  REQUIRE(measurePrimedZeros(16384) == (int)kFramesPerBlock);
}
