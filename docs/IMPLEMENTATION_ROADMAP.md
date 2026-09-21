# Implementation Roadmap

## P0: data integrity
- synchronized market/event timestamps
- raw recorder
- deterministic replay
- stale-event and late-decision tests

## P1: strategy research
- OFI and add/cancel flow
- liquidity regimes
- event reaction latency
- author historical impact
- event duration
- temporal memory

## P2: execution realism
- venue-specific order adapters
- queue/slippage model
- cancel/replace lifecycle
- fill reconciliation
- kill switch

## P3: production
- CPU pinning
- lock-free IPC
- latency histogram
- operational dashboard
- paper acceptance gate
- explicit live enablement
