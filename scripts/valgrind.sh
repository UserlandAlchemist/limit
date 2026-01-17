#!/usr/bin/env bash
set -euo pipefail

if [[ ! -f build/limit-tests ]]; then
  echo "limit-tests not found. Run ./scripts/configure.sh and ./scripts/build.sh first." >&2
  exit 1
fi

valgrind \
  --tool=memcheck \
  --leak-check=full \
  --show-leak-kinds=definite,indirect,possible \
  --errors-for-leak-kinds=definite,indirect \
  --error-exitcode=1 \
  --gen-suppressions=all \
  --suppressions=./valgrind.supp \
  ./build/limit-tests
