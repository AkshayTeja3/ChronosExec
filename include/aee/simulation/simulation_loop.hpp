#ifndef SIMULATION_LOOP_HPP
#include "aee/core/order.hpp"
#include "aee/interfaces/i_market_simulator.hpp"
#include "aee/interfaces/i_scheduler.hpp"
#include <memory>

namespace aee{
    class SimulationLoop{
        
        public:
         SimulationLoop(std::unique_ptr<IMarketSimulator> market_sim,
        std::unique_ptr<IScheduler> scheduler,
        Order order,
        std::chrono::milliseconds time_step);

        void run();
        private:
            std::unique_ptr<IMarketSimulator> market_sim_;
            std::unique_ptr<IScheduler> scheduler_;
            Order order_;
            std::chrono::milliseconds time_step_;
            
};
}
#endif // SIMULATION_LOOP_HPP
