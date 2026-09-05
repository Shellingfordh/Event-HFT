# Risk Controls

Paper mode is the default.

Minimum hard controls:
- maximum open notional
- maximum daily loss
- maximum consecutive losses
- stale event rejection
- stale market-data rejection
- duplicate event rejection
- position reconciliation
- emergency kill switch
- explicit order acknowledgement handling

Public posts can be deleted, clarified, spoofed, misinterpreted, or already priced in. Never treat social-media text as verified truth.

Fail closed if market data is stale, exchange acknowledgement is missing, local position differs from exchange position, clock/timestamp health fails, or risk state cannot be reconstructed.
