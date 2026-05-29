#ifndef NAIVE_SCHEDULER_HPP
#define NAIVE_SCHEDULER_HPP

#include "aee/interfaces/i_scheduler.hpp"
#include <chrono>

namespace aee {
    class NaiveScheduler : public IScheduler {
    public:
        NaiveScheduler(Quantity slice_size, std::chrono::milliseconds interval);
        ~NaiveScheduler() override = default;
        
        Quantity next_slice(Quantity remaining, const MarketState& state) override;
        void reset() override;
        
    private:
        Quantity slice_size_;
        std::chrono::milliseconds interval_;
    };
}

#endif // NAIVE_SCHEDULER_HPP