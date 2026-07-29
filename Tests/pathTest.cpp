// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// a unit test made using the Catch framework in catch.hpp / tests.cpp.


#include "catch.hpp"
#include "madronalib.h"
#include "MLTestUtils.h"

#if _WIN32
#define HAVE_U8_LITERALS 0
#else
#define HAVE_U8_LITERALS 1
#endif

using namespace ml;

TEST_CASE("madronalib/core/path/symbolic", "[path]")
{
  Path p("hello/world/a/b/c/d/e/f/g");
  
  Path a{"a"};
  Path b{"b"};
  Path d{"d"};
  Path p4 (a, b, "c", d);
  Path p5 (p4, "george", p4);
  
  REQUIRE(p5.getSize() == 9);
  REQUIRE(p.beginsWith("hello/world"));
  REQUIRE(!p.beginsWith("hello/world/b"));
  REQUIRE(p.beginsWith(p));
  
  Path q(p, "and/more");
  REQUIRE(!p.beginsWith(q));
  
  auto f1 = butLast(p);
  auto f2 = lastN(p, 2);
}

TEST_CASE("madronalib/core/path/dynamic", "[path]")
{
  TextPath oneElementPath(TextFragment("foo").getText());
  
  TextPath p("hello/world/a/b/c/d/e/f/g");
  auto initialSize = theSymbolTable().getSize();
  
  auto accumTest = [](TextFragment a, TextFragment b) {
    return TextFragment(a, TextFragment("+"), b);
  };
  TextFragment accumTextResult = std::accumulate(++p.begin(), p.end(),
                                    *p.begin(), accumTest);
  REQUIRE(TextFragment("hello+world+a+b+c+d+e+f+g") == accumTextResult);

  TextPath a{"a"};
  TextPath b{"b"};
  TextPath d{"d"};
  TextPath p4 (a, b, "c", d);
  TextPath p5 (p4, "george", p4);
  
  REQUIRE(p5.getSize() == 9);
  REQUIRE(p.beginsWith("hello/world"));
  REQUIRE(!p.beginsWith("hello/world/b"));
  REQUIRE(p.beginsWith(p));
  
  TextPath q(p, "and/more");
  REQUIRE(!p.beginsWith(q));
  
  // DynamicPaths don't affect the Symbol Table
  REQUIRE(theSymbolTable().getSize() == initialSize);
}

TEST_CASE("madronalib/core/symbol/equality", "[symbol]")
{
  Symbol p("hello/world");
  REQUIRE(p == "hello/world");
  REQUIRE(p != "hello/worl");
}

TEST_CASE("madronalib/core/path/equality", "[path]")
{
  Path p("hello/world");
  REQUIRE(p == "hello/world");
  REQUIRE(p != "hello/worl");
}

TEST_CASE("madronalib/core/path/init-list", "[path]")
{
  Path sum;
  for(Path p : PathList{"menu/lfo/rate", "menu/lfo/ratio", "menu/lfo/amount", "menu/learn/amount"})
  {
    sum = Path(sum, p);
  }
  REQUIRE(sum.getSize() == 12);
}


TEST_CASE("madronalib/core/path/hash-only", "[path]")
{
  // hashOnlyPath() must produce exactly the hashes runtimePath() produces,
  // so that it finds nodes created with registered Symbols.
  const char* cases[] = {
    "hello/world",
    "osc/amp",
    "a",
    "/leading/separator",
    "trailing/separator/",
    "double//separator",
    "///",
    "",
    "a/b/c/d/e/f/g/h/i/j/k/l/m/n/o",   // kPathMaxSymbols segments
    "été/日本語/café"        // multi-byte UTF-8 segments
  };

  for(const char* c : cases)
  {
    REQUIRE(hashOnlyPath(c) == runtimePath(c));
    REQUIRE(hashOnlyPath(c).getSize() == runtimePath(c).getSize());
  }

  REQUIRE(hashOnlyPath(nullptr) == Path());

  // hashOnlyPath must not register anything in the SymbolTable.
  auto initialSize = theSymbolTable().getSize();
  hashOnlyPath("some/name/never/registered/anywhere/else");
  REQUIRE(theSymbolTable().getSize() == initialSize);
}

TEST_CASE("madronalib/core/tree/hash-only-lookup", "[tree]")
{
  // a Tree populated through operator[] must be findable by hash-only lookup,
  // and the node names must still be recoverable as text.
  Tree< int > t;
  t["alpha/beta"] = 23;
  t["alpha/gamma"] = 42;

  REQUIRE(t.getNode(hashOnlyPath("alpha/beta"))->getValue() == 23);
  REQUIRE(t.getNode(hashOnlyPath("alpha/gamma"))->getValue() == 42);
  REQUIRE(t.getNode(hashOnlyPath("alpha/delta")) == nullptr);

  // creating a node registers its symbols, so names still print as text
  REQUIRE(Symbol("beta").getTextFragment() == "beta");
  REQUIRE(Symbol("gamma").getTextFragment() == "gamma");

  // reading an existing node through the const operator[] must not insert
  const Tree< int >& ct(t);
  REQUIRE(ct["alpha/beta"] == 23);
  REQUIRE(ct["alpha/delta"] == 0);
  REQUIRE(t.getNode(hashOnlyPath("alpha/delta")) == nullptr);
}
