from dataclasses import dataclass

@dataclass
class RiskState:
    daily_loss: float = 0.0
    open_notional: float = 0.0
    consecutive_losses: int = 0

@dataclass(frozen=True)
class RiskLimits:
    max_daily_loss: float
    max_open_notional: float
    max_consecutive_losses: int

def allow_new_trade(state: RiskState, limits: RiskLimits, proposed_notional: float) -> bool:
    if state.daily_loss >= limits.max_daily_loss:
        return False
    if state.open_notional + proposed_notional > limits.max_open_notional:
        return False
    if state.consecutive_losses >= limits.max_consecutive_losses:
        return False
    return True
