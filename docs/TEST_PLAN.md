# Test Plan

## Unit

- order-book calculations
- OFI and imbalance
- event score
- signal fusion
- stale-event rejection
- deadline/late behavior
- risk gates

## Replay

Replay market and X data using the original arrival timestamps. Never let historical storage order substitute for arrival time.

## A/B

A: microstructure only
B: microstructure + temporal memory
C: event only
D: event + microstructure
E: event + microstructure + temporal memory

Compare signal quality, MFE/MAE, latency, holding time, slippage sensitivity and drawdown. Do not optimize only on headline win rate.

## Paper live

Run the exact production data path with simulated execution. Keep order lifecycle, latency and fill assumptions visible.

## Live gate

Live execution should require explicit configuration, venue-specific risk limits, kill switch, stale-data protection, position reconciliation and a paper-run acceptance report.
