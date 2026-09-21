# Decision Gateway

Contract between slow/event intelligence and the low-latency C++ strategy.

Recommended production transport:

- shared memory ring buffer or Unix-domain datagram for same-host deployment;
- protobuf/flatbuffers or a fixed binary struct;
- JSON only for debugging/replay.

Message fields:
`event_id, received_ts_ns, asset, direction, relevance, novelty, credibility, impact, expected_duration_ms`.

The gateway must reject stale events and never block the market-data thread waiting for event analysis.
