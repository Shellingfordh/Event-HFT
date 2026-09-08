#pragma once
#include <array>
#include <cstdint>
#include <string>
namespace hft { enum class Side:uint8_t{Buy,Sell}; enum class Action:uint8_t{Flat,Long,Short}; struct Event{uint64_t timestamp_ns{};uint64_t sequence{};std::string source,id,text;}; struct Level{double price{},quantity{};}; template<size_t N> struct OrderBook{std::array<Level,N>bids{},asks{};uint64_t timestamp_ns{},sequence{};}; using Book10=OrderBook<10>; struct MicroFeatures{double top_imbalance{},depth_imbalance{},microprice{},spread{},pressure{},depth_slope{};}; struct Signal{Action action{Action::Flat};double score{};uint64_t timestamp_ns{};uint32_t ttl_ms{};const char* reason_code{"FLAT"};}; struct OrderIntent{Side side{Side::Buy};double quantity{},limit_price{};bool reduce_only{};}; }
