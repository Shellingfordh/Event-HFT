# BTC Event-Driven HFT v2 · Private Research Scaffold

`Event/Tweet -> Event Engine -> 10-level Microstructure -> Temporal/Impulse -> PRIVATE ALPHA -> Risk -> Paper Execution -> Fast Exit`

This v2 incorporates the original architecture ideas: C++17 hot path, SPSC, preallocation/object-pool pattern, TSC boundary, CPU-affinity hooks, ten-level depth, order-flow feature boundary, temporal memory, impulse prediction, Rust parameter-engine boundary, event intelligence and risk controls.

The actual alpha stays outside this repository. Exact weights, thresholds, timing windows, source ranking, model weights, credentials, production sizing and production endpoints are intentionally absent.

Execution is paper-only. Production exchange integration belongs behind a separately controlled adapter.

## Build

`./scripts/run_demo.sh`
