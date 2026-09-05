from dataclasses import dataclass
import re
from src.core.models import Event

@dataclass(frozen=True)
class EventFeatures:
    text_length: int
    has_url: bool
    cashtags: tuple[str, ...]
    uppercase_ratio: float

_CASHTAG = re.compile(r"\$[A-Za-z]{2,10}")

def extract_event_features(event: Event) -> EventFeatures:
    text = str(event.payload.get("text", ""))
    letters = [c for c in text if c.isalpha()]
    upper = sum(c.isupper() for c in letters)
    ratio = upper / len(letters) if letters else 0.0
    return EventFeatures(
        text_length=len(text),
        has_url=bool(re.search(r"https?://", text)),
        cashtags=tuple(x.upper() for x in _CASHTAG.findall(text)),
        uppercase_ratio=ratio,
    )
