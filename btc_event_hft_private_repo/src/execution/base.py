from abc import ABC, abstractmethod
from src.core.models import OrderIntent

class ExecutionAdapter(ABC):
    @abstractmethod
    def submit(self, order: OrderIntent) -> str:
        raise NotImplementedError

    @abstractmethod
    def cancel(self, client_order_id: str) -> None:
        raise NotImplementedError

class PaperExecution(ExecutionAdapter):
    def submit(self, order: OrderIntent) -> str:
        return f"PAPER-{order.client_tag}"

    def cancel(self, client_order_id: str) -> None:
        return None
