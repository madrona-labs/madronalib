// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// Tests for ml::Scale against Surge XT's tuning-library.
//
// Tests/data/tuning/oracle.csv holds the frequency tuning-library produces for
// every (scl, kbm, MIDI note) in Tests/data/tuning; see the README there. Aalto
// must sound the same frequencies as Surge for the same files, so ml::Scale is
// held to that table in cents.

#include <cmath>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#include "MLDSPScale.h"
#include "catch.hpp"

using namespace ml;

namespace
{
// this file's directory + "data/tuning/" (no std::filesystem: macOS 10.14 target).
// MSVC's __FILE__ separates with backslashes, so split on either.
const std::string kDataDir =
    std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of("/\\")) + "/data/tuning/";

const char* kBadFiles[] = {"badnote.scl",  "blanknote.scl",   "missingnote.scl",  "blank-line.kbm",
                           "empty-bad.kbm", "garbage-key.kbm", "missing-note.kbm"};

// float log2 of a ratio near 2^7 carries ~1e-7 octaves of rounding, ~1e-4 cents.
constexpr double kToleranceCents = 0.01;

// 12-ET as Scala text, the same notes tuning-library's evenTemperament12NoteScale() holds.
const std::string k12ETScl =
    "! 12-ET\n12 note equal temperament\n 12\n!\n 100.0\n 200.0\n 300.0\n 400.0\n 500.0\n"
    " 600.0\n 700.0\n 800.0\n 900.0\n 1000.0\n 1100.0\n 2/1\n";

// tuning-library's default KeyboardMapping: 1/1 on MIDI 60 = 261.6256 Hz (Surge's bare-.scl root).
const std::string kSurgeDefaultKbm = "! Default KBM file\n0\n0\n127\n60\n60\n261.625565300599\n0\n";

std::string readFile(const std::string& p)
{
  std::ifstream f(p);
  REQUIRE(f.good());
  std::stringstream ss;
  ss << f.rdbuf();
  return ss.str();
}

struct OracleRow
{
  bool error{false};
  double freqHz{0};
  bool mapped{true};
  double freqHzInterp{0};
};

using OracleTable = std::map<std::pair<std::string, std::string>, std::vector<OracleRow>>;

OracleTable readOracle()
{
  OracleTable table;
  std::ifstream f(kDataDir + "oracle.csv");
  REQUIRE(f.good());
  std::string line;
  std::getline(f, line);  // header
  while (std::getline(f, line))
  {
    std::stringstream ss(line);
    std::string scl, kbm, note, freq, mapped, interp;
    std::getline(ss, scl, ',');
    std::getline(ss, kbm, ',');
    std::getline(ss, note, ',');
    std::getline(ss, freq, ',');
    std::getline(ss, mapped, ',');
    std::getline(ss, interp, ',');
    OracleRow r;
    if (freq == "ERROR")
    {
      r.error = true;
    }
    else
    {
      r.freqHz = std::stod(freq);
      r.mapped = (mapped == "1");
      r.freqHzInterp = std::stod(interp);
    }
    auto& rows = table[{scl, kbm}];
    REQUIRE(rows.size() == (size_t)std::stoi(note));
    rows.push_back(r);
  }
  return table;
}

// the .scl / .kbm text a pseudo-name in the oracle stands for.
std::string sclText(const std::string& name)
{
  return (name == "none") ? k12ETScl : readFile(kDataDir + name);
}
std::string kbmText(const std::string& name)
{
  if (name == "aalto-default") return "";  // ml::Scale's default: 1/1 on MIDI 69 = 440 Hz
  if (name == "none") return kSurgeDefaultKbm;
  return readFile(kDataDir + name);
}

double centsFromLogPitch(double logPitch, double freqHz)
{
  return 1200.0 * (logPitch - std::log2(freqHz / 440.0));
}
}  // namespace

TEST_CASE("madronalib/dsp/scale/default is 12-ET with A4 = 440 Hz at 0", "[scale]")
{
  Scale s;
  REQUIRE(s.noteToLogPitch(69.f) == Approx(0.f));
  REQUIRE(s.noteToLogPitch(60.f) == Approx(-0.75f));
  REQUIRE(s.noteToLogPitch(81.f) == Approx(1.f));
  REQUIRE(s.noteToLogPitch(69.5f) == Approx(0.5f / 12.f));
  for (int n = 0; n < kMLNumNotes; ++n)
  {
    REQUIRE(s.isNoteMapped(n));
  }
}

TEST_CASE("madronalib/dsp/scale/matches tuning-library oracle", "[scale]")
{
  auto oracle = readOracle();
  REQUIRE(!oracle.empty());

  for (const auto& [pair, rows] : oracle)
  {
    const auto& [sclName, kbmName] = pair;
    INFO(sclName << " + " << kbmName);
    REQUIRE(rows.size() == 128);

    Scale s;
    bool ok = s.loadScaleFromString(sclText(sclName), kbmText(kbmName));

    if (rows[0].error)
    {
      // tuning-library rejects the pair; so do we, and fall back to 12-ET.
      CHECK_FALSE(ok);
      CHECK(s.noteToLogPitch(60.f) == Approx(-0.75f));
      continue;
    }
    REQUIRE(ok);

    double maxErrCents = 0;
    int worstNote = -1;
    int unmappedMismatches = 0;
    for (int n = 0; n < 128; ++n)
    {
      const auto& r = rows[n];
      if (s.isNoteMapped(n) != r.mapped) unmappedMismatches++;
      // an unmapped key sounds the interpolated pitch, as Surge does for continuous pitch.
      double expected = r.mapped ? r.freqHz : r.freqHzInterp;
      double err = std::fabs(centsFromLogPitch(s.noteToLogPitch((float)n), expected));
      if (err > maxErrCents)
      {
        maxErrCents = err;
        worstNote = n;
      }
    }
    INFO("worst note " << worstNote << ": expected "
                       << (rows[worstNote < 0 ? 0 : worstNote].mapped
                               ? rows[worstNote < 0 ? 0 : worstNote].freqHz
                               : rows[worstNote < 0 ? 0 : worstNote].freqHzInterp)
                       << " Hz, got "
                       << 440.0 * std::exp2(s.noteToLogPitch((float)(worstNote < 0 ? 0 : worstNote)))
                       << " Hz");
    CHECK(maxErrCents < kToleranceCents);
    CHECK(unmappedMismatches == 0);
  }
}

TEST_CASE("madronalib/dsp/scale/rejects bad files and falls back to 12-ET", "[scale]")
{
  int checked = 0;
  for (const char* name : kBadFiles)
  {
    const std::string p = kDataDir + "bad/" + name;
    const std::string ext = p.substr(p.size() - 4);
    INFO(name);
    Scale s;
    s.loadScaleFromString(readFile(kDataDir + "zeus22.scl"));  // something to fall back from
    if (ext == ".scl")
    {
      REQUIRE_FALSE(s.loadScaleFromString(readFile(p)));
    }
    else if (ext == ".kbm")
    {
      REQUIRE_FALSE(s.loadScaleFromString(k12ETScl, readFile(p)));
    }
    else
    {
      continue;
    }
    REQUIRE(s.noteToLogPitch(60.f) == Approx(-0.75f));
    checked++;
  }
  REQUIRE(checked == 7);
}

TEST_CASE("madronalib/dsp/scale/fractional notes interpolate in log pitch", "[scale]")
{
  // two-note scale with a 700-cent step: halfway must be 350 cents, not the ratio midpoint.
  Scale s;
  REQUIRE(s.loadScaleFromString("! fifths\nfifth and octave\n 2\n!\n 700.0\n 2/1\n"));
  float p69 = s.noteToLogPitch(69.f);
  float p70 = s.noteToLogPitch(70.f);
  REQUIRE(p70 - p69 == Approx(700.f / 1200.f));
  REQUIRE(s.noteToLogPitch(69.5f) == Approx(p69 + 350.f / 1200.f).margin(1e-5));
  REQUIRE(s.noteToLogPitch(69.25f) == Approx(p69 + 175.f / 1200.f).margin(1e-5));
}

TEST_CASE("madronalib/dsp/scale/assignment copies the whole tuning", "[scale]")
{
  Scale a;
  REQUIRE(a.loadScaleFromString(readFile(kDataDir + "zeus22.scl"),
                                readFile(kDataDir + "mapping-whitekeys-a440.kbm")));
  Scale b;
  b = a;
  for (int n = 0; n < 128; ++n)
  {
    REQUIRE(b.noteToLogPitch((float)n) == a.noteToLogPitch((float)n));
    REQUIRE(b.isNoteMapped(n) == a.isNoteMapped(n));
  }

  // assigning a fresh Scale resets everything, including note pitch.
  b = Scale();
  REQUIRE(b.noteToLogPitch(60.f) == Approx(-0.75f));
  REQUIRE(b.noteToLogPitch(61.f) == Approx(-0.75f + 1.f / 12.f));
  REQUIRE(b.isNoteMapped(61));
}

TEST_CASE("madronalib/dsp/scale/notes above 127 up to the table end", "[scale]")
{
  // Virta quantizes partials with notes up to kMLNumNotes - 1.
  Scale s;
  for (int n = 128; n < kMLNumNotes; ++n)
  {
    REQUIRE(s.noteToLogPitch((float)n) == Approx((n - 69.f) / 12.f));
  }
  const float top = (kMLNumNotes - 1 - 69.f) / 12.f;
  REQUIRE(s.noteToLogPitch((float)(kMLNumNotes - 1)) == Approx(top));
  REQUIRE(s.noteToLogPitch((float)(kMLNumNotes - 1) + 0.7f) == Approx(top));  // clamps
  REQUIRE(s.noteToLogPitch(1000.f) == Approx(top));
  REQUIRE(s.noteToLogPitch(-5.f) == Approx(-69.f / 12.f));
  REQUIRE(s.noteToLogPitch(std::nanf("")) == 0.f);
}

TEST_CASE("madronalib/dsp/scale/quantizePitch snaps down to a scale note", "[scale]")
{
  // characterization of existing behaviour (sequencer quantize)
  Scale s;
  REQUIRE(s.quantizePitch(0.f) == Approx(0.f));
  REQUIRE(s.quantizePitch(0.3f) == Approx(0.25f));
  REQUIRE(s.quantizePitch(-0.7f) == Approx(-0.75f));
  REQUIRE(s.quantizePitchNearest(0.3f) == Approx(0.25f + 1.f / 12.f));
}

// A plugin parses on the main thread and hands the audio thread a finished
// table through a blob parameter, so the table has to be a plain struct.
TEST_CASE("Scale table round-trips through a trivially copyable struct", "[scale]")
{
  static_assert(std::is_trivially_copyable<Scale::Table>::value, "Scale::Table must be memcpy-able");
  static_assert(sizeof(Scale::Table) == kMLNumNotes * (sizeof(double) + 1), "Scale::Table has no padding to leak");

  Scale a;
  REQUIRE(a.loadScaleFromString(readFile(kDataDir + "zeus22.scl"), readFile(kDataDir + "mapping-whitekeys-a440.kbm")));

  Scale b;
  b.setTable(a.getTable());
  for (int n = 0; n < kMLNumNotes; ++n)
  {
    CHECK(b.noteToLogPitch((float)n) == a.noteToLogPitch((float)n));
    CHECK(b.isNoteMapped(n) == a.isNoteMapped(n));
  }
  CHECK_FALSE(b.isNoteMapped(61));  // a black key the mapping leaves out
  CHECK(b.getName() == "12-equal");  // names stay with the parser, not the table

  // the copy is independent of the source
  a = Scale();
  CHECK(b.noteToLogPitch(60.f) != a.noteToLogPitch(60.f));
}
