
# Event-Driven HFT 项目开发入组文档

> **内部开发文档 / Developer Onboarding**
>
> 本文只说明：**我们要做什么、系统最终长什么样、开发负责什么、接口怎么接、怎么测试、什么算完成。**
>
> **策略逻辑、Alpha 计算方式、具体参数和信号形成机制不属于本文内容。**

---

# 1. 项目要做成什么

我们要开发一套**事件驱动的自动交易基础设施**。

第一阶段以 Crypto 为主要开发和测试环境，后续需要能够扩展到其他市场和交易接口。

开发完成后，系统应该能够完成：

```text
接收市场数据
    ↓
维护实时市场状态
    ↓
接收外部事件
    ↓
生成标准化事件
    ↓
调用策略模块
    ↓
获得标准化交易信号
    ↓
风险检查
    ↓
生成订单意图
    ↓
执行订单
    ↓
获取成交结果
    ↓
维护仓位
    ↓
退出
    ↓
记录完整交易数据
    ↓
Replay / Evaluation
```

开发人员的目标不是研究交易策略。

开发人员的目标是：

> **把这条链路做成稳定、低延迟、可测试、可替换、可回放的交易系统。**

---

# 2. 开发人员需要知道的边界

整个项目分成两部分。

## Strategy / Alpha

属于策略研究侧。

包括：

* Alpha
* 信号生成逻辑
* 参数
* 阈值
* 特征组合
* 信号过滤
* 仓位模型
* 特定策略判断

这些模块对基础设施开发人员来说属于：

> **Black Box**

开发只需要知道：

```text
输入什么
↓
输出什么
↓
什么时候调用
↓
最大允许耗时多少
```

不需要知道：

```text
为什么这么计算
参数为什么是这个值
信号为什么成立
内部具体公式
```

---

# 3. 开发人员真正需要理解的系统

整体架构：

```text
                    ┌─────────────────┐
                    │   X / Events    │
                    └────────┬────────┘
                             ↓
                    ┌─────────────────┐
                    │ Event Collector │
                    └────────┬────────┘
                             ↓
                    ┌─────────────────┐
                    │ Event Pipeline  │
                    └────────┬────────┘
                             │
                             ↓
┌──────────────┐     ┌─────────────────┐
│ Market Data  │ ──→ │  Market State   │
└──────────────┘     └────────┬────────┘
                              │
                              ↓
                     ┌─────────────────┐
                     │ Strategy Layer  │
                     │   Black Box     │
                     └────────┬────────┘
                              ↓
                     ┌─────────────────┐
                     │   Risk Engine   │
                     └────────┬────────┘
                              ↓
                     ┌─────────────────┐
                     │ Execution Layer │
                     └────────┬────────┘
                              ↓
                     ┌─────────────────┐
                     │ Exchange / API  │
                     └─────────────────┘
                              ↓
                     ┌─────────────────┐
                     │ Fill / Position │
                     └────────┬────────┘
                              ↓
                     ┌─────────────────┐
                     │ Recorder / PnL  │
                     └────────┬────────┘
                              ↓
                     ┌─────────────────┐
                     │ Replay / Test   │
                     └─────────────────┘
```

---

# 4. 开发工作的核心原则

开发阶段最重要的是以下几个原则。

### 4.1 模块解耦

不要把：

```text
行情
策略
风险
交易所
日志
```

全部写在一起。

应该做到：

```text
MarketData
Strategy
Risk
Execution
Recorder
Replay
```

可以独立测试。

---

### 4.2 Strategy 不直接操作交易所

Strategy 输出：

```text
Signal
```

或者：

```text
OrderIntent
```

不能直接调用：

```text
Binance API
Rithmic API
HTTP
WebSocket
```

交易所操作统一交给 Execution。

---

### 4.3 Execution 不负责策略判断

Execution 只关心：

```text
订单是什么
数量是多少
价格是什么
什么时候发送
交易所返回什么
有没有成交
```

不负责判断：

```text
现在应该不应该买
```

---

### 4.4 所有东西必须可 Replay

线上出现一个问题之后，必须能够回答：

> 当时系统到底收到了什么？

所以所有重要输入和输出必须带：

```text
timestamp
sequence
source
event id
```

并能够重新播放。

---

# 5. Market Data 模块

开发目标：

> 建立统一的市场数据接口。

需要支持：

```text
Order Book
Trade
Ticker
Best Bid
Best Ask
Timestamp
Sequence
```

第一阶段重点：

```text
10-level Order Book
+
Trades
```

---

## Market Data 必须解决的问题

### 数据顺序

需要检测：

```text
sequence gap
duplicate
out-of-order
```

---

### 数据时间

统一使用：

```text
timestamp_ns
```

至少区分：

```text
exchange timestamp
receive timestamp
local processing timestamp
```

不要混为一个字段。

---

### 数据异常

必须能够检测：

```text
断流
重复
序号跳跃
异常价格
异常数量
连接重建
```

发生异常时不能继续假装数据正常。

---

# 6. Order Book

需要维护实时 Order Book。

基本要求：

```text
Bid 1 ~ Bid 10
Ask 1 ~ Ask 10
```

至少能够快速获得：

```text
best_bid
best_ask
mid_price
spread
bid_depth
ask_depth
```

接口应该尽可能简单。

例如：

```cpp
const OrderBookSnapshot& snapshot();
```

或者等价接口。

---

# 7. Event Engine

Event Engine 负责接收外部事件。

第一阶段：

```text
X / Twitter
```

以后可能加入：

```text
News
Announcement
On-chain
Exchange Event
```

Event Engine 不负责判断：

> 这个事件到底值不值得交易。

它只负责：

> **可靠地把外部信息转换成标准 Event。**

---

# 8. Event 标准格式

统一 Event Schema。

至少包含：

```text
event_id
source
received_ts_ns
created_at
author_id
text
asset
event_type
```

策略需要的其他字段可以由上层增加。

重点：

> Event Engine 只负责数据完整性和标准化。

---

# 9. Strategy Interface

Strategy 对开发人员应该是黑盒。

统一接口类似：

```cpp
Signal process(
    const MarketState& market,
    const EventState& events,
    const StrategyContext& context
);
```

返回：

```text
HOLD
LONG
SHORT
EXIT
```

或者标准化 Signal。

Signal 至少需要：

```text
timestamp
direction
confidence
valid_until
strategy_id
signal_id
```

---

# 10. 不要在 Strategy 外部复制策略逻辑

例如不要自己写：

```cpp
if (xxx > xxx)
    buy();
```

也不要在 Risk：

```cpp
if (某个指标 > 某阈值)
```

更不要在 Execution：

```cpp
if (市场怎么样)
```

这些都属于策略层。

开发侧只负责：

```text
输入
→
调用
→
输出
```

---

# 11. Risk Engine

Risk 是 Strategy 和 Execution 中间的一层。

输入：

```text
Signal
Current Position
Account State
Market State
Risk State
```

输出：

```text
ALLOW
REJECT
REDUCE
EXIT
```

Risk 至少需要检查：

```text
最大仓位
最大订单数量
最大单笔数量
最大持仓
最大亏损
最大滑点
交易状态
数据状态
连接状态
```

---

# 12. Execution Engine

Execution 接收：

```text
OrderIntent
```

负责：

```text
订单生成
订单发送
订单状态
成交
撤单
超时
重试
```

建议统一生命周期：

```text
CREATED
   ↓
SUBMITTED
   ↓
ACKED
   ↓
PARTIAL_FILLED
   ↓
FILLED
```

异常：

```text
REJECTED
CANCELLED
EXPIRED
FAILED
```

---

# 13. Position Manager

Position Manager 负责维护：

```text
position
average_entry
quantity
realized_pnl
unrealized_pnl
entry_time
last_update
```

不要让 Strategy 自己维护真实仓位。

Strategy 可以看到：

```text
PositionSnapshot
```

但 Position 的最终状态由 Position Manager 管。

---

# 14. Recorder

所有线上交易数据必须能够记录。

至少记录：

### Market

```text
timestamp
symbol
book
trade
sequence
```

### Event

```text
event_id
timestamp
source
payload
```

### Signal

```text
signal_id
timestamp
direction
confidence
```

### Order

```text
order_id
timestamp
side
price
quantity
status
```

### Fill

```text
fill_id
timestamp
price
quantity
fee
```

### Position

```text
timestamp
position
PnL
```

---

# 15. Replay Engine

Replay 是开发验收的重点之一。

要求：

```text
原始数据
↓
Replay
↓
重新进入系统
↓
产生 Signal
↓
Paper Execution
↓
生成结果
```

Replay 必须尽量保证：

```text
相同输入
+
相同版本
+
相同配置
=
可重复结果
```

---

# 16. Replay 不能作弊

Replay 最重要的要求：

> **不能让策略看到未来数据。**

例如：

```text
10:00:00.100
```

执行策略时只能看到：

```text
<= 10:00:00.100
```

的数据。

不能使用：

```text
10:00:00.101
```

的信息。

所有 Replay 数据必须严格按照时间顺序进入系统。

---

# 17. Paper Trading

正式连接交易所之前必须有 Paper Execution。

结构：

```text
Strategy
 ↓
Risk
 ↓
Paper Execution
 ↓
模拟成交
 ↓
Position
 ↓
PnL
```

Paper Execution 必须尽量模拟真实执行过程，包括：

```text
订单延迟
部分成交
滑点
撤单
订单过期
手续费
```

具体参数由测试配置决定。

---

# 18. Performance 要求

这是一个低延迟系统，所以开发时需要关注：

```text
allocation
lock
copy
syscall
I/O
serialization
network
```

尤其是 Hot Path。

尽量避免：

```cpp
new
delete
malloc
free
mutex
sleep
filesystem I/O
```

出现在关键交易路径。

并不是说这些 API 永远不能使用。

而是：

> **需要明确它们位于 Hot Path 还是 Cold Path。**

---

# 19. Hot Path

Hot Path 大致：

```text
Market Data
    ↓
Market State
    ↓
Strategy
    ↓
Risk
    ↓
Order Intent
    ↓
Execution
```

这里优先考虑：

```text
低延迟
稳定延迟
少分配
少锁
少拷贝
```

---

# 20. Cold Path

Cold Path：

```text
日志
数据库
统计
Dashboard
PnL分析
Receipt查询
报告
数据导出
```

这些任务不能反过来阻塞 Hot Path。

例如不要：

```text
收到行情
↓
写数据库
↓
等待数据库返回
↓
继续策略
```

应该：

```text
收到行情
↓
Hot Path继续
↓
异步记录
```

---

# 21. Decision Clock

系统需要一个统一的 Decision Clock。

主要负责：

```text
Signal timestamp
Decision deadline
Signal expiry
one_in_flight
```

如果一个 Signal 已经过期：

```text
DROP / HOLD
```

不能继续执行旧信号。

---

# 22. 测试分级

所有模块按照以下顺序测试。

## Level 1：Unit Test

测试：

```text
OrderBook
Event Parser
Timestamp
Sequence
Risk
Position
Order State
```

---

## Level 2：Integration Test

测试：

```text
Market Data
↓
Order Book
↓
Strategy Mock
↓
Risk
↓
Execution Mock
```

确保整个链路能够跑通。

---

## Level 3：Replay Test

使用真实历史数据：

```text
Market Data
+
Historical Event
```

进行完整回放。

---

## Level 4：Paper Trading

实时数据：

```text
Live Market
+
Live Event
+
Paper Execution
```

观察系统稳定性。

---

## Level 5：Live

只有前面的测试全部通过以后才进入。

Live 初期必须使用受控规模和风险限制。

---

# 23. 开发人员怎么判断自己写完了

一个模块不能因为：

```text
代码能编译
```

就算完成。

至少需要：

```text
代码
+
Unit Test
+
日志
+
异常处理
+
接口文档
+
Replay / Integration Test
```

---

# 24. Pull Request 要求

每个 PR 写清楚：

```text
What
为什么改

How
怎么实现

Test
怎么测试

Impact
影响哪些模块
```

例如：

```text
## What
新增 Order Book sequence gap 检测。

## How
在 Market Data 层维护 last_sequence。

## Test
测试：
1. 正常连续 sequence
2. sequence gap
3. duplicate
4. reconnect

## Impact
Market Data / Recorder
```

不要提交：

```text
fix bug
update
change
```

这种无法判断影响范围的描述。

---

# 25. Git 工作方式

不要直接长期修改主分支。

建议：

```text
main
 │
 ├── feature/orderbook
 ├── feature/x-event
 ├── feature/replay
 ├── feature/execution
 └── feature/metrics
```

完成：

```text
开发
↓
本地测试
↓
Replay / Integration
↓
PR
↓
Review
↓
Merge
```

---

# 26. 新开发第一次上手

第一步：

```bash
git clone <repo>
cd <repo>
```

编译：

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4
```

运行 Demo：

```bash
./build/hft_demo
```

或者：

```bash
./scripts/run_demo.sh
```

然后阅读：

```text
README.md
docs/ARCHITECTURE.md
docs/TEST_PLAN.md
docs/TEAM_ONBOARDING.md
```

---

# 27. 第一个任务应该是什么

新成员不要第一天就改 Strategy。

推荐顺序：

```text
Day 1
环境 + 编译 + Demo

Day 2
理解 Market Data / Event / Order / Fill 数据结构

Day 3
跑 Integration Test

Day 4
跑 Replay

Day 5
开始负责自己的模块
```

第一个正式开发任务最好是：

> **增加一个明确、可测试、不会改变 Alpha 行为的基础设施功能。**

例如：

```text
OrderBook 数据检查
Event Schema 扩展
Recorder
Replay 数据校验
Execution Mock
Metrics
```

---

# 28. 哪些事情不要做

### 不要自己修改策略参数

除非策略负责人明确要求。

### 不要猜策略逻辑

接口不明确就问。

### 不要把业务判断塞进基础设施

例如：

```text
MarketData
```

不应该决定买卖。

### 不要为了方便把数据库放进 Hot Path

### 不要为了简单把所有模块写在一个类里

### 不要用未来数据做 Replay

### 不要用“跑通了”代替测试

### 不要直接拿 Live API 做未经验证的实验

---

# 29. 开发人员最终交付的不是一个 Demo

我们最终需要的是：

```text
Reliable Infrastructure
        +
Reproducible Research
        +
Controlled Execution
```

也就是：

```text
实时运行
     +
历史回放
     +
Paper Trading
     +
完整记录
     +
可重复测试
```

策略研究人员以后可以替换：

```text
Alpha A
Alpha B
Alpha C
```

而不需要重写：

```text
Market Data
Risk
Execution
Recorder
Replay
```

同样，换交易所也不应该重写整个 Strategy。

---

# 30. 给开发人员的最终工作目标

你不需要理解这个系统为什么能够产生交易信号。

你需要保证的是：

```text
数据进得来
    ↓
数据不乱
    ↓
状态维护正确
    ↓
策略调用正确
    ↓
信号不丢
    ↓
风险检查正确
    ↓
订单状态正确
    ↓
成交记录正确
    ↓
仓位正确
    ↓
数据完整保存
    ↓
历史可以重新跑
```

如果做到这一点，策略研究人员可以在这个基础设施上持续迭代 Alpha，而不用反复修改交易系统底层。

---

## 项目开发者应该记住的一句话

> **你负责把交易系统做成一个稳定、低延迟、可测试、可回放的机器；策略负责告诉这台机器什么时候应该做什么。**

**策略是 Black Box，接口是 Contract，测试是验收标准。**
