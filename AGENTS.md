# Repository Guidelines

## Project Structure & Module Organization

This repository is currently minimal and only contains initial scaffolding.
As the project grows, prefer a clear top-level layout such as:

- `src/` for JUCE app code and DSP integration
- `tests/` for unit or integration tests
- `assets/` for bundled presets, UI resources, or documentation artifacts
- `third_party/JUCE/` for the JUCE source (submodule recommended)

If you introduce new top-level directories, document them in `README.md` and
keep names short and descriptive.

## Build, Test, and Development Commands

Build system is CMake + Ninja. Canonical commands:

- `./scripts/configure.sh` (configure via CMake + Ninja)
- `./scripts/build.sh` (build)
- `./scripts/run.sh` (launch app)
- `ctest --test-dir build` (run tests)
- `./scripts/format.sh` (apply clang-format)
- `./scripts/lint.sh` (clang-tidy, CppCoreGuidelines enforced)
- `./scripts/lint-md.sh` (markdown lint via mdl)
- `./scripts/sanitize.sh` (ASan/UBSan build)
- `./scripts/run-sanitize.sh` (run sanitized app)
- `./scripts/sanitize-test.sh` (run tests with ASan/UBSan)
- `./scripts/valgrind.sh` (run tests under Valgrind memcheck)

Note: This repo does not permit running `sudo` commands. If elevated installs
are required, provide the exact commands for the user to run manually.

Reproducible builds are enabled by default via compiler prefix-map flags. Keep
paths stable and avoid embedding timestamps in generated assets.

## Coding Style & Naming Conventions

Style and linting:

- Indentation: 2 spaces (see `.clang-format`)
- File naming: kebab-case (e.g., `audio-engine.cpp`, `midi-router.hpp`)
- Formatting: `clang-format` with repo config
- Linting: `clang-tidy` with CppCoreGuidelines as errors
- Prefer the C++ standard library for core data structures and algorithms where
  it fits, before introducing custom utilities.

## Testing Guidelines

Testing uses Catch2 (C++). Guidelines:

- Framework: Catch2 (via CMake FetchContent)
- Test file location and naming (e.g., `tests/TestAudioGraph.cpp`)
- How to run tests locally (command and expected output)

## Commit & Pull Request Guidelines

Use Conventional Commits for all changes. Examples:

- `feat(engine): add bounded recorder`
- `fix(ui): clamp meter range`
- `chore(ci): add build cache`

For pull requests, include:

- A short summary of changes and rationale
- Any new build/test commands or setup notes
- Screenshots or recordings for UI changes, if applicable

## Configuration & Safety Notes

Limit is intentionally constrained and offline-first. Avoid adding background services,
cloud dependencies, or plugin hosting infrastructure without explicit project agreement.

## CppCoreGuidelines

All code must follow the CppCoreGuidelines. Run `./scripts/lint.sh` before committing
changes.

GUI sanitizer runs may require leak suppressions for system libraries (see `lsan.supp`).

## Change Validation

When modifying C++:

- Run `./scripts/format.sh` and `./scripts/lint.sh`.
- Run `./scripts/build.sh` and `ctest --test-dir build`.
- If changes could introduce leaks (allocation, ownership, lifetime, containers,
  new/delete), run `./scripts/valgrind.sh` before committing.

When modifying Markdown:

- Run `./scripts/lint-md.sh`.
