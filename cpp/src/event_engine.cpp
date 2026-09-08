#include "hft/event/event_engine.hpp"
namespace hft{EventFeatures extract_event_features(const Event&e){bool c=e.text.find("ETF")!=std::string::npos||e.text.find("SEC")!=std::string::npos||e.text.find("hack")!=std::string::npos||e.text.find("listing")!=std::string::npos;return{c,!e.id.empty(),.5,c?1.:0.};}}
