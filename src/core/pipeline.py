from src.core.models import Event, MarketSnapshot, Signal
from src.features.event_features import extract_event_features
from src.signals.alpha_interface import AlphaEngine

class EventPipeline:
    def __init__(self, alpha: AlphaEngine):
        self.alpha = alpha

    def process(self, event: Event, market: MarketSnapshot) -> Signal:
        _ = extract_event_features(event)
        return self.alpha.evaluate(event, market)
