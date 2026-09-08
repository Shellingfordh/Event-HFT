#include "hft/core/timestamp.hpp"
#include <chrono>
#if defined(__x86_64__)||defined(__i386__)
#include <x86intrin.h>
#endif
namespace hft{uint64_t monotonic_ns()noexcept{return(uint64_t)std::chrono::steady_clock::now().time_since_epoch().count();}uint64_t tsc_ticks()noexcept{
#if defined(__x86_64__)||defined(__i386__)
return __rdtsc();
#else
return monotonic_ns();
#endif
}}
