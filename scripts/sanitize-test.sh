#!/usr/bin/env bash
set -euo pipefail

cmake -S . -B build-sanitize -G Ninja -DCMAKE_BUILD_TYPE=Debug -DLIMIT_SANITIZE=ON
cmake --build build-sanitize

export ASAN_OPTIONS="detect_leaks=1:symbolize=1"

lsan_supp_path="$(pwd)/lsan.supp"
export LSAN_OPTIONS="suppressions=${lsan_supp_path}"

./build-sanitize/limit-tests
