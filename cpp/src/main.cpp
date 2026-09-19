#include <iostream>
#include <iomanip>
#include "hft/strategy.hpp"
using namespace hft;
int main(){
 Params p; Strategy s(p); Book b; b.ts=1'000'000; double mid=100000;
 for(int t=0;t<30;t++){
   double m=mid+t*0.8; for(int i=0;i<10;i++){b.l[i].bid_px=m-i*0.5;b.l[i].ask_px=m+i*0.5;b.l[i].bid_qty=100+i*8+(t>10?50:0);b.l[i].ask_qty=100+i*10-(t>10?25:0);} Event e; e.relevance=t>10?0.9:0.2;e.novelty=t==11?1:0.2;e.sentiment=t>10?0.8:0.05; e.ts=b.ts+t*25000; s.on_book(b,e,e.ts); auto pos=s.position(); std::cout<<e.ts<<" mid="<<std::fixed<<std::setprecision(2)<<m<<" pos="<<(int)pos.side<<" qty="<<pos.qty<<" entry="<<pos.entry_px<<"\n";
 }
}
