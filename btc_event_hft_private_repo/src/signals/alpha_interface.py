from abc import ABC, abstractmethod
from src.core.models import Event, MarketSnapshot, Signal

class AlphaEngine(ABC):
    """Private alpha boundary. Production logic belongs in a private package."""

    @abstractmethod
    def evaluate(self, event: Event, market: MarketSnapshot) -> Signal:
        raise NotImplementedError

class RedactedAlpha(AlphaEngine):
    def evaluate(self, event: Event, market: MarketSnapshot) -> Signal:
        return Signal(
            action="FLAT",
            score=0.0,
            reason_code="REDACTED_ALPHA",
            timestamp_ns=market.timestamp_ns,
            ttl_ms=0,
        )
