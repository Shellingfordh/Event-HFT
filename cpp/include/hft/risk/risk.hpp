#pragma once
#include "hft/core/types.hpp"
namespace hft { bool risk_allows(const Decision&, const MarketSnapshot&, const EventSignal&, double max_spread_bps); }
