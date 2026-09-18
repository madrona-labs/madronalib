// Oracle generator for madronalib/Tests/scaleTests.cpp.
//
// Emits oracle.csv: (scl, kbm, midinote) -> frequency, computed with Surge XT's
// tuning-library (Paul Walker, MIT). ml::Scale must reproduce these numbers.
//
// Columns: scl,kbm,midinote,freq_hz,mapped,freq_hz_interp
//   freq_hz        Tuning::frequencyForMidiNote (unmapped keys hold a placeholder)
//   mapped         Tuning::isMidiNoteMapped
//   freq_hz_interp same, after withSkippedNotesInterpolated() (what continuous
//                  pitch should sound on an unmapped key)
//   ERROR rows     the scl/kbm pair is rejected by the library
//
// Pseudo-files: scl "none" = 12-ET; kbm "none" = tuning-library's default mapping
// (1/1 on MIDI 60 = 261.6256 Hz); kbm "aalto-default" = madronalib's default
// mapping (1/1 on MIDI 69 = 440 Hz, Aalto 1's convention).
//
// Also prints, to stderr, which files under bad/ the library rejects.
//
// Build against the pinned tuning-library checkout (see gen-oracle.sh).
#include "Tunings.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

int main(int argc, char** argv)
{
  if (argc < 3)
  {
    std::cerr << "usage: gen-oracle <data-dir> <out.csv>\n";
    return 1;
  }
  fs::path dir(argv[1]);
  std::vector<std::string> scls, kbms;
  for (auto& e : fs::directory_iterator(dir))
  {
    auto p = e.path();
    if (p.extension() == ".scl") scls.push_back(p.filename().string());
    if (p.extension() == ".kbm") kbms.push_back(p.filename().string());
  }
  std::sort(scls.begin(), scls.end());
  std::sort(kbms.begin(), kbms.end());
  scls.insert(scls.begin(), "none");
  kbms.insert(kbms.begin(), "aalto-default");
  kbms.insert(kbms.begin(), "none");

  std::ofstream os(argv[2]);
  os << "scl,kbm,midinote,freq_hz,mapped,freq_hz_interp\n";
  os << std::setprecision(10);  // significant digits: some mappings reach below 1e-6 Hz

  for (auto& sn : scls)
  {
    Tunings::Scale s;
    try
    {
      s = (sn == "none") ? Tunings::evenTemperament12NoteScale()
                         : Tunings::readSCLFile((dir / sn).string());
    }
    catch (const std::exception& e)
    {
      std::cerr << "SCL ERROR " << sn << ": " << e.what() << "\n";
      continue;
    }
    for (auto& kn : kbms)
    {
      Tunings::KeyboardMapping k;
      try
      {
        if (kn == "aalto-default")
          k = Tunings::startScaleOnAndTuneNoteTo(69, 69, 440.0);
        else if (kn != "none")
          k = Tunings::readKBMFile((dir / kn).string());
      }
      catch (const std::exception& e)
      {
        std::cerr << "KBM ERROR " << kn << ": " << e.what() << "\n";
        continue;
      }
      std::unique_ptr<Tunings::Tuning> t;
      try
      {
        t = std::make_unique<Tunings::Tuning>(s, k);
      }
      catch (const std::exception& e)
      {
        std::cerr << "TUNING ERROR " << sn << " + " << kn << ": " << e.what() << "\n";
        for (int mn = 0; mn < 128; ++mn)
          os << sn << "," << kn << "," << mn << ",ERROR,ERROR,ERROR\n";
        continue;
      }
      auto ti = t->withSkippedNotesInterpolated();
      for (int mn = 0; mn < 128; ++mn)
      {
        os << sn << "," << kn << "," << mn << "," << t->frequencyForMidiNote(mn) << ","
           << (t->isMidiNoteMapped(mn) ? 1 : 0) << "," << ti.frequencyForMidiNote(mn) << "\n";
      }
    }
  }

  // report which bad files are rejected
  for (auto& e : fs::directory_iterator(dir / "bad"))
  {
    auto p = e.path();
    bool rejected = false;
    try
    {
      if (p.extension() == ".scl")
        Tunings::Tuning t(Tunings::readSCLFile(p.string()));
      else if (p.extension() == ".kbm")
        Tunings::Tuning t(Tunings::evenTemperament12NoteScale(), Tunings::readKBMFile(p.string()));
      else
        continue;
    }
    catch (const std::exception& ex)
    {
      rejected = true;
      std::cerr << "bad/" << p.filename().string() << ": rejected (" << ex.what() << ")\n";
    }
    if (!rejected) std::cerr << "bad/" << p.filename().string() << ": ACCEPTED\n";
  }
  return 0;
}
