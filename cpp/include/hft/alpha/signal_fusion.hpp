#pragma once
#include "hft/core/types.hpp"
namespace hft { Decision fuse(const EventSignal&, const MicroSignal&, std::uint64_t now, double threshold); }
