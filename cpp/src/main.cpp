#include <iostream>
#include "hft/decision_loop.hpp"
#include "hft/core/clock.hpp"
#include "hft/risk/risk.hpp"
#include "hft/execution/paper_execution.hpp"
int main(){
 using namespace hft; MarketSnapshot s; s.ts=now_ns(); s.book.bid[0]={100,120};s.book.ask[0]={100.01,60};s.mid=100.005;s.spread_bps=1.0;
 EventSignal e; e.ts=s.ts;e.id="demo";e.asset="BTC";e.direction=Side::Long;e.relevance=.9;e.novelty=.9;e.credibility=.9;e.impact=.9;
 DecisionLoop loop(70000,.65); auto d=loop.run(s,nullptr,e); std::cout<<"action="<<(int)d.action<<" score="<<d.score<<" late="<<d.late<<" reason="<<d.reason<<"\n";
 if(risk_allows(d,s,e,15)){auto o=make_paper_order(d,s,.001);std::cout<<"paper order side="<<(int)o.side<<" px="<<o.limit_px<<" qty="<<o.qty<<"\n";} else std::cout<<"risk rejected\n";
}
