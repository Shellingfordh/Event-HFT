# New Member Onboarding

Read in this order:

1. README.md
2. ARCHITECTURE.md
3. STRATEGY.md
4. X_EVENT_ENGINE.md
5. DECISION_CLOCK.md
6. TEST_PLAN.md
7. JEV_REFERENCE.md

First sprint:

- compile the demo;
- inspect the EventSignal and MarketSnapshot schemas;
- build a recorder/replay fixture;
- add tests for stale events and late decisions;
- add one microstructure feature and demonstrate incremental value on replay;
- never change live execution settings during research.

Project boundary:

The Alpha research owner controls the final signal fusion and confidential coefficients. Infrastructure contributions should expose clean interfaces and tests rather than hard-code strategy assumptions.
