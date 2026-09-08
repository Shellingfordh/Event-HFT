#pragma once
#include "hft/core/types.hpp"
namespace hft {MicroFeatures compute_micro_features(const Book10&);double top_imbalance(const Book10&)noexcept;double depth_imbalance(const Book10&)noexcept;double microprice(const Book10&)noexcept;double depth_slope(const Book10&)noexcept;}
