#!/usr/bin/env bash
set -euo pipefail

rg --files -g '*.sh' scripts | xargs -r shellcheck
