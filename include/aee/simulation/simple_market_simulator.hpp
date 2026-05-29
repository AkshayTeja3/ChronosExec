#ifndef SIMPLE_MARKET_SIMULATOR_HPP
#define SIMPLE_MARKET_SIMULATOR_HPP

#include "aee/interfaces/i_market_simulator.hpp"
#include <chrono>
#include <random>
#include <mutex>

namespace aee {
    class SimpleMarketSimulator : public IMarketSimulator {
    public:
        SimpleMarketSimulator(Price initial_mid, Quantity depth, double spread_bps);
        ~SimpleMarketSimulator() override = default;
        
        Fill execute(Quantity qty) override;
        MarketState get_current_state() const override { return state_; }
        void update_state(std::chrono::milliseconds dt) override;
        void set_regime(MarketRegime regime);
        void apply_event_shock(double volatility_multiplier,double price_shock);
        //Symbol get_symbol() const(){return symbol_;}
        
    private:
        MarketState state_;
        double spread_bps_;
        OrderId next_order_id_;
        Quantity depth_;  // ← add this, you need to store depth
        Quantity original_depth_; // ← store original depth for reference
        std::mt19937 rng_;
        std::normal_distribution<double> normal_dist_;
        double current_volatility_;
        mutable std::mutex market_mutex_;
        Symbol symbol_;

        Price calculate_execution_price(Quantity qty) const;
        double get_current_spread_bps() const;

    };
}

#endif