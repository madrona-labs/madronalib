// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// a unit test made using the Catch framework in catch.hpp / tests.cpp.

#include "catch.hpp"
#include "MLTestUtils.h"
#include "MLDSPSample.h"
#include "MLDSPGens.h"
#include "MLDSPFilters.h"
#include "MLDSPMath.h"

#include "FFTRealFixLen.h"

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
// FFT-based filter tests
// ================================================================

namespace {

constexpr int kFFTOrder = 6; // 2^6 = 64 = kFramesPerBlock
constexpr int N = kFramesPerBlock;

template<typename FilterT>
std::array<float, N / 2> getMagnitudes(FilterT& filter)
{
  SignalBlock impulse{0.f};
  impulse[0] = 1.0f;
  
  SignalBlock output = filter(impulse);
  
  ffft::FFTRealFixLen<kFFTOrder> fft;
  std::array<float, N> fftOut{};
  fft.do_fft(fftOut.data(), output.data());
  
  std::array<float, N / 2> mag{};
  for (int i = 0; i < N / 2; ++i)
  {
    float re = fftOut[i];
    float im = (i == 0) ? 0.f : fftOut[i + N / 2];
    mag[i] = sqrtf(re * re + im * im);
  }
  return mag;
}

float magToDB(float m) { return 20.f * log10f(m + 1e-12f); }

} // anonymous namespace

// ================================================================
// Lopass
// ================================================================

TEST_CASE("madronalib/filters/lopass", "[filters]")
{
  SECTION("DC passes through")
  {
    Lopass<float> lp;
    lp.coeffs = Lopass<float>::makeCoeffs({0.1f, 0.5f});
    auto mag = getMagnitudes(lp);
    REQUIRE(mag[0] > 0.9f);
  }
  
  SECTION("high frequencies are attenuated")
  {
    Lopass<float> lp;
    lp.coeffs = Lopass<float>::makeCoeffs({0.05f, 0.5f});
    auto mag = getMagnitudes(lp);
    for (int i = N / 4; i < N / 2; ++i)
      REQUIRE(mag[i] < mag[0] * 0.1f);
  }
  
  SECTION("lower omega means narrower passband")
  {
    Lopass<float> lpWide, lpNarrow;
    lpWide.coeffs = Lopass<float>::makeCoeffs({0.2f, 0.5f});
    lpNarrow.coeffs = Lopass<float>::makeCoeffs({0.05f, 0.5f});
    REQUIRE(getMagnitudes(lpWide)[N / 8] > getMagnitudes(lpNarrow)[N / 8]);
  }
  
  SECTION("resonance peaks near cutoff")
  {
    Lopass<float> lp;
    lp.coeffs = Lopass<float>::makeCoeffs({0.1f, 0.1f});
    auto mag = getMagnitudes(lp);
    float peak = *std::max_element(mag.begin() + 1, mag.end());
    REQUIRE(peak > mag[0]);
  }
}

// ================================================================
// Hipass
// ================================================================

TEST_CASE("madronalib/filters/hipass", "[filters]")
{
  SECTION("DC is rejected")
  {
    Hipass<float> hp;
    hp.coeffs = Hipass<float>::makeCoeffs({0.1f, 0.5f});
    auto mag = getMagnitudes(hp);
    REQUIRE(mag[0] < 0.01f);
  }
  
  SECTION("high frequencies pass through")
  {
    Hipass<float> hp;
    hp.coeffs = Hipass<float>::makeCoeffs({0.05f, 0.5f});
    auto mag = getMagnitudes(hp);
    // upper bins should have significant energy
    float upperAvg = 0.f;
    for (int i = N / 4; i < N / 2; ++i) upperAvg += mag[i];
    upperAvg /= (N / 4);
    REQUIRE(upperAvg > 0.5f);
  }
  
  SECTION("higher omega means narrower passband")
  {
    Hipass<float> hpWide, hpNarrow;
    hpWide.coeffs = Hipass<float>::makeCoeffs({0.05f, 0.5f});
    hpNarrow.coeffs = Hipass<float>::makeCoeffs({0.2f, 0.5f});
    // at a mid bin, the wider (lower cutoff) filter passes more
    REQUIRE(getMagnitudes(hpWide)[N / 8] > getMagnitudes(hpNarrow)[N / 8]);
  }
  
  SECTION("resonance peaks near cutoff")
  {
    Hipass<float> hp;
    hp.coeffs = Hipass<float>::makeCoeffs({0.1f, 0.1f});
    auto mag = getMagnitudes(hp);
    float peak = *std::max_element(mag.begin() + 1, mag.end());
    // peak should exceed the high-frequency asymptote
    REQUIRE(peak > mag[N / 2 - 1]);
  }
}

// ================================================================
// Bandpass
// ================================================================

TEST_CASE("madronalib/filters/bandpass", "[filters]")
{
  SECTION("DC is rejected")
  {
    Bandpass<float> bp;
    bp.coeffs = Bandpass<float>::makeCoeffs({0.1f, 0.5f});
    auto mag = getMagnitudes(bp);
    REQUIRE(mag[0] < 0.01f);
  }
  
  SECTION("has a peak near cutoff frequency")
  {
    Bandpass<float> bp;
    bp.coeffs = Bandpass<float>::makeCoeffs({0.1f, 0.5f});
    auto mag = getMagnitudes(bp);
    // bin for omega=0.1: bin = omega * N = 6.4, so peak near bin 6
    long peakBin = std::max_element(mag.begin(), mag.end()) - mag.begin();
    REQUIRE(peakBin >= 4);
    REQUIRE(peakBin <= 9);
  }
  
  SECTION("narrower Q gives sharper peak")
  {
    Bandpass<float> bpWide, bpNarrow;
    bpWide.coeffs = Bandpass<float>::makeCoeffs({0.1f, 1.0f});   // wide (k=1)
    bpNarrow.coeffs = Bandpass<float>::makeCoeffs({0.1f, 0.1f}); // narrow (k=0.1)
    auto magWide = getMagnitudes(bpWide);
    auto magNarrow = getMagnitudes(bpNarrow);
    // narrow filter should have higher peak
    float peakWide = *std::max_element(magWide.begin(), magWide.end());
    float peakNarrow = *std::max_element(magNarrow.begin(), magNarrow.end());
    REQUIRE(peakNarrow > peakWide);
  }
  
  SECTION("both DC and Nyquist are attenuated")
  {
    Bandpass<float> bp;
    bp.coeffs = Bandpass<float>::makeCoeffs({0.1f, 0.5f});
    auto mag = getMagnitudes(bp);
    float peak = *std::max_element(mag.begin(), mag.end());
    REQUIRE(mag[0] < peak * 0.1f);
    REQUIRE(mag[N / 2 - 1] < peak * 0.1f);
  }
}

// ================================================================
// LoShelf
// ================================================================

TEST_CASE("madronalib/filters/loshelf", "[filters]")
{
  SECTION("boost increases low frequency energy")
  {
    LoShelf<float> ls;
    ls.coeffs = LoShelf<float>::makeCoeffs({0.1f, 0.7f, 2.0f}); // A=2: boost
    auto mag = getMagnitudes(ls);
    // DC should be boosted relative to high freqs
    REQUIRE(mag[0] > mag[N / 2 - 1] * 1.5f);
  }
  
  SECTION("cut reduces low frequency energy")
  {
    LoShelf<float> ls;
    ls.coeffs = LoShelf<float>::makeCoeffs({0.1f, 0.7f, 0.5f}); // A=0.5: cut
    auto mag = getMagnitudes(ls);
    // DC should be lower than high freqs
    REQUIRE(mag[0] < mag[N / 2 - 1]);
  }
  
  SECTION("A=1 is unity")
  {
    LoShelf<float> ls;
    ls.coeffs = LoShelf<float>::makeCoeffs({0.1f, 0.7f, 1.0f});
    auto mag = getMagnitudes(ls);
    REQUIRE(std::fabs(mag[0] - mag[N / 4]) < 0.05f);
  }
  
  SECTION("higher omega shifts the shelf frequency up")
  {
    LoShelf<float> lsLow, lsHigh;
    lsLow.coeffs = LoShelf<float>::makeCoeffs({0.05f, 0.7f, 2.0f});
    lsHigh.coeffs = LoShelf<float>::makeCoeffs({0.2f, 0.7f, 2.0f});
    // at a mid bin, the higher-omega shelf should still be boosting
    REQUIRE(getMagnitudes(lsHigh)[N / 8] > getMagnitudes(lsLow)[N / 8]);
  }
}

// ================================================================
// HiShelf
// ================================================================

TEST_CASE("madronalib/filters/hishelf", "[filters]")
{
  SECTION("boost increases high frequency energy")
  {
    HiShelf<float> hs;
    hs.coeffs = HiShelf<float>::makeCoeffs({0.1f, 0.7f, 2.0f});
    auto mag = getMagnitudes(hs);
    REQUIRE(mag[N / 2 - 1] > mag[0] * 1.5f);
  }
  
  SECTION("cut reduces high frequency energy")
  {
    HiShelf<float> hs;
    hs.coeffs = HiShelf<float>::makeCoeffs({0.1f, 0.7f, 0.5f});
    auto mag = getMagnitudes(hs);
    REQUIRE(mag[N / 2 - 1] < mag[0]);
  }
  
  SECTION("A=1 is unity")
  {
    HiShelf<float> hs;
    hs.coeffs = HiShelf<float>::makeCoeffs({0.1f, 0.7f, 1.0f});
    auto mag = getMagnitudes(hs);
    REQUIRE(std::fabs(mag[0] - mag[N / 4]) < 0.05f);
  }
  
  SECTION("lower omega shifts the shelf frequency down")
  {
    HiShelf<float> hsLow, hsHigh;
    hsLow.coeffs = HiShelf<float>::makeCoeffs({0.05f, 0.7f, 2.0f});
    hsHigh.coeffs = HiShelf<float>::makeCoeffs({0.2f, 0.7f, 2.0f});
    // at a mid bin, the lower-omega shelf should already be boosting
    REQUIRE(getMagnitudes(hsLow)[N / 8] > getMagnitudes(hsHigh)[N / 8]);
  }
}

// ================================================================
// Bell
// ================================================================

TEST_CASE("madronalib/filters/bell", "[filters]")
{
  SECTION("has a peak near cutoff frequency when boosting")
  {
    Bell<float> b;
    b.coeffs = Bell<float>::makeCoeffs({0.1f, 0.5f, 2.0f});
    auto mag = getMagnitudes(b);
    long peakBin = std::max_element(mag.begin(), mag.end()) - mag.begin();
    REQUIRE(peakBin >= 4);
    REQUIRE(peakBin <= 9);
  }
  
  SECTION("DC and Nyquist are near unity when boosting")
  {
    Bell<float> b;
    b.coeffs = Bell<float>::makeCoeffs({0.1f, 0.5f, 2.0f});
    auto mag = getMagnitudes(b);
    REQUIRE(std::fabs(mag[0] - 1.0f) < 0.1f);
    REQUIRE(std::fabs(mag[N / 2 - 1] - 1.0f) < 0.1f);
  }
  
  SECTION("A=1 is unity")
  {
    Bell<float> b;
    b.coeffs = Bell<float>::makeCoeffs({0.1f, 0.5f, 1.0f});
    auto mag = getMagnitudes(b);
    REQUIRE(std::fabs(mag[0] - mag[N / 4]) < 0.05f);
  }
  
  SECTION("cut creates a notch near cutoff frequency")
  {
    Bell<float> b;
    b.coeffs = Bell<float>::makeCoeffs({0.1f, 0.5f, 0.5f});
    auto mag = getMagnitudes(b);
    long minBin = std::min_element(mag.begin(), mag.end()) - mag.begin();
    REQUIRE(minBin >= 4);
    REQUIRE(minBin <= 9);
  }
}

// ================================================================
// OnePole
// ================================================================

TEST_CASE("madronalib/filters/onepole", "[filters]")
{
  SECTION("DC passes through")
  {
    OnePole<float> op;
    op.coeffs = OnePole<float>::makeCoeffs({0.1f});
    auto mag = getMagnitudes(op);
    REQUIRE(std::fabs(mag[0] - 1.0f) < 0.05f);
  }
  
  SECTION("monotonically decreasing magnitude")
  {
    OnePole<float> op;
    op.coeffs = OnePole<float>::makeCoeffs({0.05f});
    auto mag = getMagnitudes(op);
    for (int i = 1; i < N / 2; ++i)
    {
      REQUIRE(mag[i] <= mag[i - 1] + 1e-6f);
    }
  }
}

// ================================================================
// DCBlocker
// ================================================================

TEST_CASE("madronalib/filters/dcblocker", "[filters]")
{
  SECTION("DC is rejected")
  {
    DCBlocker<float> dc;
    dc.coeffs = DCBlocker<float>::makeCoeffs({0.045f});
    
    // feed constant DC and verify output decays toward zero
    SignalBlock dcInput(1.0f);
    SignalBlock output;
    for (int i = 0; i < 200; ++i)
    {
      output = dc(dcInput);
    }
    // after settling, output should be near zero
    REQUIRE(std::fabs(output[kFramesPerBlock - 1]) < 0.01f);
  }
  
  SECTION("high frequencies pass through")
  {
    DCBlocker<float> dc;
    dc.coeffs = DCBlocker<float>::makeCoeffs({0.045f});
    
    // alternating +1/-1 is at Nyquist — should pass through
    SignalBlock nyquist;
    for (int i = 0; i < kFramesPerBlock; ++i)
      nyquist[i] = (i & 1) ? -1.f : 1.f;
    
    // settle
    SignalBlock output;
    for (int i = 0; i < 10; ++i)
      output = dc(nyquist);
    
    REQUIRE(rms(output) > 0.9f);
  }
}
// ================================================================
// Allpass1
// ================================================================

TEST_CASE("madronalib/filters/allpass1", "[filters]")
{
  SECTION("magnitude is flat across spectrum")
  {
    Allpass1<float> ap(0.5f);
    auto mag = getMagnitudes(ap);
    // all bins should be near the same magnitude
    float avg = 0.f;
    for (int i = 0; i < N / 2; ++i) avg += mag[i];
    avg /= (N / 2);
    for (int i = 0; i < N / 2; ++i)
    {
      REQUIRE(std::fabs(mag[i] - avg) < 0.1f);
    }
  }
  
  SECTION("different coefficients give same magnitude but different phase")
  {
    Allpass1<float> ap1(0.3f);
    Allpass1<float> ap2(0.7f);
    auto mag1 = getMagnitudes(ap1);
    auto mag2 = getMagnitudes(ap2);
    // magnitudes should be nearly identical
    for (int i = 0; i < N / 2; ++i)
    {
      REQUIRE(std::fabs(mag1[i] - mag2[i]) < 0.1f);
    }
    // but the outputs should actually differ (different phase)
    SignalBlock impulse{0.f};
    impulse[0] = 1.0f;
    ap1 = Allpass1<float>(0.3f);
    ap2 = Allpass1<float>(0.7f);
    SignalBlock out1 = ap1(impulse);
    SignalBlock out2 = ap2(impulse);
    bool differ = false;
    for (int i = 0; i < kFramesPerBlock; ++i)
    {
      if (std::fabs(out1[i] - out2[i]) > 1e-6f) { differ = true; break; }
    }
    REQUIRE(differ);
  }
}

TEST_CASE("madronalib/filters/pink_filter_rolloff", "[filters]")
{
  constexpr int kFFTOrder = 6;
  ffft::FFTRealFixLen<kFFTOrder> fft;
  constexpr int N = kFramesPerBlock;
  
  auto measureRolloff = [&](float sr) {
    PinkFilter<float> pf;
    pf.init(sr);
    
    // impulse response
    SignalBlock impulse{0.f};
    impulse[0] = 1.0f;
    SignalBlock output = pf(impulse);
    
    // FFT
    std::array<float, N> fftOut{};
    fft.do_fft(fftOut.data(), output.data());
    
    // magnitude at a given bin
    auto magAtBin = [&](int bin) {
      float re = fftOut[bin];
      float im = (bin == 0) ? 0.f : fftOut[bin + N / 2];
      return sqrtf(re * re + im * im);
    };
    
    // bins are spaced at sr/N Hz
    // pick octave-spaced bins: bin 4, 8, 16 → frequencies sr/16, sr/8, sr/4
    float mag4 = magAtBin(4);
    float mag8 = magAtBin(8);
    float mag16 = magAtBin(16);
    
    // each octave up should drop roughly 3 dB (factor ~0.707)
    float drop_4_to_8 = 20.f * log10f(mag8 / mag4);
    float drop_8_to_16 = 20.f * log10f(mag16 / mag8);
    
    return std::make_pair(drop_4_to_8, drop_8_to_16);
  };
  
  SECTION("44100 Hz: ~3 dB per octave")
  {
    auto [drop1, drop2] = measureRolloff(44100.f);
    REQUIRE(drop1 < -1.5f);
    REQUIRE(drop1 > -4.5f);
    REQUIRE(drop2 < -1.5f);
    REQUIRE(drop2 > -4.5f);
  }
  
  SECTION("96000 Hz: ~3 dB per octave")
  {
    auto [drop1, drop2] = measureRolloff(96000.f);
    REQUIRE(drop1 < -1.5f);
    REQUIRE(drop1 > -4.5f);
    REQUIRE(drop2 < -1.5f);
    REQUIRE(drop2 > -4.5f);
  }
}

TEST_CASE("madronalib/filters/ladder", "[filters]")
{
  // small impulse to stay in the linear region of tanh
  auto getLadderMagnitudes = [&](LadderFilter<float>& lf) {
    constexpr int kFFTOrder = 6;
    ffft::FFTRealFixLen<kFFTOrder> fft;
    
    SignalBlock impulse{0.f};
    impulse[0] = 0.01f;
    
    SignalBlock output = lf(impulse);
    
    std::array<float, N> fftOut{};
    fft.do_fft(fftOut.data(), output.data());
    
    std::array<float, N / 2> mag{};
    for (int i = 0; i < N / 2; ++i)
    {
      float re = fftOut[i];
      float im = (i == 0) ? 0.f : fftOut[i + N / 2];
      mag[i] = sqrtf(re * re + im * im);
    }
    return mag;
  };
  
  SECTION("lowpass: DC passes, high frequencies attenuated")
  {
    LadderFilter<float> lf;
    lf.mode = LadderFilter<float>::kLopass;
    lf.coeffs = LadderFilter<float>::makeCoeffs({0.1f, 0.0f});
    auto mag = getLadderMagnitudes(lf);
    REQUIRE(mag[0] > 0.001f);
    for (int i = N / 4; i < N / 2; ++i)
      REQUIRE(mag[i] < mag[0] * 0.25f);
  }
  
  SECTION("highpass: DC rejected, high frequencies pass")
  {
    LadderFilter<float> lf;
    lf.mode = LadderFilter<float>::kHipass;
    lf.coeffs = LadderFilter<float>::makeCoeffs({0.1f, 0.0f});
    auto mag = getLadderMagnitudes(lf);
    REQUIRE(mag[0] < 0.0001f);
    float upperAvg = 0.f;
    for (int i = N / 4; i < N / 2; ++i) upperAvg += mag[i];
    upperAvg /= (N / 4);
    REQUIRE(upperAvg > mag[0] * 10.f);
  }
  
  SECTION("bandpass: DC rejected, peak near cutoff")
  {
    LadderFilter<float> lf;
    lf.mode = LadderFilter<float>::kBandpass;
    lf.coeffs = LadderFilter<float>::makeCoeffs({0.1f, 0.0f});
    auto mag = getLadderMagnitudes(lf);
    REQUIRE(mag[0] < 0.0001f);
    int peakBin = std::max_element(mag.begin(), mag.end()) - mag.begin();
    REQUIRE(peakBin >= 3);
    REQUIRE(peakBin <= 12);
  }
  
  SECTION("resonance peaks near cutoff")
  {
    LadderFilter<float> lf;
    lf.mode = LadderFilter<float>::kLopass;
    lf.coeffs = LadderFilter<float>::makeCoeffs({0.1f, 0.9f});
    auto mag = getLadderMagnitudes(lf);
    float peak = *std::max_element(mag.begin() + 1, mag.end());
    REQUIRE(peak > mag[0]);
  }
  
  SECTION("steeper than 2-pole SVF")
  {
    LadderFilter<float> ladder;
    ladder.mode = LadderFilter<float>::kLopass;
    ladder.coeffs = LadderFilter<float>::makeCoeffs({0.1f, 0.0f});
    auto magLadder = getLadderMagnitudes(ladder);
    
    Lopass<float> svf;
    svf.coeffs = Lopass<float>::makeCoeffs({0.1f, 0.5f});
    
    // scale SVF impulse the same way
    SignalBlock impulse{0.f};
    impulse[0] = 0.01f;
    SignalBlock svfOut = svf(impulse);
    ffft::FFTRealFixLen<kFFTOrder> fft;
    std::array<float, N> fftOut{};
    fft.do_fft(fftOut.data(), svfOut.data());
    std::array<float, N / 2> magSVF{};
    for (int i = 0; i < N / 2; ++i)
    {
      float re = fftOut[i];
      float im = (i == 0) ? 0.f : fftOut[i + N / 2];
      magSVF[i] = sqrtf(re * re + im * im);
    }
    
    REQUIRE(magLadder[N / 2 - 1] < magSVF[N / 2 - 1]);
  }
}
