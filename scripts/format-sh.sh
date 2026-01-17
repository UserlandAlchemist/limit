#!/usr/bin/env bash
set -euo pipefail

rg --files -g '*.sh' scripts | xargs -r shfmt -w -i 2 -ci
