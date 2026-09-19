# Event HFT Complete Strategy Implementation

## What this repository contains

A concrete implementation of the strategy architecture discussed previously: 10-level microstructure features, order-flow pressure, temporal memory, impulse/persistence scoring, event-driven signal fusion, add-on logic, exits, adaptive parameter example, and paper execution.

## Build

```bash
cmake -S . -B build
cmake --build build -j
./build/event_hft
```

Rust parameter engine:

```bash
cd rust/parameter_engine
cargo run
```

## Safety

Default mode is simulation. No exchange keys, social-media credentials, or live-order endpoints are present. Before any live deployment, add venue-specific adapters, sequence-gap handling, rejects/cancel-replace handling, kill switches, clock synchronization, persistent audit logs, and extensive replay tests.

## Strategy completeness

The repository contains executable formulas and state transitions rather than empty interfaces. Parameters are explicit in `configs/default.toml`. Where the original discussion did not provide a proprietary mathematical formula, this repository uses a documented engineering approximation and labels it accordingly.

## X / Twitter event ingestion

The event lane is now included as a runnable service using the official X API v2 Filtered Stream.

```bash
export X_BEARER_TOKEN='YOUR_TOKEN'
./scripts/run_x_event_engine.sh
```

See `docs/X_EVENT_ENGINE.md` and `services/x_event_engine/README.md`. Raw posts are retained for replay and normalized events are written to `data/x_events.jsonl`. No credentials are included.
