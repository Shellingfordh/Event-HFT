#include "hft/core/spsc_queue.hpp"
#include "hft/market/orderbook.hpp"
#include <cassert>
int main(){hft::SPSCQueue<int,8>q;assert(q.push(42));int x=0;assert(q.pop(x)&&x==42);hft::Book10 b;for(int i=0;i<10;i++){b.bids[i]={100-i,10};b.asks[i]={101+i,5};}assert(hft::compute_micro_features(b).depth_imbalance>0);return 0;}
