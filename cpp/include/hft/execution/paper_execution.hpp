#pragma once
#include "hft/core/types.hpp"
#include <atomic>
namespace hft{class PaperExecution{std::atomic<uint64_t>next{1};public:uint64_t submit(const OrderIntent&)noexcept{return next.fetch_add(1,std::memory_order_relaxed);}void cancel(uint64_t)noexcept{}};}
