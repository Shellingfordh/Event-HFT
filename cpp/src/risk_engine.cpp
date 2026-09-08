#include "hft/risk/risk_engine.hpp"
namespace hft{bool RiskEngine::allow(const RiskState&s,const RiskLimits&l,const Signal&g,double n,uint64_t now)const noexcept{return g.action!=Action::Flat&&now>=g.timestamp_ns&&now-g.timestamp_ns<=l.max_signal_age_ns&&s.daily_loss<l.max_daily_loss&&s.open_notional+n<=l.max_open_notional&&s.consecutive_losses<l.max_consecutive_losses;}}
