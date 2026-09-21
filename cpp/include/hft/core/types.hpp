#pragma once
#include <array>
#include <cstdint>
#include <string>
namespace hft {
using TimestampNs = std::uint64_t;
enum class Side { None, Long, Short };
enum class Action { Hold, EnterLong, EnterShort, Add, Exit };
struct Level { double px=0; double qty=0; };
struct OrderBook10 { TimestampNs ts=0; std::array<Level,10> bid{}; std::array<Level,10> ask{}; };
struct Trade { TimestampNs ts=0; double px=0; double qty=0; Side aggressor=Side::None; };
struct MarketSnapshot { TimestampNs ts=0; OrderBook10 book{}; Trade last{}; double mid=0; double spread_bps=0; };
struct EventSignal { TimestampNs ts=0; std::string id; std::string asset; Side direction=Side::None; double relevance=0; double novelty=0; double credibility=0; double impact=0; double duration_ms=0; };
struct MicroSignal { TimestampNs ts=0; double imbalance=0; double ofi=0; double pressure=0; double velocity=0; double acceleration=0; double liquidity=1; double persistence=0; };
struct Decision { TimestampNs ts=0; Action action=Action::Hold; double score=0; double event_score=0; double micro_score=0; bool late=false; std::string reason; };
struct OrderIntent { TimestampNs ts=0; Side side=Side::None; double qty=0; double limit_px=0; std::string reason; };
}
