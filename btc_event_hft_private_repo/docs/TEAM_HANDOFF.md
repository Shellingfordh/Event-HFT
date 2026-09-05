# Team Handoff

## Data
Own event adapters, normalization, timestamps, market-data adapters, and schema validation.

## Research
Own event taxonomy, private datasets, statistical validation, and regime analysis.

Do not commit proprietary labels, thresholds, or model weights.

## Execution
Own paper execution, order state machine, cancellation, latency measurement, and exchange adapter interfaces.

## Risk
Own daily loss cap, position cap, stale-signal rejection, kill switch, and consecutive-loss protection.

## Git workflow

```bash
git checkout -b feature/<name>
git add .
git commit -m "feat: <description>"
git push origin feature/<name>
```

Every PR should state what changed, what was tested, whether secrets are touched, whether alpha is exposed, and whether execution risk changed.

## Protected information

Keep outside this repository:

`PRIVATE_ALPHA.md`
`private/`
`models/`
`data/private/`
`.env`
production credentials
exact source ranking
exact event-to-entry timing
exact signal thresholds
exact position sizing
exact stop/target/timeout parameters

Give teammates interfaces and fixtures instead of the proprietary implementation.
