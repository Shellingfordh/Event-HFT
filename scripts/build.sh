#!/usr/bin/env bash
set -euo pipefail
cmake -S . -B build
cmake --build build -j"$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 2)"
