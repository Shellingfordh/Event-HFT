# Event Engine

Normalize authorized public events into `event_id`, source, source timestamp, receive timestamp, text and metadata.

Candidate dimensions: source quality, category, novelty, duplication, recency, immediate price response and liquidity response.

Adversarial cases include deleted/edited posts, quote tweets, bot amplification, old news resurfacing and ambiguous language. Event detection creates a candidate; private alpha decides tradability.
