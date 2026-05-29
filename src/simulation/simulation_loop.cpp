#include "aee/simulation/simulation_loop.hpp"
#include <iostream>
#include <chrono>
#include <mutex>


namespace aee{
    std::mutex cout_mutex;
    
    SimulationLoop ::SimulationLoop(std::unique_ptr<IMarketSimulator> market_sim,

        std::unique_ptr<IScheduler> scheduler,

        Order order,

        std::chrono::milliseconds time_step)

        : market_sim_(std::move(market_sim)),

          scheduler_(std::move(scheduler)),

          order_(order),

          time_step_(time_step) {}

    void SimulationLoop::run() {

    auto start_time = std::chrono::high_resolution_clock::now();

    Price arrival_price = market_sim_->get_current_state().ask;
    
    while (order_.remaining() > 0) {

        Quantity slice_size = scheduler_->next_slice(order_.remaining(), market_sim_->get_current_state());
        
        if (slice_size == 0) {

            std ::cout << "Warning Scheduler returned 0, stopping simulation"<<std::endl;

            break;

        }
        
        Fill fill = market_sim_->execute(slice_size);

        if (fill.filled_quantity == 0) {

            std::cout << "No fill received for slice size " << slice_size << ", stopping simulation." << std::endl;

            break;

        }
        order_.fill(fill.filled_quantity, fill.execution_price, fill.execution_time, fill.latency_ns);

        
        market_sim_->update_state(time_step_);
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end_time - start_time;
    
    double total_cost = order_.average_price() * order_.filled();

    double arrival_cost = arrival_price * order_.filled();

    double shortfall = total_cost - arrival_cost;

    {

        std::lock_guard<std::mutex> lock(cout_mutex);
    
    
    std::cout << "=== SIMULATION REPORT ===" << std::endl;

    std::cout << "Total time: " << elapsed.count() << " seconds" << std::endl;

    std::cout << "Slices executed: " << order_.fills().size() << std::endl;

    std::cout << "Arrival price: " << arrival_price << std::endl;

    std::cout << "Average execution price: " << order_.average_price() << std::endl;

    std::cout << "Implementation shortfall: " << shortfall << std::endl;

    std::cout << "Total filled: " << order_.filled() << " / " << order_.filled() + order_.remaining() << std::endl;

    }

    

}

}