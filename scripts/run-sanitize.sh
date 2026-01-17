#!/usr/bin/env bash
set -euo pipefail

export ASAN_OPTIONS="detect_leaks=1"
export LSAN_OPTIONS="suppressions=$(pwd)/lsan.supp"

./build-sanitize/Limit_artefacts/Debug/Limit
