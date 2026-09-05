from collections.abc import Iterator
from src.core.models import Event
from .base import EventSource

class XPublicAdapter(EventSource):
    """
    Adapter boundary for an authorized X/Twitter data source.

    Credentials and source-selection logic stay outside this repository.
    The adapter emits normalized Event objects only.
    """

    def __init__(self, client=None):
        self.client = client

    def events(self) -> Iterator[Event]:
        yield from ()
