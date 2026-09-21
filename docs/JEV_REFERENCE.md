# Reference Analysis: jarrodwatts/jev-trader

Public reference: https://github.com/jarrodwatts/jev-trader

Useful ideas incorporated at the architectural level:

1. **Decision clock**: one bounded decision opportunity per market clock.
2. **One in-flight decision**: prevents overlapping decisions from racing the same book state.
3. **Late -> HOLD**: when a decision misses its opportunity, do not blindly submit a stale order.
4. **Hot/cold path split**: order submission is kept minimal; receipts, fills, PnL and telemetry are asynchronous.
5. **Dry run**: real market data can be used while execution is simulated.
6. **Explicit order intent and fill events**: intent, placement, receipt and fill are distinct lifecycle states.
7. **Latency instrumentation**: measure book read, decision latency and complete loop latency.
8. **Venue adapter boundary**: market/book reading and execution are isolated from the model.

What we do not copy conceptually:

- Kuru-specific contract encoding
- Monad-specific block assumptions
- MON-USDC quoting policy
- the Jev prompt/model implementation
- its exact source code

Our strategy remains event-driven: X events propose a hypothesis, market microstructure confirms/rejects it, and execution uses a venue-neutral interface.
