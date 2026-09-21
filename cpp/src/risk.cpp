#include "hft/risk/risk.hpp"
namespace hft { bool risk_allows(const Decision& d,const MarketSnapshot& s,const EventSignal& e,double max_spread_bps){ if(d.action==Action::Hold||d.late)return false; if(s.spread_bps>max_spread_bps)return false; if(e.impact<0.60)return false; return true; } }
