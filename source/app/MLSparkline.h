// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#pragma once
#include "MLTextUtils.h"
#include "MLDSPOps.h"
#include "MLDSPProjections.h"


namespace ml {


TextFragment sparkline(const SignalBlock& x)
{
  constexpr int kSparkLevels = 7; // _⎽⎼—⎻⎺‾
  const char* kSparkChars[kSparkLevels] = {"_", "⎽", "⎼", "—", "⎻", "⎺", "‾"};
  static auto proj = projections::linear({-1, 1}, {0, kSparkLevels});
  
  std::string result;
  for (int i = 0; i < kFramesPerBlock; ++i)
  {
    float v = proj(x[i]);
    int idx = clamp((int)v, 0, kSparkLevels - 1);
    result += kSparkChars[idx];
  }
  return TextFragment(result.c_str());
}

TextFragment sparkgraph(const SignalBlock& x)
{
  constexpr int kBarLevels = 8;
  const char* kBarChars[kBarLevels] = {"▁", "▂", "▃", "▄", "▅", "▆", "▇", "█"};
  static auto proj = projections::linear({-1, 1}, {0, kBarLevels});
  
  std::string result;
  for (int i = 0; i < kFramesPerBlock; ++i)
  {
    float v = proj(x[i]);
    int idx = clamp((int)v, 0, kBarLevels - 1  );
    result += kBarChars[idx];
  }
  return TextFragment(result.c_str());
}

}  // namespace ml
