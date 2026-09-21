#include "hft/features/microstructure.hpp"
#include <algorithm>
namespace hft {
MicroSignal compute_micro(const MarketSnapshot& s, const MarketSnapshot* prev){
  MicroSignal m; m.ts=s.ts;
  double bid=0,ask=0; for(int i=0;i<10;i++){bid+=s.book.bid[i].qty;ask+=s.book.ask[i].qty;}
  double den=bid+ask; m.imbalance=den?((bid-ask)/den):0;
  double microDen=s.book.bid[0].qty+s.book.ask[0].qty;
  double micro=microDen?((s.book.ask[0].px*s.book.bid[0].qty+s.book.bid[0].px*s.book.ask[0].qty)/microDen):s.mid;
  m.pressure=s.mid?((micro-s.mid)/s.mid*10000.0):0;
  if(prev && prev->ts<s.ts){ double dt=(s.ts-prev->ts)/1e6; if(dt>0) m.velocity=(s.mid-prev->mid)/dt; }
  if(prev) m.ofi=(s.book.bid[0].qty-prev->book.bid[0].qty)-(s.book.ask[0].qty-prev->book.ask[0].qty);
  m.acceleration=m.velocity; m.liquidity=std::clamp(1.0-s.spread_bps/50.0,0.0,1.0);
  double raw=0.45*m.imbalance+0.25*m.pressure+0.20*m.ofi+0.10*m.velocity;
  m.persistence=std::clamp(std::abs(raw),0.0,1.0);
  return m;
}
}
