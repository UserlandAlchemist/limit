# ![Limit logo](assets/limit-logo-final-a-transparent.svg)

Limit is a deliberately constrained, runs-on-the-box software music instrument.
It is not a general-purpose DAW; it is a focused environment for capturing,
committing, and completing musical ideas within a fixed and finite structure.
Constraint is the feature.

## What It Is

- A JUCE-based standalone app with a minimal, controller-first UI
- A linear, bounded musical timeline (no non-linear arranging or micro-editing)
- A small, fixed set of internal instruments/effects built with JUCE's DSP facilities
- Designed around MIDI control (nanoKEY2 + MPD218) and a future Manifold keyboard
  surface

## What It Is Not

Limit explicitly avoids:

- DAW-style editing, comping, or unlimited undo
- Unlimited tracks, devices, routing graphs, or plugin GUIs
- Mouse-centric precision workflows
- Cloud services, background daemons, or online dependencies
- AI composition or generative features

When the constraints are reached, the work is considered finished or handed off elsewhere.

## Repository Layout

- `src/`: JUCE app code and DSP integration
- `tests/`: unit/integration tests (Catch2)
- `assets/`: bundled presets, UI resources, and logos
- `third_party/JUCE/`: JUCE source (submodule recommended)

## Build & Run (Local)

JUCE must be available at `third_party/JUCE` (submodule recommended).

```sh
./scripts/configure.sh
./scripts/build.sh
./scripts/run.sh
```

Run tests (Catch2):

```sh
ctest --test-dir build
```

Formatting and linting:

```sh
./scripts/format.sh
./scripts/lint.sh
./scripts/lint-md.sh
./scripts/format-sh.sh
./scripts/lint-sh.sh
```

Sanitizer build (ASan/UBSan):

```sh
./scripts/sanitize.sh
```

Run sanitized app:

```sh
./scripts/run-sanitize.sh
```

Run sanitized tests:

```sh
./scripts/sanitize-test.sh
```

Leak sanitizer suppressions live in `lsan.supp` (add entries only for known
system-library leaks).
Current suppressions include ALSA (`snd_pcm_open`) from JUCE device init.

For symbolized sanitizer backtraces, install `llvm-symbolizer` (package `llvm`
or `llvm-17/18`) so `run-sanitize.sh` can resolve stack frames.

Valgrind (memcheck on tests):

```sh
./scripts/valgrind.sh
```

## CI

CI runs on GitHub Actions (Ubuntu). It performs configure, build, tests, and lint:

- Build: `./scripts/configure.sh`, `./scripts/build.sh`
- Tests: `ctest --test-dir build`
- Lint: `./scripts/lint.sh`, `./scripts/lint-md.sh`

Local tooling needed for linting:

- `clang-format`, `clang-tidy`
- `mdl` (ruby gem)

## OP-1 Field Influence

Limit borrows the OP-1 Field’s instrument-first workflow and its deliberate bounds:

- A tape-style recording model with fixed length and finite storage (bounded, linear
  time)
- A small, fixed device set: synth, drum, sampler, effects; no plugin sprawl
- Mode-driven interaction (sound, sequence, tape, mix) rather than a DAW
  timeline
- Tape “tricks” and simple edits over deep, mouse-centric micro-editing
- Master processing is post-tape on the output bus (send/master effects operate
  on tape playback)
- Automatic, always-on persistence rather than explicit save management

## References

- Design specification: `docs/design.md`.
- Control scheme and dev keyboard emulation: `docs/control-scheme.md`.

## License

GPL-3.0-or-later. See `LICENSE`.
