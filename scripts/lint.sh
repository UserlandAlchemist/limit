#!/usr/bin/env bash
set -euo pipefail

if [[ ! -f build/compile_commands.json ]]; then
  echo "compile_commands.json not found. Run ./scripts/configure.sh first." >&2
  exit 1
fi

rg --files -g '*.cpp' -g '*.hpp' src tests | xargs -r clang-tidy -p build
