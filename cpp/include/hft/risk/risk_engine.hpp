#pragma once
#include "hft/core/types.hpp"
namespace hft{struct RiskLimits{double max_daily_loss{5},max_open_notional{50};uint32_t max_consecutive_losses{3};uint64_t max_signal_age_ns{500000000};};struct RiskState{double daily_loss{},open_notional{};uint32_t consecutive_losses{};};class RiskEngine{public:bool allow(const RiskState&,const RiskLimits&,const Signal&,double,uint64_t)const noexcept;};}
