# Event-Driven Microstructure Trading System V5

This version combines the project's original microstructure/event architecture with a **decision-clock / hot-path design inspired by the public `jarrodwatts/jev-trader` architecture**.

Reference: https://github.com/jarrodwatts/jev-trader

## What V5 adds

- X/Twitter event ingestion as a first-class event source.
- Event -> market-reaction confirmation -> short-horizon execution pipeline.
- Explicit decision clock with a hard decision deadline and `LATE/HOLD` state.
- One in-flight decision per clock tick to prevent overlapping decisions.
- Hot path separated from receipts, persistence, analytics and dashboards.
- Model adapter boundary: local heuristic now, optional external model later.
- Dry-run execution by default.
- Order intent / fill / position / PnL event model.
- Replay-oriented event schema and deterministic decision inputs.
- Venue-neutral execution interface for Binance/Rithmic/CME adapters.
- Dashboard/SSE-compatible telemetry contract without putting telemetry on the hot path.

## Core philosophy

The X event is **not** a direct BUY/SELL trigger. It creates an event hypothesis.
The microstructure engine tests whether the market is actually responding. Temporal memory measures persistence/decay. Risk decides whether execution is permitted. Execution handles the venue.

```text
X/Twitter -> Event Intelligence -> Event Alpha
                                      \
                                       -> Signal Fusion -> Risk -> Execution
Market Data -> Order Book -> Microstructure /
                               Temporal Memory
```

## Safety

The default mode is simulation. No credentials are included. Do not put API keys or private keys in the repository. Live execution adapters must be explicitly enabled outside the default configuration.

## Build

```bash
./scripts/build.sh
./build/hft_demo
```

## X collector

```bash
cp .env.example .env
export X_BEARER_TOKEN='...'
./scripts/run_x_event_engine.sh
```

The collector uses the official X API filtered stream. It does not use browser automation, stolen cookies, or access-control bypasses.

## Decision loop

The V5 decision loop is deliberately modeled after the useful engineering idea in `jev-trader`: a bounded decision window, one in-flight decision, late decisions become HOLD, and confirmation/fill accounting happens off the hot path. `jev-trader` uses a ~300 ms Monad block budget and keeps receipt/fill processing off the hot path. V5 generalizes that pattern to configurable market-data clocks rather than assuming Monad blocks.

## Important limitation

The Alpha formulas and coefficients in this repository are engineering baselines, not a claim that they reproduce a proprietary validated strategy. They must be evaluated with replay data before any live use.



# 本地测试与验证

> 目标：先把整条链路在本地跑通，再接真实 X 数据和真实交易账户。
>
> **默认测试模式不会下单。** 先使用本地 demo / replay / paper execution 验证。真实交易凭证不要写进仓库。

## 1. 环境要求

macOS / Linux 推荐：

- Git
- CMake >= 3.20
- C++17 编译器
- Python 3.10+
- Rust 1.80+（如果需要运行 parameter engine）
- `curl`
- `jq`（可选，用于查看 JSONL）

macOS 可以先：

```bash
brew install cmake jq
```

检查：

```bash
cmake --version
c++ --version
python3 --version
cargo --version
```

---

## 2. 解压并进入项目

```bash
unzip hft_event_strategy_complete_v5_jev_integrated.zip
cd hft_event_strategy_complete_v5_jev_integrated
```

如果目录名不同，进入实际项目目录即可：

```bash
ls
```

正常应该能看到类似：

```text
cpp/
services/
configs/
docs/
scripts/
rust/
CMakeLists.txt
README.md
.env.example
```

---

## 3. 第一次运行：只验证 C++ 核心链路

先编译：

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(sysctl -n hw.ncpu 2>/dev/null || nproc)
```

如果编译成功：

```bash
./build/hft_demo
```

Linux 如果没有 `nproc`：

```bash
cmake --build build -j4
```

这一阶段只检查：

```text
Market Data
    ↓
OrderBook
    ↓
Microstructure
    ↓
Temporal Memory
    ↓
Signal
    ↓
Risk
    ↓
Paper Execution
```

**这里不需要 API Key，也不会连接真实交易账户。**

---

# 4. 一键跑本地 Demo

如果项目提供了 demo script：

```bash
chmod +x scripts/*.sh
./scripts/run_demo.sh
```

如果没有执行权限：

```bash
bash scripts/run_demo.sh
```

预期看到：

```text
[market] ...
[feature] ...
[event] ...
[signal] ...
[risk] ...
[execution] PAPER ...
```

重点不是看某一次 BUY/SELL，而是确认每个模块的数据都能向下游传递。

---

# 5. 测试 X/Twitter Event Engine

X Event Engine 不建议一上来就连接真实 Stream。

先检查配置：

```bash
cat configs/x_rules.json
```

再复制：

```bash
cp .env.example .env
```

然后编辑：

```bash
nano .env
```

真实 X API token 只放在 `.env`，不要提交 Git：

```bash
git status
```

确认 `.env` 没有出现在待提交文件里。

---

## 5.1 无 X Token 的本地测试

先使用本地事件：

```bash
python3 services/x_event_engine/x_collector.py --demo
```

如果当前版本的 collector 没有 `--demo` 参数，则使用项目提供的：

```bash
bash scripts/run_x_event_engine.sh
```

然后观察 JSONL：

```bash
find . -name "*.jsonl" -type f
```

例如：

```bash
tail -f data/events/*.jsonl
```

---

## 5.2 测试事件处理链

一个事件应该经过：

```text
X Post
 ↓
received_ts_ns
 ↓
author
 ↓
text
 ↓
asset
 ↓
event_type
 ↓
novelty
 ↓
relevance
 ↓
credibility
 ↓
historical_impact
 ↓
expected_duration
 ↓
Event Alpha
```

检查 JSON：

```bash
jq . path/to/event.jsonl | head -100
```

最重要的是确认：

```text
received_ts_ns
```

存在。

**策略时间必须以系统收到事件的时间为准，而不是事后 API 返回的时间。**

否则 Replay 很容易产生未来数据泄漏。

---

# 6. Replay 测试

Replay 是整个项目最重要的本地测试方式之一。

不要一开始就接真实账户。

准备：

```text
market_data/
events/
trades/
orderbook/
```

然后：

```bash
python3 scripts/replay.py --input data/ --mode paper
```

如果项目中的 replay 脚本名称不同：

```bash
find scripts -maxdepth 2 -type f
```

找到 replay 入口后运行：

```bash
python3 <replay_script> --help
```

Replay 必须严格按照 timestamp 排序：

```text
event received_ts
market exchange_ts
market receive_ts
trade receive_ts
```

**禁止读取未来数据。**

例如：

```text
10:00:00.100  X event received
10:00:00.105  book update
10:00:00.108  trade
10:00:00.115  price impulse
```

策略在 `10:00:00.100` 时不能知道后面的三个事件。

---

# 7. 完整链路测试

推荐按照这个顺序测试：

```text
① Market Data
      ↓
② OrderBook
      ↓
③ Microstructure
      ↓
④ X Event
      ↓
⑤ Event Alpha
      ↓
⑥ Temporal Memory
      ↓
⑦ Signal Fusion
      ↓
⑧ Risk
      ↓
⑨ Paper Execution
      ↓
⑩ Metrics
```

不要一次性启动所有服务然后看结果。

应该逐层验证。

---

## 7.1 Market Data

检查：

```text
timestamp
symbol
bid
ask
bid_size
ask_size
depth[10]
trade_price
trade_size
trade_side
```

---

## 7.2 Microstructure

至少确认这些 feature 有输出：

```text
top5_imbalance
top2_trade_imbalance
ofi
buy_pressure
sell_pressure
depth_slope
spread
microprice
price_velocity
price_acceleration
liquidity_state
```

---

## 7.3 Event Alpha

检查：

```text
event_id
asset
direction
novelty
relevance
credibility
impact
expected_duration
received_ts_ns
```

---

## 7.4 Signal Fusion

最终应该得到类似：

```json
{
  "event_signal": "...",
  "market_signal": "...",
  "persistence": "...",
  "action": "BUY",
  "confidence": 0.0,
  "timestamp_ns": 0
}
```

这里的具体阈值属于策略研究参数，不要把 Demo 输出当成已经验证的 Alpha。

---

# 8. Decision Clock 测试

V5 引入了类似 JEV 的 bounded decision window。

测试重点：

```text
decision_start
       ↓
feature calculation
       ↓
signal
       ↓
risk
       ↓
order intent
       ↓
deadline
```

如果超过 deadline：

```text
late = true
```

策略应该：

```text
HOLD
```

而不是继续发送一个已经过期的订单。

JEV 的公开实现也是每个 Monad block 进行一次决策，并在决策过期时放弃该周期；我们吸收的是这个 bounded-decision 思路，而不是直接复制它的 300ms 参数。citeturn0view0

---

# 9. One-in-flight 测试

测试：

```text
Decision A
    ↓
in_flight = true

Decision B arrives
    ↓
如果 A 尚未结束
    ↓
B 不得重复发送执行请求
```

重点检查：

```text
duplicate order
stale signal
race condition
```

---

# 10. Paper Execution

本地开发默认：

```text
PAPER = true
```

或者使用项目配置中的 paper execution 开关。

检查日志：

```bash
grep -R "PAPER\|SIM\|DRY" logs/ data/ 2>/dev/null
```

你应该看到：

```text
PAPER BUY
PAPER SELL
PAPER HOLD
```

而不是：

```text
LIVE ORDER
```

---

# 11. 不要直接接真实资金

真实交易之前至少完成：

```text
[ ] C++ 编译
[ ] Unit Test
[ ] Market Replay
[ ] Event Replay
[ ] No-lookahead 检查
[ ] Signal Replay
[ ] Paper Execution
[ ] Risk Limit
[ ] Order Timeout
[ ] Duplicate Order Test
[ ] Disconnect Test
[ ] Stale Data Test
[ ] Position Reconciliation
```

然后才考虑测试网或极小规模真实账户。

---

# 12. 性能测试

JEV 项目公开 README 中也提供了 book-read 和 calldata encoding benchmark。我们的测试同样应该把数据读取、Signal、Execution 分开测，而不是只看整个程序平均耗时。citeturn0view0

运行：

```bash
find . -iname "*bench*" -o -iname "*benchmark*"
```

如果存在 benchmark：

```bash
./build/<benchmark_name>
```

建议记录：

```text
market_data_us
feature_us
event_us
fusion_us
risk_us
execution_us
end_to_end_us
```

重点看：

```text
p50
p95
p99
max
```

不要只看平均值。

---

# 13. 推荐的本地开发顺序

新同学第一次拿到代码，严格按下面执行：

```bash
# 1. 编译
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4

# 2. Demo
./build/hft_demo

# 3. 单元测试
ctest --test-dir build --output-on-failure

# 4. Event Engine
bash scripts/run_x_event_engine.sh

# 5. Replay
python3 scripts/replay.py --help

# 6. Paper
# 使用 paper 配置运行完整链路
```

然后再开始改代码。

---

# 14. 团队开发时每个人先测试自己的模块

### Market Data

负责：

```text
Market Data → OrderBook
```

验收：

```text
timestamp 正确
盘口顺序正确
断线可以恢复
不会重复数据
```

### X Event

负责：

```text
X → Event
```

验收：

```text
received_ts 正确
事件不会重复
规则过滤正确
JSONL 可 Replay
```

### Feature

负责：

```text
OrderBook/Trade → Feature
```

验收：

```text
OFI
Imbalance
Pressure
Velocity
Liquidity
```

### Strategy

负责：

```text
Event + Feature → Signal
```

验收：

```text
没有未来数据
Signal timestamp 正确
late signal 会 HOLD
```

### Execution

负责：

```text
Signal → Order Intent
```

验收：

```text
不会重复下单
不会发送 stale order
risk cap 生效
paper/live 状态明确
```

---

# 15. 最终验收标准

本项目不是：

```text
./start.sh
看到 BUY
```

就算成功。

真正的本地测试完成标准是：

```text
                ┌── X Event ───────┐
                │                  │
Market Data ────┼── Microstructure ┤
                │                  │
                └── Temporal ──────┘
                         ↓
                   Signal Fusion
                         ↓
                       Risk
                         ↓
                  Decision Clock
                         ↓
                  Paper Execution
                         ↓
                    Trade Log
                         ↓
                      Metrics
```

整个链路可以：

1. 录制
2. Replay
3. 重现
4. 对比
5. 修改参数
6. 再 Replay
7. 输出 metrics

才算进入真正的策略研发阶段。

---

## 最重要的一条

**先 Replay，再 Paper，最后才 Live。**

尤其是 X Event + 微观结构策略，最容易出现：

```text
未来数据泄漏
事件时间错位
盘口时间错位
重复事件
stale signal
执行延迟造成的虚假 Alpha
```

所以任何“这个参数很好”“这个信号很准”的结论，都必须能在 Replay 中重新跑出来。
