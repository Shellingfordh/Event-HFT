# Automatic Parameter Engine

Observed issue: crypto parameters can become invalid as the market regime changes.

Public architecture:

`live features -> regime state -> candidate parameters -> constraints -> shadow evaluation -> stability gate -> promote/reject`

The optimization objective, search space, regime classifier and promotion thresholds remain private. Positive recent PnL alone must not promote a candidate.
