cmake_minimum_required(VERSION 3.16)
project(hft_event_strategy_v5 CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(hft_demo
  cpp/src/main.cpp
  cpp/src/decision_loop.cpp
  cpp/src/microstructure.cpp
  cpp/src/event_alpha.cpp
  cpp/src/signal_fusion.cpp
  cpp/src/risk.cpp
  cpp/src/paper_execution.cpp
)
target_include_directories(hft_demo PRIVATE cpp/include)
if (UNIX)
  target_link_libraries(hft_demo PRIVATE pthread)
endif()
