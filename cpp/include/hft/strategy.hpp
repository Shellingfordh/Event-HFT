#pragma once
#include <array>
#include <deque>
#include <cmath>
#include <cstdint>
#include <algorithm>

namespace hft {
enum class Side { Flat=0, Long=1, Short=-1 };
struct Level { double bid_px{}, bid_qty{}, ask_px{}, ask_qty{}; };
struct Book { std::array<Level,10> l{}; uint64_t ts{}; double last{}; };
struct Event { double relevance=0, novelty=0, sentiment=0, shock=0; uint64_t ts{}; };
struct Features {
 double mid{}, spread{}, top5_imb{}, top2_trade_imb{}, thickness_imb{}, microprice_edge{}, pressure{}, slope_bid{}, slope_ask{}, velocity{}, event_score{}, persistence{}, impulse{};
};
struct Params {
 double entry=0.62, exit=0.20, add_on=0.78, event_weight=0.30, memory_weight=0.25;
 double max_spread_bps=3.0, min_persistence=0.55, max_position=1.0, add_on_fraction=0.5;
 double stop_bps=8.0, take_bps=12.0, max_hold_us=750000;
 double decay_us=250000.0;
};
struct Position { Side side=Side::Flat; double qty=0, entry_px=0; uint64_t entry_ts=0; };

class TemporalMemory {
 struct Sample { uint64_t ts; double signed_flow; double mid; };
 std::deque<Sample> q_; double decay_;
 public:
 explicit TemporalMemory(double decay_us):decay_(decay_us){}
 void push(uint64_t ts,double flow,double mid){q_.push_back({ts,flow,mid}); while(q_.size()>4096 || (!q_.empty() && ts-q_.front().ts>5000000)) q_.pop_front();}
 double persistence(uint64_t now,double direction) const {
   double num=0,den=0; for(auto it=q_.rbegin();it!=q_.rend();++it){ if(it->ts>now) continue; double w=std::exp(-double(now-it->ts)/decay_); den+=w; num+=w*((it->signed_flow*direction>0)?1.0:-1.0); }
   return den?std::clamp((num/den+1.0)/2.0,0.0,1.0):0.5;
 }
};

inline Features compute_features(const Book& b, const Event& e, double prev_mid=0, double prev_flow=0) {
 Features f; const auto& x=b.l; f.mid=(x[0].bid_px+x[0].ask_px)*0.5; f.spread=x[0].ask_px-x[0].bid_px;
 double b5=0,a5=0,b2=0,a2=0,ball=0,aall=0; for(int i=0;i<10;i++){ball+=x[i].bid_qty; aall+=x[i].ask_qty; if(i<5){b5+=x[i].bid_qty;a5+=x[i].ask_qty;} if(i<2){b2+=x[i].bid_qty;a2+=x[i].ask_qty;}}
 auto imb=[](double a,double b){return (a+b>0)?(a-b)/(a+b):0.0;};
 f.top5_imb=imb(b5,a5); f.top2_trade_imb=imb(b2,a2); f.thickness_imb=imb(ball,aall);
 double micro=(x[0].ask_px*x[0].bid_qty+x[0].bid_px*x[0].ask_qty)/(x[0].bid_qty+x[0].ask_qty+1e-12); f.microprice_edge=(f.mid>0)?(micro-f.mid)/f.mid*10000:0;
 f.pressure=0.50*f.top5_imb+0.30*f.top2_trade_imb+0.20*f.thickness_imb;
 double bden=0,aden=0; for(int i=0;i<10;i++){bden+=x[i].bid_qty*(i+1);aden+=x[i].ask_qty*(i+1);} f.slope_bid=bden/(ball+1e-9); f.slope_ask=aden/(aall+1e-9);
 f.velocity=(prev_mid>0)?(f.mid-prev_mid)/prev_mid*1e6:0; f.event_score=std::clamp(0.45*e.relevance+0.30*e.novelty+0.25*std::abs(e.sentiment),0.0,1.0);
 f.impulse=std::clamp(0.65*std::abs(f.pressure)+0.20*std::abs(f.microprice_edge)/10.0+0.15*f.event_score,0.0,1.0);
 return f;
}

class Strategy {
 Params p_; TemporalMemory mem_; Position pos_{}; double prev_mid_=0,prev_flow_=0;
 public:
 explicit Strategy(Params p):p_(p),mem_(p.decay_us){}
 const Position& position()const{return pos_;}
 double score(const Features& f, const Event& e, uint64_t ts){
   double raw=0.55*f.pressure + 0.20*std::tanh(f.velocity/2.0) + p_.event_weight*(e.sentiment*e.relevance*e.novelty);
   double dir=raw>=0?1:-1; double pers=mem_.persistence(ts,dir); return std::clamp(0.65*std::abs(raw)+0.20*pers+0.15*f.impulse,0.0,1.0)*(raw>=0?1:-1);
 }
 Side on_book(const Book& b,const Event& e,uint64_t ts){
   auto f=compute_features(b,e,prev_mid_,prev_flow_); double flow=f.pressure; mem_.push(ts,flow,f.mid); double s=score(f,e,ts); double abs_s=std::abs(s);
   double spread_bps=f.mid>0?f.spread/f.mid*10000:999;
   if(pos_.side!=Side::Flat){ bool stop=(pos_.side==Side::Long?f.mid<=pos_.entry_px*(1-p_.stop_bps/10000):f.mid>=pos_.entry_px*(1+p_.stop_bps/10000)); bool take=(pos_.side==Side::Long?f.mid>=pos_.entry_px*(1+p_.take_bps/10000):f.mid<=pos_.entry_px*(1-p_.take_bps/10000)); bool timeout=ts-pos_.entry_ts>p_.max_hold_us; bool flip=(pos_.side==Side::Long?s<-p_.exit:s>p_.exit); if(stop||take||timeout||flip||abs_s<p_.exit){pos_={};} }
   if(pos_.side==Side::Flat && spread_bps<=p_.max_spread_bps && abs_s>=p_.entry && f.impulse>=p_.entry){pos_.side=s>0?Side::Long:Side::Short;pos_.qty=p_.max_position;pos_.entry_px=f.mid;pos_.entry_ts=ts;}
   else if(pos_.side!=Side::Flat && abs_s>=p_.add_on && ((s>0&&pos_.side==Side::Long)||(s<0&&pos_.side==Side::Short)) && pos_.qty<p_.max_position*(1+p_.add_on_fraction)){pos_.qty+=p_.max_position*p_.add_on_fraction;}
   prev_mid_=f.mid;prev_flow_=flow; return pos_.side;
 }
};
}
