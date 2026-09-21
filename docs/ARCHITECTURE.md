# V5 Architecture

## 1. Two information highways

### External event highway
`X -> Collector -> Event Intelligence -> EventSignal`

### Internal market highway
`Venue -> Market Data -> OrderBook -> Microstructure -> Temporal Memory`

They meet only at Signal Fusion.

## 2. Inspired engineering pattern

The public `jarrodwatts/jev-trader` project demonstrates a useful real-time pattern: one decision per Monad block, a bounded hot loop, one in-flight decision, and receipts/fills handled asynchronously. V5 adopts the engineering pattern without copying its source and without assuming Monad's 300 ms block cadence.

## 3. V5 difference

`jev-trader` is designed around Kuru MON-USDC and a model choosing buy/sell each Monad block, followed by post-only quoting. V5 is venue-neutral and event-driven: external events form a hypothesis, microstructure confirms/rejects it, and the execution policy may use marketable limits, aggressive orders, or later a venue-specific passive policy.

## 4. Hot path rule

The hot path may read already-normalized market state, read the latest EventSignal, calculate deterministic features, run signal fusion, run risk checks, and submit an order intent. It must not perform database writes, dashboard pushes, blocking HTTP calls, or slow model calls.

## 5. Slow path

X collection, NLP/LLM analysis, author statistics, persistence, receipts, fill accounting, dashboards and research reports live off the hot path.
