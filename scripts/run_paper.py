from src.core.models import Event, MarketSnapshot
from src.core.pipeline import EventPipeline
from src.signals.alpha_interface import RedactedAlpha

def main():
    pipeline = EventPipeline(RedactedAlpha())
    event = Event("tweet", 1, "demo", "demo-1", {"text": "$BTC example event"})
    market = MarketSnapshot("BTCUSDT", 2, 63800, 63801, 63800.5, 10, 8)
    print(pipeline.process(event, market))

if __name__ == "__main__":
    main()
