#pragma once
#include <chrono>
#include <cstdint>
namespace hft {
inline std::uint64_t now_ns(){ return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(); }
class DecisionClock {
  std::uint64_t budget_ns_;
public:
  explicit DecisionClock(std::uint64_t budget_us): budget_ns_(budget_us*1000ULL) {}
  bool within(std::uint64_t start) const { return now_ns()-start <= budget_ns_; }
  std::uint64_t budget_ns() const { return budget_ns_; }
};
}
