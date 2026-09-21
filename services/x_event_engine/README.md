# X Event Engine

This service is intentionally a source adapter. It records raw public X API stream events and keeps them outside the trading hot path.

Pipeline:

`X Filtered Stream -> raw recorder -> normalizer -> event intelligence -> EventSignal -> C++ decision loop`

Do not call an LLM synchronously from the order hot path. If an external model is used, run it asynchronously and attach the resulting event hypothesis to the next decision window. The market-data confirmation layer remains deterministic.
