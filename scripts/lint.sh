#!/usr/bin/env bash
set -euo pipefail

if [[ ! -f build/compile_commands.json ]]; then
  echo "compile_commands.json not found. Run ./scripts/configure.sh first." >&2
  exit 1
fi

root_dir="$(pwd)"
header_filter="^${root_dir}/(src|tests)/"

if command -v rg >/dev/null 2>&1; then
  rg --files -g '*.cpp' -g '*.hpp' src tests | xargs -r clang-tidy -p build -header-filter="$header_filter" --system-headers=0
else
  find src tests -name '*.cpp' -o -name '*.hpp' | xargs -r clang-tidy -p build -header-filter="$header_filter" --system-headers=0
fi
