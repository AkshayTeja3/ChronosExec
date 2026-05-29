cmake_minimum_required(VERSION 3.20)
project(AdaptiveExecutionEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Common source files
set(CORE_SOURCES
    src/core/order.cpp
    src/core/thread_pool.cpp
)

set(SIMULATION_SOURCES
    src/simulation/simple_market_simulator.cpp
    src/simulation/naive_scheduler.cpp
    src/simulation/adaptive_scheduler.cpp
    src/simulation/simulation_loop.cpp
)

# Main simulation executable
add_executable(aee_sim 
    ${CORE_SOURCES}
    ${SIMULATION_SOURCES}
    src/main.cpp
)

# Concurrency test executable
add_executable(concurrency_test 
    ${CORE_SOURCES}
    src/test_concurrency.cpp
)

# Parallel simulation executable
add_executable(parallel_sim 
    ${CORE_SOURCES}
    ${SIMULATION_SOURCES}
    src/parallel_main.cpp
)

# Benchmark executable
add_executable(benchmark 
    src/core/order.cpp
    src/core/thread_pool.cpp
    src/simulation/simple_market_simulator.cpp
    src/simulation/naive_scheduler.cpp
    src/simulation/adaptive_scheduler.cpp
    src/simulation/simulation_loop.cpp
    src/benchmark.cpp
)