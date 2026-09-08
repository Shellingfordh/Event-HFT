# v2 Overview

The original low-latency HFT design is merged with an event-driven BTC research layer.

`Event/Tweet -> normalize -> novelty/relevance -> 10-level book -> order-flow -> temporal memory -> impulse -> PRIVATE ALPHA -> risk -> paper execution -> fast exit`

C++17 owns hot-path contracts. SPSC, object pools and timestamp boundaries are provided. CPU affinity and core isolation are deployment concerns. The ten-level book exposes imbalance, microprice, spread, pressure and depth slope. The temporal/impulse interfaces expose data contracts without the proprietary model. Rust owns the parameter-engine boundary.

The repository does not contain exact alpha weights, source ranking, watchlists, thresholds, timing windows, model weights, production sizing or credentials.
