// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#include "MLPath.h"

#include "MLTextUtils.h"
#include "utf.hpp"

namespace ml
{


// Runtime Path creation - parses string and registers Symbols

Path runtimePath(const char* str)
{
  Path p;
  if (!str) return p;
  
  const char separator = '/';
  const char* q = str;
  while (*q)
  {
    while (*q == separator) ++q;
    const char* segStart = q;
    while (*q && (*q != separator)) ++q;
    size_t len = q - segStart;
    if (len > 0) p.addElement(Symbol(segStart, len));
  }
  return p;
}

Path runtimePath(const Symbol& sym) { return runtimePath(sym.getUTF8Ptr()); }

Path runtimePath(const TextFragment& frag) { return runtimePath(frag.getText()); }


// Path creation for lookup only - hashes segments without registering Symbols.
// Same scan as runtimePath() above, but using the constexpr Symbol(hash) ctor,
// so the SymbolTable is never touched: no allocation, no lock, no global state.

Path hashOnlyPath(const char* str)
{
  Path p;
  if (!str) return p;

  const char separator = '/';
  const char* q = str;
  while (*q)
  {
    while (*q == separator) ++q;
    const char* segStart = q;
    while (*q && (*q != separator)) ++q;
    size_t len = q - segStart;
    if (len > 0) p.addElement(Symbol(fnv1aRuntime(segStart, len)));
  }
  return p;
}


// Runtime TextPath creation - parses string and creates TextFragments


TextPath runtimeTextPath(const char* str, const char separator)
{
  TextPath p;
  if (!str) return p;

  auto it = TextFragment::Iterator(str);
  const char* segmentStartPtr = str;

  do
  {
    size_t segmentSizeInBytes = 0;

    // Skip separators
    while (*it == separator)
    {
      segmentStartPtr++;
      ++it;
    }

    // Advance to end of segment
    while ((*it != separator) && (*it != '\0'))
    {
      segmentSizeInBytes += utf::internal::utf_traits<utf::utf8>::write_length(*it);
      ++it;
    }

    // Create and add the TextFragment
    if (segmentSizeInBytes > 0)
    {
      p.addElement(TextFragment(segmentStartPtr, static_cast<int>(segmentSizeInBytes)));
      segmentStartPtr += segmentSizeInBytes;
    }
  } while (*it != '\0');

  return p;
}

TextPath runtimeTextPath(const Symbol& sym) { return runtimeTextPath(sym.getUTF8Ptr()); }

TextPath runtimeTextPath(const TextFragment& frag) { return runtimeTextPath(frag.getText()); }

}  // namespace ml
