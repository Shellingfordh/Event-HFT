#include "hft/execution/paper_execution.hpp"
namespace hft { OrderIntent make_paper_order(const Decision& d,const MarketSnapshot& s,double qty){OrderIntent o;o.ts=d.ts;o.qty=qty;o.reason=d.reason;if(d.action==Action::EnterLong){o.side=Side::Long;o.limit_px=s.book.ask[0].px;}else if(d.action==Action::EnterShort){o.side=Side::Short;o.limit_px=s.book.bid[0].px;}return o;} }
