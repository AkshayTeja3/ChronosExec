#include "aee/core/thread_pool.hpp"
#include "aee/simulation/simple_market_simulator.hpp"
#include "aee/simulation/adaptive_scheduler.hpp"
#include "aee/simulation/simulation_loop.hpp"
#include <iostream>
#include <memory>
#include <vector>

// Function to run one simulation
double run_single_simulation(int order_id, int shares, int depth) {
    auto market = std::make_unique<aee::SimpleMarketSimulator>(100.0, depth, 10.0);
    auto scheduler = std::make_unique<aee::AdaptiveScheduler>(500, 250, 15.0);
    aee::Order order(order_id, aee::Side::BUY, shares, 0.0);
    
    aee::SimulationLoop loop(std::move(market), std::move(scheduler), order, std::chrono::milliseconds(100));
    loop.run();
    
    // Return shortfall (we'll capture it from output later)
    return 0.0;  // Placeholder
}

int main() {
    aee::ThreadPool pool(4);
    
    std::cout << "=== Running 5 Orders in Parallel ===" << std::endl;
    
    std::vector<std::future<double>> results;
    
    // Submit 5 orders of different sizes
    for (int i = 0; i < 5; ++i) {
        int shares = 2000 + (i * 1000);  // 2000, 3000, 4000, 5000, 6000
        results.push_back(pool.submit(run_single_simulation, i, shares, 3000));
    }
    
    // Wait for all to complete
    for (auto& result : results) {
        result.get();
    }
    
    std::cout << "\nAll 5 orders completed in parallel!" << std::endl;
    
    return 0;
}