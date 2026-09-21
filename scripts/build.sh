#!/usr/bin/env bash
set -euo pipefail
mkdir -p build
cmake -S . -B build
cmake --build build -j2
