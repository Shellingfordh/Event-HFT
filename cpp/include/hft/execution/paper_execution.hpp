#pragma once
#include "hft/core/types.hpp"
namespace hft { OrderIntent make_paper_order(const Decision&, const MarketSnapshot&, double qty); }
