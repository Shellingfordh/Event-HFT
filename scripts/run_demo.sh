#!/usr/bin/env bash
set -euo pipefail
R=$(cd "$(dirname "$0")/..";pwd)
cmake -S "$R/cpp" -B "$R/build"
cmake --build "$R/build" -j
ctest --test-dir "$R/build" --output-on-failure
(cd "$R/rust/parameter_engine" && cargo test)
