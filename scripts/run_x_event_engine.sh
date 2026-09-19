#!/usr/bin/env bash
set -euo pipefail
: "${X_BEARER_TOKEN:?Set X_BEARER_TOKEN first}"
python3 services/x_event_engine/x_collector.py sync-rules
exec python3 services/x_event_engine/x_collector.py stream
