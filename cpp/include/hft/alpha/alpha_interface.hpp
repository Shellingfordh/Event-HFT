#pragma once
#include "hft/core/types.hpp"
#include "hft/event/event_engine.hpp"
namespace hft{class AlphaEngine{public:virtual~AlphaEngine()=default;virtual Signal evaluate(const Event&,const EventFeatures&,const Book10&,const MicroFeatures&)noexcept=0;};class RedactedAlpha:public AlphaEngine{public:Signal evaluate(const Event&,const EventFeatures&,const Book10&,const MicroFeatures&)noexcept override{return{Action::Flat,0,0,0,"PRIVATE_ALPHA"};}};}
