# Repository Guidelines

## Project Structure & Module Organization

This repository is currently minimal and only contains `README.md`. As the project grows, prefer a clear top-level layout such as:

- `src/` for JUCE app code and DSP integration
- `tests/` for unit or integration tests
- `assets/` for bundled presets, UI resources, or documentation artifacts

If you introduce new top-level directories, document them in `README.md` and keep names short and descriptive.

## Build, Test, and Development Commands

No build or test commands are defined yet. When you add a build system (e.g., CMake, Meson, or a JUCE project generator), include canonical commands here. Examples to add later:

- `cmake -S . -B build` (configure)
- `cmake --build build` (build)
- `ctest --test-dir build` (run tests)

## Coding Style & Naming Conventions

No style guide is established. If you add C++ code, document the following in this section and enforce it with tooling where possible:

- Indentation (e.g., 2 or 4 spaces)
- File naming (use kebab-case for filenames, e.g., `audio-engine.cpp`, `midi-router.hpp`)
- Formatting tools (e.g., `clang-format` with a checked-in `.clang-format`)

## Testing Guidelines

No testing framework is configured. When tests are introduced, specify:

- Framework (e.g., Catch2, GoogleTest)
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

Limit is intentionally constrained and offline-first. Avoid adding background services, cloud dependencies, or plugin hosting infrastructure without explicit project agreement.
