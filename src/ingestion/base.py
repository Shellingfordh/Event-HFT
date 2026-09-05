from abc import ABC, abstractmethod
from collections.abc import Iterator
from src.core.models import Event

class EventSource(ABC):
    @abstractmethod
    def events(self) -> Iterator[Event]:
        raise NotImplementedError
