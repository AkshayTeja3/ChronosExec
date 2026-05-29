#include "aee/simulation/simple_market_simulator.hpp"
#include "aee/simulation/adaptive_scheduler.hpp"
#include "aee/simulation/simulation_loop.hpp"
#include "aee/core/thread_pool.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <memory>

const int NUM_ORDERS = 4;
const int SHARES_PER_ORDER = 200000;
const int DEPTH =50000;

void run_order(int id) {
    auto market = std::make_unique<aee::SimpleMarketSimulator>(100.0, DEPTH, 10.0);
    auto scheduler = std::make_unique<aee::AdaptiveScheduler>(500, 250, 15.0);
    aee::Order order(id, aee::Side::BUY, SHARES_PER_ORDER, 0.0);
    
    aee::SimulationLoop loop(std::move(market), std::move(scheduler), order, std::chrono::milliseconds(100));
    loop.run();
}

int main() {
    std::cout << "=== Benchmark: " << NUM_ORDERS << " orders x " << SHARES_PER_ORDER << " shares ===" << std::endl;
    
    // Sequential
    auto seq_start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_ORDERS; ++i) {
        run_order(i);
    }
    auto seq_end = std::chrono::high_resolution_clock::now();
    auto seq_ms = std::chrono::duration_cast<std::chrono::milliseconds>(seq_end - seq_start).count();
    
    // Parallel
    aee::ThreadPool pool(4);
    auto par_start = std::chrono::high_resolution_clock::now();
    
    std::vector<std::future<void>> futures;
    for (int i = 0; i < NUM_ORDERS; ++i) {
        futures.push_back(pool.submit(run_order, i));
    }
    for (auto& f : futures) {
        f.get();
    }
    
    auto par_end = std::chrono::high_resolution_clock::now();
    auto par_ms = std::chrono::duration_cast<std::chrono::milliseconds>(par_end - par_start).count();
    
    double speedup = static_cast<double>(seq_ms) / par_ms;
    
    std::cout << "\n=== RESULTS ===" << std::endl;
    std::cout << "Sequential: " << seq_ms << " ms" << std::endl;
    std::cout << "Parallel (4 threads): " << par_ms << " ms" << std::endl;
    std::cout << "Speedup: " << speedup << "x" << std::endl;
    
    return 0;
}