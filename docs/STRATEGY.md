# Event + Microstructure HFT Strategy

这是基于此前讨论内容实现的可运行、可审计版本。它把策略思想具体化为确定性的特征、时间记忆、事件冲击、信号融合、进出场和风险规则。

## Pipeline

Market Data -> 10-level Book -> Microstructure Features -> Temporal Memory -> Impulse/Persistence -> Event Fusion -> Signal -> Risk/Position -> Paper Execution

## Features

- Top-5 depth imbalance: `(bid5-ask5)/(bid5+ask5)`
- Top-2 depth imbalance
- Full 10-level thickness imbalance
- Microprice edge
- Weighted pressure = 0.50 top5 + 0.30 top2 + 0.20 full-depth
- Book slope proxies
- Mid-price velocity
- Event score = relevance/novelty/sentiment combination
- Impulse score combines pressure, microprice edge and event score

## Temporal memory

Recent signed flow is exponentially decayed. Persistence measures how consistently historical flow agrees with the current direction. The implementation keeps a bounded five-second microstructure window and a configurable decay constant.

## Signal

`raw = 0.55*pressure + 0.20*tanh(velocity/2) + event_weight*(sentiment*relevance*novelty)`

The final directional score combines raw directional pressure, persistence and impulse. Entry requires score and impulse thresholds plus a spread guard.

## Position lifecycle

Entry -> optional same-direction add-on -> take-profit / stop-loss / timeout / signal reversal / weak-signal exit.

## Event engine

The current executable accepts an Event object. A production adapter can map X/Twitter/news/event feeds into relevance, novelty and sentiment. No credentials or live social-media integration are included.

## Important

This is an engineering implementation derived from the strategy description supplied in the conversation. It is not a reconstruction of unavailable proprietary source code and it has not been validated as profitable. Execution defaults to paper/simulation.
