# Decision Clock

The decision clock is the bridge between event-driven research and fast execution.

## Rules

- one decision in flight per clock interval;
- fixed deadline for the hot path;
- if the deadline is missed, emit HOLD;
- stale EventSignals are ignored;
- model inference belongs off the hot path unless it is deterministic and bounded;
- receipt/fill processing is asynchronous.

This generalizes the bounded-loop idea demonstrated by `jev-trader`, whose public documentation describes a ~300 ms Monad block budget, two hot-path RPC calls, and asynchronous receipt/fill handling. V5 does not claim the same latency on Binance/CME; the actual budget must be measured per venue and adapter.
