// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#pragma once

#include <functional>
#include <iostream>
#include <vector>

#include "MLDSPMath.h"

namespace ml
{
struct Interval
{
  float x1;
  float x2;
  inline bool operator==(const Interval& b) const { return (x1 == b.x1) && (x2 == b.x2); }
  inline bool operator!=(const Interval& b) const { return !(operator==(b)); }
  inline const Interval operator*(const float b) const { return Interval{x1 * b, x2 * b}; }
  inline const Interval operator*=(const float b)
  {
    x1 *= b;
    x2 *= b;
    return *this;
  }
};

inline float midpoint(Interval m) { return (m.x1 + m.x2) * 0.5f; }
inline bool within(float f, const Interval m) { return (f >= m.x1) && (f < m.x2); }

using Projection = std::function<float(float)>;

inline Projection compose(Projection a, Projection b)
{
  return [=](float x) { return a(b(x)); };
}

namespace projections
{
// useful projections with no parameters defined on (0, 1)

static const Projection zero{[](float x) { return 0.f; }};
static const Projection unity{[](float x) { return x; }};
static const Projection squared{[](float x) { return x * x; }};
static const Projection flip{[](float x) { return 1 - x; }};
static const Projection clip{[](float x) { return ml::clamp(x, 0.f, 1.f); }};
static const Projection smoothstep{[](float x) { return 3 * x * x - 2 * x * x * x; }};
static const Projection flatcenter{[](float x)
                                   {
                                     float c = (x - 0.5f);
                                     return 4 * c * c * c + 0.5f;
                                   }};
static const Projection bell{[](float x)
                             {
                               float px = x * 2 - 1;
                               return powf(2.f, -(10.f * px * px));
                             }};
static const Projection easeOut{[](float x)
                                {
                                  float m = x - 1;
                                  return 1 - m * m;
                                }};
static const Projection easeIn{[](float x) { return x * x; }};
static const Projection easeInOut{[](float x) {
  return (x < 0.5f) ? easeIn(x * 2.f) * 0.5f : easeOut(x * 2.f - 1.f) * 0.5f + 0.5f;
}};
static const Projection easeOutCubic{[](float x)
                                     {
                                       float n = 1 - x;
                                       return 1 - n * n * n;
                                     }};
static const Projection easeInCubic{[](float x) { return x * x * x; }};
static const Projection easeInOutCubic{[](float x) {
  return (x < 0.5f) ? easeInCubic(x * 2.f) * 0.5f : easeOutCubic(x * 2.f - 1.f) * 0.5f + 0.5f;
}};
static const Projection easeOutQuartic{[](float x)
                                       {
                                         float m = x - 1;
                                         return 1 - m * m * m * m;
                                       }};
static const Projection easeInQuartic{[](float x) { return x * x * x * x; }};
static const Projection easeInOutQuartic{
    [](float x) {
      return (x < 0.5f) ? easeInQuartic(x * 2.f) * 0.5f
                        : easeOutQuartic(x * 2.f - 1.f) * 0.5f + 0.5f;
    }};

static const Projection overshoot{[](float x) { return 3 * x - 2 * x * x; }};

// bisquared projection: x^2, but inverted for x < 0.
static const Projection bisquared{[](float x) { return fabs(x) * x; }};

// inverse of bisquared projection
static const Projection invBisquared{[](float x) { return sqrtf(fabs(x)) * sign(x); }};

// functions taking one or more parameters and returning projections

// return a constant, occasionally useful
inline Projection constant(const float k)
{
  return [=](float x) { return k; };
}

// projections::log returns a projection from [0, 1] to a logarithmic curve
// on [a, b] scaled back to [0, 1]. works for positive a, b with a < b only.
inline Projection log(Interval m)
{
  float a = m.x1;
  float b = m.x2;
  if (b - a == 0.f)
  {
    return [=](float x) { return a; };
  }
  else if (a == 0.f)
  {
    return [=](float x) { return 0.f; };
  }
  else
  {
    return [=](float x) { return a * (powf((b / a), x) - 1) / (b - a); };
  }
}

// the inverse of the log projection.
// works for positive a, b with a < b only.
inline Projection exp(Interval m)
{
  float a = m.x1;
  float b = m.x2;
  if (b - a == 0.f)
  {
    return [=](float x) { return a; };
  }
  else if (a == 0.f)
  {
    return [=](float x) { return 0.f; };
  }
  else
  {
    return [=](float x) { return logf((x * (b - a) + a) / a) / logf(b / a); };
  }
}

// logZeroThresh and expZeroThresh are for log parameters that should reach 0
// at the low end of their travel, which a log curve on [a, b] never does.
// logZeroThresh maps [0, 1] to a log curve on [a, b], with the bottom of the
// range coerced to zero. Unlike log and exp these map to and from the real
// values directly, not a unit curve to be scaled with intervalMap, so that
// zero is exactly zero. works for positive a, b with a < b only.
//
// Values under the threshold, a small step above a, map to zero. So the pair
// is a bijection between {0} and [xt, 1] on the unit side and {0} and
// [a*(1 + kZeroThreshEpsilon), b] on the real side.
constexpr float kZeroThreshEpsilon{0.001f};

inline Projection logZeroThresh(Interval m)
{
  float a = m.x1;
  float b = m.x2;
  if ((b - a == 0.f) || (a == 0.f))
  {
    return [=](float x) { return a; };
  }
  else
  {
    const float thresh = a * (1.f + kZeroThreshEpsilon);
    return [=](float x)
    {
      float y = a * powf((b / a), x);
      return (y < thresh) ? 0.f : y;
    };
  }
}

// the inverse of the logZeroThresh projection.
inline Projection expZeroThresh(Interval m)
{
  float a = m.x1;
  float b = m.x2;
  if ((b - a == 0.f) || (a == 0.f))
  {
    return [=](float x) { return 0.f; };
  }
  else
  {
    const float thresh = a * (1.f + kZeroThreshEpsilon);
    return [=](float x) { return (x < thresh) ? 0.f : logf(x / a) / logf(b / a); };
  }
}

// linear projection mapping an interval to another interval
inline Projection linear(const Interval a, const Interval b)
{
  float a1 = a.x1;
  float a2 = a.x2;
  float b1 = b.x1;
  float b2 = b.x2;

  if (a1 - a2 == 0.f)
  {
    return [=](float x) { return b1; };
  }
  else
  {
    return [=](float x)
    {
      // project interval a to interval b
      float m = (b2 - b1) / (a2 - a1);
      return m * (x - a1) + b1;
    };
  }
}

// linear projection mapping an interval to another interval
inline Projection add(float f)
{
  return [=](float x) { return x + f; };
}

// a projection mapping an interval to another interval with an intermediate
// shaping projection on [0, 1]
inline Projection intervalMap(const Interval a, const Interval b, Projection c)
{
  return [=](float x)
  {
    // project interval a to interval (0,1)
    const float scaleA = 1 / (a.x2 - a.x1);
    const float offsetA = (-a.x1) / (a.x2 - a.x1);
    // project interval (0, 1) to interval b
    const float scaleB = (b.x2 - b.x1);
    const float offsetB = b.x1;
    return c(x * scaleA + offsetA) * scaleB + offsetB;
  };
}

// commonly used pair of Projections to go from [0, 1] to a log parameter and back.
inline Projection unityToLogParam(Interval paramInterval)
{
  return projections::intervalMap({0, 1}, paramInterval, projections::log(paramInterval));
}

inline Projection logParamToUnity(Interval paramInterval)
{
  return projections::intervalMap(paramInterval, {0, 1}, projections::exp(paramInterval));
}

// a piecewiseLinear Projection is specifed with n output values-
// these are equally distributed over [0, 1]. So with 3 output
// values (a, b, c) we get a two-line function going from (0, a) to
// (0.5, b) to (1, c).
inline Projection piecewiseLinear(std::initializer_list<float> values)
{
  const std::vector<float> table(values);

  if (table.size() > 1)
  {
    return [=](float x)
    {
      int ni = (int)table.size() - 1;
      float nf = static_cast<float>(ni);
      float xf = nf * clamp(x, 0.f, 1.f);
      int xi = static_cast<int>(xf);
      float xr = xf - xi;

      if (x < 1.0f)
      {
        return lerp(table[xi], table[xi + 1], xr);
      }
      else
      {
        return table[ni];
      }
    };
  }
  else if (table.size() == 1)
  {
    return [=](float x) { return table[0]; };
  }
  else
  {
    return [=](float x) { return 0.f; };
  }
}

// like piecewiseLinear, but with a shape for each segment for easing and such

inline Projection piecewise(std::initializer_list<float> valueList,
                            std::initializer_list<Projection> shapeList)
{
  // TODO asserts not working on Windows
  // assert(shapeList.size() == valueList.size() - 1);
  const std::vector<float> table(valueList);
  const std::vector<Projection> shapeTable(shapeList);

  if (table.size() > 1)
  {
    return [=](float x)
    {
      int ni = (int)table.size() - 1;
      float nf = static_cast<float>(ni);
      float xf = nf * clamp(x, 0.f, 1.f);
      int xi = static_cast<int>(xf);
      float xr = xf - xi;

      if (x < 1.0f)
      {
        // map xr to shape
        float xrm = shapeTable[xi](xr);
        return lerp(table[xi], table[xi + 1], xrm);
      }
      else
      {
        return table[ni];
      }
    };
  }
  else if (table.size() == 1)
  {
    return [=](float x) { return table[0]; };
  }
  else
  {
    return [=](float x) { return 0.f; };
  }
}

inline void printTable(const Projection& p, std::string pName, Interval domain, size_t points)
{
  Projection pointToX = projections::linear(Interval{0.f, points - 1.f}, domain);
  std::cout << "\n----------------\n";
  std::cout << pName << ": \n";
  for (int i = 0; i < points; ++i)
  {
    float x = pointToX((float)i);
    float y = p(x);
    std::cout << i << ": (" << x << ", " << y << ")\n";
  }
}

}  // namespace projections

inline std::ostream& operator<<(std::ostream& out, const ml::Interval& m)
{
  out << "[" << m.x1 << " - " << m.x2 << "]";
  return out;
}

}  // namespace ml
