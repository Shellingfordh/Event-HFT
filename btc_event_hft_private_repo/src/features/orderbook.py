from src.core.models import MarketSnapshot

def top_of_book_imbalance(snapshot: MarketSnapshot) -> float:
    total = snapshot.bid_size + snapshot.ask_size
    if total <= 0:
        return 0.0
    return (snapshot.bid_size - snapshot.ask_size) / total

def microprice(snapshot: MarketSnapshot) -> float:
    total = snapshot.bid_size + snapshot.ask_size
    if total <= 0:
        return snapshot.mid
    return (
        snapshot.ask * snapshot.bid_size
        + snapshot.bid * snapshot.ask_size
    ) / total
