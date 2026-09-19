# X / Twitter Event Engine

## Role in the strategy

X is an independent event-data lane. It does not directly place orders.

```text
X Filtered Stream
        |
        v
X Collector
        |
        +--> raw audit log
        |
        v
Event Normalizer
        |
        +--> author intelligence
        +--> asset detection
        +--> novelty / relevance
        +--> direction / sentiment
        +--> impact
        |
        v
Event Alpha
        |
        +--------------------+
        |                    |
        v                    v
Microstructure Alpha   Temporal Memory
        |                    |
        +---------+----------+
                  v
             Signal Fusion
                  |
                Risk
                  |
              Execution
```

## Official API integration

The implementation uses X API v2 Filtered Stream. A bearer token is supplied through `X_BEARER_TOKEN`. The stream accepts persistent connections and active filter rules. Reconnect handling uses the API's supported `backfill_minutes` parameter, capped here at five minutes.

## Rule layer

`configs/x_rules.json` defines the first-pass event universe. Keep rules broad enough to capture candidate events, then make the event classifier stricter. Do not put trading decisions directly into X rules.

## Author intelligence

The normalizer preserves author ID, username, verification and public metrics so a later research process can maintain a persistent `AuthorProfile`. Recommended fields:

- historical_event_count
- event_to_price_reaction
- median_reaction_latency
- direction_hit_rate
- asset_affinity
- credibility_score

These should be estimated from replay data, not hard-coded as facts.

## Event scoring

The included scorer is an engineering baseline:

`impact = 0.45 * relevance + 0.25 * novelty + 0.30 * credibility`

Sentiment/direction is rule-based for deterministic development. Replace it with the team's validated classifier when available.

## Latency design

JSONL is for audit/replay and development. In production, bridge the normalized event into the C++ event-ingress queue without routing the trading hot path through a database or synchronous HTTP call.

## Operational safeguards

- Never commit API credentials.
- Deduplicate by event ID.
- Preserve source timestamps and receive timestamps.
- Detect stream disconnects and reconnect.
- Keep raw payloads for replay.
- Record rule IDs that caused each event to match.
- Start with paper execution.
- Do not infer profitability from the event score alone.
