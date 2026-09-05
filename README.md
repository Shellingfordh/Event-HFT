# BTC Event-Driven HFT · Private Research Scaffold

> Internal team repository. This repository intentionally does not contain proprietary alpha logic, thresholds, source ranking, model weights, API credentials, or production execution parameters.

## Goal

Build an event-driven crypto research/execution pipeline:

`X/Tweet events → event normalization → market-impact features → signal score → risk gate → short-lived position → fast exit`

The public repository contains infrastructure and interfaces. The proprietary alpha implementation stays in a private package/repository.

## Architecture

```text
Authorized Event API
        ↓
Event Normalization
        ↓
Event Features + Market Snapshot
        ↓
PRIVATE ALPHA
        ↓
Risk Gate
        ↓
Paper / Controlled Execution
        ↓
Fast Exit
```

## Never commit

- API keys or secrets
- private watchlists
- exact alpha weights
- exact entry/exit thresholds
- model weights
- production endpoints
- production position limits
- private datasets

## Development order

1. Implement event normalization.
2. Connect an authorized/test event source.
3. Implement market-data adapter.
4. Run paper mode.
5. Record event-to-order timestamps and execution quality.
6. Validate signal quality on a private dataset.
7. Connect controlled execution only after risk checks pass.
