#!/usr/bin/env bash
set -euo pipefail

symbolizer_path=""
if command -v llvm-symbolizer >/dev/null 2>&1; then
  symbolizer_path="$(command -v llvm-symbolizer)"
elif command -v llvm-symbolizer-18 >/dev/null 2>&1; then
  symbolizer_path="$(command -v llvm-symbolizer-18)"
elif command -v llvm-symbolizer-17 >/dev/null 2>&1; then
  symbolizer_path="$(command -v llvm-symbolizer-17)"
fi

asan_options="detect_leaks=1"
if [[ -n "${symbolizer_path}" ]]; then
  asan_options+=";symbolize=1;external_symbolizer_path=${symbolizer_path}"
else
  asan_options+=";symbolize=0"
fi

export ASAN_OPTIONS="${asan_options}"
export LSAN_OPTIONS="suppressions=$(pwd)/lsan.supp"

./build-sanitize/Limit_artefacts/Debug/Limit
