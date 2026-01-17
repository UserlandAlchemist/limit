#!/usr/bin/env bash
set -euo pipefail

rg --files -g '*.cpp' -g '*.hpp' src tests | xargs -r clang-format -i
