# Latency Measurement

Record `source -> receive -> decode -> feature -> signal -> risk -> submit -> ack -> fill -> exit` timestamps.

Report p50/p90/p99/worst-case. TSC must be calibrated and validated on the actual host; raw TSC values are not portable between machines. CPU affinity, isolation, preallocation and cache behavior should be benchmarked on the target system.
