#include "hft/event/event_alpha.hpp"
#include <algorithm>
namespace hft { double event_score(const EventSignal& e){ return std::clamp(0.30*e.relevance+0.20*e.novelty+0.15*e.credibility+0.35*e.impact,0.0,1.0); } }
