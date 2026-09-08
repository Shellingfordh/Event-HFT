#pragma once
#include "hft/core/types.hpp"
namespace hft {struct EventFeatures{bool relevant{},novel{};double source_quality{},event_strength{};};EventFeatures extract_event_features(const Event&);}
