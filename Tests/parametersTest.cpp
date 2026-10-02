// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// a unit test made using the Catch framework in catch.hpp / tests.cpp.

#include "catch.hpp"
#include "madronalib.h"
#include "MLTestUtils.h"

using namespace ml;

// Create some parameters.
// TODO read from JSON
void readParameterDescriptions(ParameterDescriptionList& params)
{
  params.push_back( std::make_unique< ParameterDescription >(WithValues{
    { "name", "linear-param" },
    { "range", {0, 1} }
  } ) );

  params.push_back( std::make_unique< ParameterDescription >(WithValues{
    { "name", "log-param" },
    { "range", {0.001, 1} },
    { "log", true },
    { "real_default", 0.05 }
  } ) );
  
  params.push_back( std::make_unique< ParameterDescription >(WithValues{
    { "name", "log-param-with-offset" },
    { "range", {1, 6} },
    { "log", true },
    { "offset", -1.f },
    { "real_default", 0.0 }
  } ) );

  params.push_back( std::make_unique< ParameterDescription >(WithValues{
    { "name", "log-param-zero-thresh" },
    { "range", {0.005, 5} },
    { "log", true },
    { "zero_thresh", true },
    { "real_default", 0.01 }
  } ) );
}

// Test to confirm that the projections are invertible
TEST_CASE("madronalib/core/parameters", "[parameters]")
{
  theSymbolTable().clear();
  
  ParameterStore params;
  ParameterDescriptionList pdl;

  // make parameters and projections and set defaults
  readParameterDescriptions(pdl);
  
  // build the parameter tree, creating projections
  buildParameterStore(pdl, params);
  
  params.setValue("linear-param", 0.88f);
  auto v1 = params.getRealFloatValue("linear-param");
  
  // build name index
  std::vector< Path > paramNames;
  for(size_t i=0; i < pdl.size(); ++i)
  {
    ParameterDescription& pd = *pdl[i];
    paramNames.push_back(runtimePath(pd.getTextProperty("name")));
  }
  
  for(auto & pname : paramNames)
  {
    // TODO fix this weird access with a Parameter class (see MLParameters TODO)
    // this should allow us to forget about the name list (though that is sometimes useful)
    // and just write for(auto& param : params).
    ParameterDescription& pdesc = *params.descriptions[pname];
    ParameterProjection& pproj = params.projections[pname];
    
    int nSteps{10};
    for(int i=0; i<=nSteps; ++i)
    {
      float fNorm = i/float(nSteps);
      float fReal = pproj.normalizedToReal(fNorm);
      float fNorm2 = pproj.realToNormalized(fReal);
      
      REQUIRE(testUtils::nearlyEqual(fNorm, fNorm2));
    }
  }
}

// a log parameter with zero_thresh reaches zero at the bottom of its range
TEST_CASE("madronalib/core/parameters/zero_thresh", "[parameters]")
{
  theSymbolTable().clear();

  ParameterStore params;
  ParameterDescriptionList pdl;
  readParameterDescriptions(pdl);
  buildParameterStore(pdl, params);

  Path pname("log-param-zero-thresh");
  ParameterProjection& pproj = params.projections[pname];

  // the bottom of the dial is zero, not the low end of the log range
  REQUIRE(pproj.normalizedToReal(0.f) == 0.f);
  REQUIRE(pproj.realToNormalized(0.f) == 0.f);

  // the rest of the range is the plain log curve
  REQUIRE(pproj.normalizedToReal(1.f) == Approx(5.f));
  REQUIRE(pproj.normalizedToReal(0.5f) == Approx(std::sqrt(0.005f * 5.f)));

  // real values under the threshold are zero on the dial
  REQUIRE(pproj.realToNormalized(0.005f) == 0.f);
  REQUIRE(pproj.realToNormalized(0.001f) == 0.f);

  // above the threshold the projections invert each other
  for (float fReal : {0.006f, 0.01f, 0.25f, 5.f})
  {
    float fNorm = pproj.realToNormalized(fReal);
    REQUIRE(fNorm > 0.f);
    REQUIRE(pproj.normalizedToReal(fNorm) == Approx(fReal));
  }

  // the store never holds a real value between zero and the threshold
  params.setFromRealValue(pname, 0.005f);
  REQUIRE(params.getRealFloatValue("log-param-zero-thresh") == 0.f);
  REQUIRE(params.getNormalizedFloatValue("log-param-zero-thresh") == 0.f);

  params.setFromNormalizedValue(pname, 0.f);
  REQUIRE(params.getRealFloatValue("log-param-zero-thresh") == 0.f);

  params.setFromRealValue(pname, 0.01f);
  REQUIRE(params.getRealFloatValue("log-param-zero-thresh") == Approx(0.01f));

  // a plain log parameter is unchanged: its low end is the bottom of the dial
  ParameterProjection& plain = params.projections["log-param"];
  REQUIRE(plain.normalizedToReal(0.f) == Approx(0.001f));
}
