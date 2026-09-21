#pragma once
#include "hft/core/types.hpp"
namespace hft {
class DecisionLoop {
 public:
  DecisionLoop(std::uint64_t budget_us, double threshold): budget_us_(budget_us), threshold_(threshold), in_flight_(false) {}
  Decision run(const MarketSnapshot&, const MarketSnapshot*, const EventSignal&);
 private: std::uint64_t budget_us_; double threshold_; bool in_flight_;
};
}
