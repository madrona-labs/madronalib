

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "mldsp.h"

#ifdef _WINDOWS
#include "Windows.h"
#endif

using namespace ml;

constexpr float mySinFillFn(int n){ return const_math::sin(n*kTwoPi/(kFramesPerBlock));  }
	
int main()
{
#ifdef _WINDOWS
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif

	std::cout << "DSP Ops:\n";
	
 	// columnIndex()
	SignalBlock ci = columnIndex();
	std::cout << "index: " << ci << "\n\n";

	// generate a vector using map() and columnIndex()
	std::cout << "index squared: " << map(([](float x) {return x*x;}), ci) << "\n\n";

	auto sinMadronaLib = sin(rangeOpen(0, kTwoPi));
	std::cout << "madronalib sin: " << sinMadronaLib << "\n\n";

	// store a lambda on (SignalBlock)->(SignalBlock) defined using map(float)->(float)
	auto sinNative = [&](const SignalBlock& x){ return map( [](float x){ return sinf(x*kTwoPi/(kFramesPerBlock)); }, x); }(columnIndex());
	std::cout << "native sin: " << sinNative << "\n\n";

	std::cout << "difference from native: " << sinNative - sinMadronaLib << "\n\n";

	// constexpr fill. unfortunately this cannot be made to work with a lambda in C++11.
	constexpr SignalBlock kSinVec(mySinFillFn);
	std::cout << "constexpr sin table: " << kSinVec << "\n\n";

	std::cout << "difference from native: " << sinNative - kSinVec << "\n\n";

	SignalBlockInt iv1(23);
	std::cout << "int fill: " << iv1 << "\n\n";

	SignalBlockInt iv2(truncateFloatToInt(columnIndex()));
	std::cout << "int index: " << iv2 << "\n\n";

	NoiseGen<float> r;
	SignalBlockInt iv3(truncateFloatToInt(r()*SignalBlock(64)));
	std::cout << "rand ints in [-64, 64]: " << iv3 << "\n\n";
	
	// for filters example / test
	FDN<4> f;
	// NOTE: the minimum possible delay time is kFramesPerBlock.
	f.setDelaysInSamples({{67, 73, 91, 103}});
	f.setFilterCutoffs({{0.1f, 0.2f, 0.3f, 0.4f}});
	f.mFeedbackGains = {{0.5f, 0.5f, 0.5f, 0.5f}};
	SignalBlock silence, impulse;
	impulse[0] = 1.f;
	std::cout << "silence:" << silence << "\n";
	std::cout << "impulse:" << impulse << "\n";
	std::cout << f(impulse) << "\n";
	std::cout << f(silence) << "\n";
	std::cout << f(silence) << "\n";
	
	OnePole<float> op;
	op.coeffs = OnePole<float>::makeCoeffs({0.15f});
	std::cout << op(impulse) << "\n";
	std::cout << op(silence) << "\n";
	std::cout << op(silence) << "\n";

	// clear filter
	op = OnePole<float>();
	// restore coeffs
	op.coeffs = OnePole<float>::makeCoeffs({0.15f});
	std::cout << op(silence) << "\n";
	std::cout << op(impulse) << "\n";
	
	
	TickGen<float> ticks(16);
	

	Lopass<float> lp1;
	lp1.coeffs = Lopass<float>::makeCoeffs({0.25f, 1.0f});
	
	
	SignalBlock tick;
	tick[0] = 1;


	// upsampler for a generator with 1 input row
  TestSineGen<float> sineGen;
	Upsample2xFunction<1> upper;
	std::cout << "\n\n" << upper([&](const SignalBlock x) { return sineGen(x); }, SignalBlock(440.f / 44100.f)) << "\n\n";


	// IntegerDelay p(100);
	//std::cout << "\n\n" << p(SignalBlock(), SignalBlock()) << "\n" << p(SignalBlock(), SignalBlock()) << "\n\n";

//	SignalBlock y = lp1(tick);
	auto lpTestFn = [&](const SignalBlock x){ return (x); };


	FeedbackDelayFunction feedbackFn;

	// set the delay time. a time < kFramesPerBlock will not work.
	SignalBlock vDelayTime2(65.f);
	std::cout << feedbackFn(tick, lpTestFn, vDelayTime2 ) << "\n";
	for(int i=0; i<4; ++i)
	{
		std::cout << feedbackFn(SignalBlock(), lpTestFn, vDelayTime2 ) << "\n";
	}

	std::cout << "\n\n\n\n";

	SignalBlock tick2;
	tick2[20] = 1; // after PitchbendableDelay warmup

	PitchbendableDelay pd1;
	SignalBlock vDelayTime3(4.f);
	std::cout << pd1(tick2, vDelayTime3) << "\n";
	for(int i=0; i<4; ++i)
	{
		std::cout << pd1(SignalBlock(), vDelayTime3) << "\n";
	}
	std::cout << pd1(tick2, vDelayTime3) << "\n";
	std::cout << pd1(tick2, 4.f) << "\n";

#ifdef _WINDOWS
	system("pause");
#endif

}

