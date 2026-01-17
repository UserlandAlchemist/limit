#!/usr/bin/env bash
set -euo pipefail

rg --files -g '*.md' -g '!third_party/**' -g '!build/**' -g '!build-*/**' . | xargs -r mdl -c .mdlrc
