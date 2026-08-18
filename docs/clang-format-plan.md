# Code formatting: audit, and a guide to rolling it out after the next release

## Context

You remembered adding something to format code before check-in and didn't see it running.
It isn't — nothing formats anything today, in any of the five repos. This document is the
audit of what's actually there, plus a step-by-step guide for the one-time sweep and
enforcement, to be done **after the next release**. Nothing here has been executed.

## Where things stand today

- **Nothing is running.** No formatting git hook, no CI lint step, no global git config or
  `init.templateDir`, no Claude hooks in any `settings.json`.
- **The one hook that exists is not a formatter.** `madronalib/.git/hooks/pre-commit`
  refuses commits directly on `main` (overridable with `--no-verify`). It is untracked, so
  it lives only in your clone. Note all five repos are currently sitting on `main`.
- **madronalib has the only real style config**: a tracked `.clang-format`, Google-based,
  with `BreakBeforeBraces: Allman`, `IndentWidth: 2`, `ColumnLimit: 100`.
- **The other repos have none** — `aaltoverb`, `ml-private`, `manzanita`, `aalto`. Only
  vendored `external/miniz` copies in aaltoverb and manzanita, which are third-party and
  already carry their own config.
- **Tools are installed and current**: `clang-format 18.1.8` and `git-clang-format`, both
  from Homebrew. clang-format 18 accepts madronalib's config with no deprecation warnings.
- **The config broadly matches house style.** Allman braces and 2-space indent are
  preserved. The differences are spacing details (below).

### Scale of a full sweep — measured, not estimated

Diff lines if madronalib's config were applied as-is:

| Tree | Files changed | Diff lines |
|---|---|---|
| `ml-private/widgets` | 59 / 59 | ~6,700 |
| `manzanita/source/common` | 29 / 29 | ~5,000 |
| `manzanita/source/widgets` | 24 / 24 | ~1,800 |
| `aaltoverb/Source` | 9 / 12 | ~1,470 |
| `madronalib/source/app` | 35 / 58 | ~1,130 |

Tracked C/C++ files, excluding vendored code: ml-private 251, madronalib 121, manzanita 87,
aalto 67, aaltoverb 12 — **538 total**.

Worth knowing: madronalib is not fully conformant to its own committed config, so this is a
first application everywhere, not a re-application.

## Step 1 — Get team buy-in (three of you) before touching anything

- Circulate the **config plus a real before/after diff**, not the config alone — people
  react to their own code, not to YAML. `ml-private/widgets/MLDial.cpp` is a good sample:
  it shows every contested rule in one file.
- Time-box comments to a fixed date so this doesn't stall.
- Decide each hot take below explicitly, then record the decision in the repo (a short
  `STYLE.md`, or comments in `.clang-format`) so it isn't relitigated.
- **Pin the clang-format version.** Output differs between major versions, so if one person
  runs 17 and another 19, the sweep re-churns forever. Agree on 18.x, write the version
  down, and have everyone check `clang-format --version`.

## Step 2 — Hot takes to settle first

These are the rules that generate most of the churn and are most likely to be argued about.
Each is a one-line change in `.clang-format`.

- **`SpaceBeforeParens: ControlStatements`** — turns `if(hasDetents)` into `if (hasDetents)`.
  The widget code consistently writes `if(`, so this alone is a large share of the diff.
  Set to `Never` to keep the existing look.
- **Spaces around binary operators** — `kTwoPi*0.375f` becomes `kTwoPi * 0.375f`. The DSP
  and animation code deliberately uses tight spacing to signal grouping
  (`_bounceCounter/float(kBounceMs)`, `magnitude(travel)*0.5f`). Widening it can make
  precedence read *worse*. Little direct control here beyond accepting it.
- **`ReflowComments: true` + `ColumnLimit: 100`** — rewraps every hand-wrapped prose comment
  block. ml-private has a lot of carefully wrapped explanatory comments at ~80 columns;
  these will all be reflowed. Consider `ReflowComments: false` to leave prose alone.
- **Short-body collapsing** — `AllowShortLoopsOnASingleLine: true`,
  `AllowShortIfStatementsOnASingleLine: WithoutElse`, `AllowShortFunctionsOnASingleLine: All`
  pull two-line bodies onto one line (`while (angle < a0) angle += kTwoPi;`).
- **`IncludeBlocks: Regroup` with `SortIncludes: false`** — a contradictory pair. Regroup can
  still reshuffle include blocks, which matters here because header order is load-bearing in
  places (`manzanita.h`, `MZWidget.h`). Safest is `IncludeBlocks: Preserve`.
- **`DerivePointerAlignment: true`** — silently overrides `PointerAlignment: Left`, deriving
  from each file's existing majority. Result is *not* consistent repo-wide. Set it `false`
  if consistency is the goal.
- **Modernize the option spellings** while it's under review: the file uses pre-14 names
  (`AllowAllConstructorInitializersOnNextLine`, `ConstructorInitializerAllOnOneLineOrOnePerLine`,
  `AlignOperands: true`). Still accepted by 18, but they'll rot.

## Step 3 — Sweep, one repo at a time

- **Timing:** immediately after the release ships, when no long-lived branches are open. A
  format sweep and an in-flight branch conflict on essentially every line.
- **Suggested order:** `madronalib` → `manzanita` → `ml-private` → `aalto` + `aaltoverb`.
  madronalib first: it owns the config, has a test suite already wired in CI
  (`.github/workflows/build-and-test.yml`), and is the lowest-risk place to discover a bad
  rule. Save ml-private for a moment when both plugins are at a stable point, since both
  build from it.
- **Copy the agreed `.clang-format` to each repo root.** clang-format only walks up from the
  file being formatted, and these are sibling repos, so each needs its own copy.
- **Exclude vendored code** — `aaltoverb/Source/external`, `manzanita/source/external`,
  `madronalib/external`, and the `external/` submodules. Drive the sweep off the tracked
  file list rather than a bare recursive glob:

  ```
  git ls-files '*.cpp' '*.h' '*.mm' | grep -vE '(^|/)external/' | xargs clang-format -i
  ```

  Check each repo for other vendored code before running (ml-private has no `external/`
  directory but should be eyeballed).
- **Commit the sweep entirely on its own**, with no behavior change mixed in, and say so in
  the message. This is what makes it reviewable and revertible.

## Step 4 — Keep `git blame` usable

This is the main cost of a sweep, and it is fully solvable:

- Create `.git-blame-ignore-revs` in each repo containing the sweep commit SHA.
- `git config blame.ignoreRevsFile .git-blame-ignore-revs` (git 2.50.1 here, well past the
  2.23 requirement).
- GitHub honors the file automatically in its blame view, with no configuration.

## Step 5 — Enforce, so it stays swept

- Add a tracked `.githooks/pre-commit` that runs `git clang-format --staged`, formatting only
  the lines the commit touches.
- Activate with `git config core.hooksPath .githooks` — `.git/hooks` isn't tracked, so a
  tracked directory plus this one-time command is what makes the hook survive a fresh clone.
- **In madronalib, fold in the existing no-commits-on-`main` guard**: setting `core.hooksPath`
  stops `.git/hooks/pre-commit` from running, so that guard must move into the new hook or it
  silently disappears.
- Optional CI backstop: fail the build when `git clang-format --diff` against the merge base
  returns anything. Catches anyone who commits with `--no-verify` or a stale clone.

## Verification

- After each repo's sweep, `clang-format --dry-run -Werror` over the same file list should be
  silent.
- **Rebuild every consumer before moving to the next repo.** ml-private and manzanita are
  shared, so a sweep there must be followed by building aaltoverb *and* aalto:
  ```
  cmake --build /Users/randy/dev/aaltoverb/build
  cmake --build /Users/randy/dev/aalto/build
  ```
- Run madronalib's existing test suite after its sweep.
- Confirm the sweep commit is genuinely behavior-free: skim `git show --stat`, and check that
  the diff contains no changed string literals or numeric constants.
- Sanity-check the hook by making a deliberately misformatted one-line edit and committing —
  it should come out formatted.
- Confirm blame still works: `git blame` a swept file and check it points past the sweep
  commit to the real author.
