# Event-Driven Microstructure Trading System V5

This version combines the project's original microstructure/event architecture with a **decision-clock / hot-path design inspired by the public `jarrodwatts/jev-trader` architecture**.

Reference: https://github.com/jarrodwatts/jev-trader

## What V5 adds

- X/Twitter event ingestion as a first-class event source.
- Event -> market-reaction confirmation -> short-horizon execution pipeline.
- Explicit decision clock with a hard decision deadline and `LATE/HOLD` state.
- One in-flight decision per clock tick to prevent overlapping decisions.
- Hot path separated from receipts, persistence, analytics and dashboards.
- Model adapter boundary: local heuristic now, optional external model later.
- Dry-run execution by default.
- Order intent / fill / position / PnL event model.
- Replay-oriented event schema and deterministic decision inputs.
- Venue-neutral execution interface for Binance/Rithmic/CME adapters.
- Dashboard/SSE-compatible telemetry contract without putting telemetry on the hot path.

## Core philosophy

The X event is **not** a direct BUY/SELL trigger. It creates an event hypothesis.
The microstructure engine tests whether the market is actually responding. Temporal memory measures persistence/decay. Risk decides whether execution is permitted. Execution handles the venue.

```text
X/Twitter -> Event Intelligence -> Event Alpha
                                      \
                                       -> Signal Fusion -> Risk -> Execution
Market Data -> Order Book -> Microstructure /
                               Temporal Memory
```

## Safety

The default mode is simulation. No credentials are included. Do not put API keys or private keys in the repository. Live execution adapters must be explicitly enabled outside the default configuration.

## Build

```bash
./scripts/build.sh
./build/hft_demo
```

## X collector

```bash
cp .env.example .env
export X_BEARER_TOKEN='...'
./scripts/run_x_event_engine.sh
```

The collector uses the official X API filtered stream. It does not use browser automation, stolen cookies, or access-control bypasses.

## Decision loop

The V5 decision loop is deliberately modeled after the useful engineering idea in `jev-trader`: a bounded decision window, one in-flight decision, late decisions become HOLD, and confirmation/fill accounting happens off the hot path. `jev-trader` uses a ~300 ms Monad block budget and keeps receipt/fill processing off the hot path. V5 generalizes that pattern to configurable market-data clocks rather than assuming Monad blocks.

## Important limitation

The Alpha formulas and coefficients in this repository are engineering baselines, not a claim that they reproduce a proprietary validated strategy. They must be evaluated with replay data before any live use.
