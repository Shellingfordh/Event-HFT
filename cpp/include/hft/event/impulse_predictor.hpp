#pragma once
#include "hft/core/types.hpp"
namespace hft{struct ImpulseObservation{uint64_t timestamp_ns{};MicroFeatures micro{};double trade_buy_ratio{},trade_intensity{},short_return{};};struct ImpulsePrediction{double expected_move{},confidence{};uint32_t horizon_ms{};bool valid{};};class ImpulsePredictor{public:virtual~ImpulsePredictor()=default;virtual ImpulsePrediction predict(const ImpulseObservation&)noexcept=0;};class RedactedImpulse:public ImpulsePredictor{public:ImpulsePrediction predict(const ImpulseObservation&)noexcept override{return{};}};}
