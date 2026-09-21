# Strategy Specification V5

## Thesis

Capture short-horizon price reactions to information events, but require observable market confirmation before execution.

## Event Alpha

Inputs:
- author identity / historical influence
- novelty
- relevance to asset
- event type
- direction
- credibility
- expected impact
- expected duration

Output: directional event hypothesis in [-1, 1] plus confidence/age metadata.

## Microstructure Alpha

Inputs:
- 10-level depth
- trade flow
- OFI
- imbalance
- microprice displacement
- book slope
- price velocity / acceleration
- liquidity state

Output: directional market-response score and persistence.

## Confirmation

Long requires event direction = long and microstructure direction = long. Short is symmetric. A high-impact event with no confirming flow is a WAIT state.

## Exit

Exit logic must be implemented as a separate state machine and evaluated independently. Candidate exit reasons:
- signal decay
- event expiration
- microstructure reversal
- liquidity collapse
- max holding time
- risk limit

## Research requirement

Every feature must show incremental value on the same replay dataset. Avoid tuning dozens of thresholds simultaneously. Use out-of-sample periods and preserve event arrival timestamps to avoid look-ahead bias.
