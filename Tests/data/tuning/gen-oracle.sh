#!/bin/sh
# Regenerate oracle.csv from the scl/kbm files in this directory.
# Needs a checkout of surge-synthesizer/tuning-library at the sha vendored in
# external/tuning-library (the last C++17 commit; later ones only change file-path
# handling and require C++20, and produce this same table).
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
TL_SHA=748d639
TL="${TL:-$HERE/tuning-library}"
if [ ! -d "$TL/include" ]; then
  git clone -q https://github.com/surge-synthesizer/tuning-library "$TL"
  git -C "$TL" checkout -q "$TL_SHA"
fi
clang++ -std=c++17 -O1 -I "$TL/include" "$HERE/gen-oracle.cpp" -o "$HERE/gen-oracle"
"$HERE/gen-oracle" "$HERE" "$HERE/oracle.csv"
rm -f "$HERE/gen-oracle"
wc -l "$HERE/oracle.csv"
