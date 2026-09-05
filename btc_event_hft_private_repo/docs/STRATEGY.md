# Strategy Brief · Team Version

## Working thesis

We are researching an event-driven, short-horizon BTC strategy:

`public event → abnormal market response → short-lived directional trade → fast exit`

Candidate event classes can include major protocol announcements, exchange incidents, ETF/regulatory headlines, macro announcements, security incidents, or statements from influential market participants.

A keyword match is not a trading signal.

The system should distinguish ordinary chatter, recycled news, low-credibility posts, already-priced information, genuinely new information, and events followed by measurable order-flow response.

## Signal lifecycle

```text
EVENT
  ↓
NORMALIZE
  ↓
RELEVANCE
  ↓
NOVELTY
  ↓
SOURCE QUALITY
  ↓
MARKET RESPONSE
  ↓
PRIVATE ALPHA SCORE
  ↓
RISK GATE
  ↓
SHORT-LIVED POSITION
  ↓
EXIT
```

## Long / short

Positive estimated near-term repricing pressure may produce LONG.

Negative estimated near-term repricing pressure may produce SHORT.

Weak or conflicting evidence produces FLAT.

The exact thresholds, timing window, leverage, sizing, and exit parameters are intentionally omitted.

## Validation

Track net PnL after fees, average edge, slippage, adverse selection, fill ratio, signal-to-order latency, event-to-price latency, drawdown, tail losses, profit factor, turnover, event-category performance, and regime performance.

Do not optimize solely for win rate.

## Anti-overfitting

Every new parameter needs a hypothesis, training period, validation period, untouched forward period, and paper/live-forward test.

If a parameter only works for a few hours of crypto data, treat that as regime sensitivity until proven otherwise.
