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

asan_options="detect_leaks=1"
if [[ -n "${symbolizer_path}" ]]; then
  export ASAN_SYMBOLIZER_PATH="${symbolizer_path}"
  export LLVM_SYMBOLIZER_PATH="${symbolizer_path}"
  asan_options+=":symbolize=1:external_symbolizer_path=${symbolizer_path}"
else
  echo "llvm-symbolizer not found; sanitizer backtraces will be unsymbolized." >&2
  asan_options+=":symbolize=0"
fi

if [[ "${LIMIT_ASAN_VERBOSE:-}" == "1" ]]; then
  asan_options+=":verbosity=1"
fi

export ASAN_OPTIONS="${asan_options}"

lsan_supp_path="$(pwd)/lsan.supp"
export LSAN_OPTIONS="suppressions=${lsan_supp_path}:fast_unwind_on_malloc=0:fast_unwind_on_free=0"

./build-sanitize/Limit_artefacts/Debug/Limit
