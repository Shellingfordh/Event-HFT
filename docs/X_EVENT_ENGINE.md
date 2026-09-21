# X Event Engine

## Purpose

Turn public X events into time-stamped, asset-aware event hypotheses. It is not a direct trade trigger.

## Stages

1. Filtered Stream ingestion
2. Raw event persistence
3. Author enrichment
4. Asset/entity detection
5. Event type classification
6. Novelty detection
7. Relevance scoring
8. Direction classification
9. Impact and expected duration estimation
10. Emit `EventSignal`

## Latency discipline

The collector can be asynchronous. The decision engine must use the actual `received_ts_ns` as the earliest time at which the event becomes available to the strategy. `created_at` is informational and cannot be used to move the event backwards in time.

## Event-to-market research

For each event, measure price and microstructure response over 100 us, 500 us, 1 ms, 5 ms, 20 ms, 100 ms, 1 s, 5 s and 30 s where the venue's data quality supports it.

Store MFE, MAE, signed return, trade intensity, spread change and OFI response. Group by event type, author, asset and market regime.
