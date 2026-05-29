#include "aee/simulation/simple_market_simulator.hpp"
#include "aee/simulation/adaptive_scheduler.hpp"
#include "aee/simulation/simulation_loop.hpp"
#include <iostream>
#include <memory>

int main() {
    auto market = std::make_unique<aee::SimpleMarketSimulator>(100.0, 3000, 10.0);
    auto scheduler = std::make_unique<aee::AdaptiveScheduler>(500, 250, 15.0);
    aee::Order order(1, aee::Side::BUY, 5000, 0.0);
    
    aee::SimulationLoop loop(std::move(market), std::move(scheduler), order, std::chrono::milliseconds(100));
    loop.run();
    
    return 0;
}