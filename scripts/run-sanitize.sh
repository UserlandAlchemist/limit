#!/usr/bin/env bash
set -euo pipefail

symbolizer_path=""
if command -v llvm-symbolizer >/dev/null 2>&1; then
  symbolizer_path="$(command -v llvm-symbolizer)"
elif command -v llvm-symbolizer-19 >/dev/null 2>&1; then
  symbolizer_path="$(command -v llvm-symbolizer-19)"
elif command -v llvm-symbolizer-18 >/dev/null 2>&1; then
  symbolizer_path="$(command -v llvm-symbolizer-18)"
elif command -v llvm-symbolizer-17 >/dev/null 2>&1; then
  symbolizer_path="$(command -v llvm-symbolizer-17)"
fi

if [[ -n "${symbolizer_path}" ]]; then
  export ASAN_SYMBOLIZER_PATH="${symbolizer_path}"
  export LLVM_SYMBOLIZER_PATH="${symbolizer_path}"
  export ASAN_OPTIONS="detect_leaks=1:symbolize=1"
else
  echo "llvm-symbolizer not found; sanitizer backtraces will be unsymbolized." >&2
  export ASAN_OPTIONS="detect_leaks=1:symbolize=0"
fi

if [[ "${LIMIT_ASAN_VERBOSE:-}" == "1" ]]; then
  export ASAN_OPTIONS="${ASAN_OPTIONS}:verbosity=1"
fi

export LSAN_OPTIONS="suppressions=$(pwd)/lsan.supp"

./build-sanitize/Limit_artefacts/Debug/Limit
