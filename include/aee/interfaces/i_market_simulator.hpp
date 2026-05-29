#ifndef I_MARKET_SIMULATOR_HPP
#define I_MARKET_SIMULATOR_HPP

#include "aee/core/types.hpp"

namespace aee {
    class IMarketSimulator {
    public:
        virtual ~IMarketSimulator() = default;
        virtual Fill execute(Quantity qty) = 0;
        virtual MarketState get_current_state() const = 0;
        virtual void update_state(std::chrono::milliseconds dt) = 0;
    };
}

#endif // I_MARKET_SIMULATOR_HPP