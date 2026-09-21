#include "hft/decision_loop.hpp"
#include "hft/core/clock.hpp"
#include "hft/features/microstructure.hpp"
#include "hft/alpha/signal_fusion.hpp"
namespace hft {
Decision DecisionLoop::run(const MarketSnapshot& s,const MarketSnapshot* prev,const EventSignal& e){
  Decision d; auto start=now_ns(); if(in_flight_){d.ts=start;d.late=true;d.reason="decision already in flight";return d;} in_flight_=true;
  auto m=compute_micro(s,prev); d=fuse(e,m,start,threshold_); DecisionClock clock(budget_us_); if(!clock.within(start)){d.action=Action::Hold;d.late=true;d.reason="decision deadline exceeded";} in_flight_=false; return d;
}
}
