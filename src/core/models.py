from dataclasses import dataclass, field
from typing import Any, Literal

EventType = Literal["tweet", "market", "system"]

@dataclass(frozen=True)
class Event:
    event_type: EventType
    timestamp_ns: int
    source: str
    event_id: str
    payload: dict[str, Any] = field(default_factory=dict)

@dataclass(frozen=True)
class MarketSnapshot:
    symbol: str
    timestamp_ns: int
    bid: float
    ask: float
    last: float
    bid_size: float = 0.0
    ask_size: float = 0.0

    @property
    def mid(self) -> float:
        return (self.bid + self.ask) / 2.0

    @property
    def spread(self) -> float:
        return self.ask - self.bid

@dataclass(frozen=True)
class Signal:
    action: Literal["LONG", "SHORT", "FLAT"]
    score: float
    reason_code: str
    timestamp_ns: int
    ttl_ms: int
    metadata: dict[str, Any] = field(default_factory=dict)

@dataclass(frozen=True)
class OrderIntent:
    symbol: str
    side: Literal["BUY", "SELL"]
    quantity: float
    limit_price: float | None
    reduce_only: bool = False
    client_tag: str = "event-hft"
