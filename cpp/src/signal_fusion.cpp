#include "hft/alpha/signal_fusion.hpp"
#include "hft/event/event_alpha.hpp"
#include <algorithm>
#include <cmath>
namespace hft {
Decision fuse(const EventSignal& e,const MicroSignal& m,std::uint64_t now,double threshold){
 Decision d; d.ts=now; d.event_score=event_score(e); d.micro_score=std::clamp(0.5+0.5*(0.55*m.imbalance+0.25*m.pressure+0.20*m.ofi),0.0,1.0);
 double dir=(e.direction==Side::Long?1.0:e.direction==Side::Short?-1.0:0.0); double microDir=(m.imbalance+m.pressure+m.ofi>=0?1.0:-1.0);
 d.score=0.45*d.event_score+0.55*d.micro_score; if(dir==0 || microDir!=dir || m.liquidity<0.25){d.action=Action::Hold; d.reason="event/market not confirmed";return d;}
 if(d.score>=threshold && m.persistence>=0.55){d.action=(dir>0?Action::EnterLong:Action::EnterShort);d.reason="event confirmed by microstructure";} else {d.action=Action::Hold;d.reason="below confirmation threshold";} return d;
}
}
