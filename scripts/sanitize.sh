#!/usr/bin/env bash
set -euo pipefail

cmake -S . -B build-sanitize -G Ninja -DCMAKE_BUILD_TYPE=Debug -DLIMIT_SANITIZE=ON
cmake --build build-sanitize
