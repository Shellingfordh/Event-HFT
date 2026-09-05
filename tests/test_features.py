from src.core.models import Event, MarketSnapshot
from src.features.event_features import extract_event_features
from src.features.orderbook import top_of_book_imbalance, microprice

def test_event_features():
    event = Event("tweet", 1, "test", "1", {"text": "$BTC BREAKING https://example.com"})
    f = extract_event_features(event)
    assert "$BTC" in f.cashtags
    assert f.has_url

def test_orderbook_features():
    s = MarketSnapshot("BTCUSDT", 1, 100.0, 101.0, 100.5, 3.0, 1.0)
    assert top_of_book_imbalance(s) == 0.5
    assert 100.0 < microprice(s) < 101.0
