#ifndef I_SCHEDULER_HPP
#define I_SCHEDULER_HPP

#include "aee/core/types.hpp"

namespace aee {
    class IScheduler {
    public:
        virtual ~IScheduler() = default;
        virtual Quantity next_slice(Quantity remaining, const MarketState& state) = 0;
        virtual void reset() = 0;
    };
}

#endif // I_SCHEDULER_HPP