# X Event Engine

This service is the social-event ingestion side of the HFT system. It uses the official X API v2 Filtered Stream rather than browser scraping.

Pipeline:

`X Filtered Stream -> raw JSONL -> normalized Event -> Event Alpha -> C++ signal fusion`

## Setup

```bash
export X_BEARER_TOKEN='YOUR_TOKEN'
python3 services/x_event_engine/x_collector.py sync-rules
python3 services/x_event_engine/x_collector.py stream
```

No token is stored in the repository. `.env` is ignored by git.

## Output

- `data/x_raw.jsonl`: original API payloads for replay/audit.
- `data/x_events.jsonl`: normalized events with author metadata and event features.

Each normalized event contains `asset`, `direction`, `sentiment`, `relevance`, `novelty`, `credibility`, and `impact`.

## Production notes

The collector reconnects with exponential backoff and requests up to five minutes of backfill after reconnect. The hot trading path should consume normalized events from an IPC/lock-free adapter; JSONL is intentionally the audit/replay sink and development bridge.
