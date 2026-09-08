#pragma once
#include "hft/event/impulse_predictor.hpp"
#include <array>
namespace hft{template<size_t N>class TemporalMemory{std::array<ImpulseObservation,N>d{};size_t p{},n{};public:void push(const ImpulseObservation&x)noexcept{d[p]=x;p=(p+1)%N;if(n<N)n++;}size_t size()const noexcept{return n;}};}
