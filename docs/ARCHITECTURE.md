# Architecture

```text
Authorized X / event source
          ↓
   Event normalization
          ↓
 ┌────────┴─────────┐
 ↓                  ↓
Event features   Market snapshot
 └────────┬─────────┘
          ↓
     PRIVATE ALPHA
          ↓
       Risk Gate
          ↓
 Paper / controlled execution
          ↓
     Exit / reconcile
```

## Research hypothesis

Some public information events can produce short-lived repricing impulses in BTC.

The event is a candidate catalyst. The market response is the confirmation layer.

Useful public research features include event age, source metadata, text/cashtag extraction, top-of-book imbalance, spread, microprice, short-horizon return, trade intensity, and depth change.

The exact feature combination, weighting, timing window, entry/exit logic, and position sizing remain private.

## Latency accounting

Record:

`source_timestamp → receive_timestamp → feature_timestamp → signal_timestamp → order_submit_timestamp → exchange_ack_timestamp`

Never infer end-to-end latency from one component measurement.
