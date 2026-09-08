#pragma once
#include <array>
#include <atomic>
#include <cstddef>
#include <utility>
namespace hft { template<class T,size_t N> class SPSCQueue{static_assert((N&(N-1))==0,"N must be power of two");std::array<T,N>b{};alignas(64)std::atomic<size_t>h{0},t{0};public:bool push(const T&v)noexcept{auto x=h.load(std::memory_order_relaxed),n=x+1;if(n-t.load(std::memory_order_acquire)>N)return false;b[x&(N-1)]=v;h.store(n,std::memory_order_release);return true;}bool pop(T&v)noexcept{auto x=t.load(std::memory_order_relaxed);if(x==h.load(std::memory_order_acquire))return false;v=std::move(b[x&(N-1)]);t.store(x+1,std::memory_order_release);return true;}}; }
