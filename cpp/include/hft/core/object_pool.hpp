#pragma once
#include <array>
namespace hft {template<class T,size_t N>class ObjectPool{std::array<T,N>a{};std::array<bool,N>u{};public:T*acquire()noexcept{for(size_t i=0;i<N;i++)if(!u[i]){u[i]=true;return&a[i];}return nullptr;}void release(T*p)noexcept{if(!p)return;auto i=(size_t)(p-a.data());if(i<N)u[i]=false;}};}
