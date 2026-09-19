# Architecture

C++17 owns the latency-sensitive path. Rust owns adaptive parameter logic. Keep the hot path allocation-free after startup.

Suggested production threads:

1. feed-ingress
2. book-builder
3. feature/alpha
4. risk/order-manager
5. execution gateway
6. event-ingress
7. telemetry/logger

For CME migration, replace the simulated feed/execution adapters with Rithmic Diamond or MDP 3.0 adapters. Keep strategy and risk interfaces unchanged.
