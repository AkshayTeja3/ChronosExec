#include "aee/simulation/simple_market_simulator.hpp"
#include <stdexcept>
#include <chrono>
#include <iostream>
#include <thread>


namespace aee {

// Constructor
SimpleMarketSimulator::SimpleMarketSimulator(Price initial_mid, Quantity depth, double spread_bps)
    : depth_(depth), spread_bps_(spread_bps), next_order_id_(1),original_depth_(depth),rng_(std::random_device{}()),normal_dist_(0.0,1.0),current_volatility_(0.20){
    state_.bid = initial_mid * (1.0 - spread_bps / 10000.0);
    state_.ask = initial_mid * (1.0 + spread_bps / 10000.0);
    state_.bid_depth = depth;
    state_.ask_depth = depth;
    state_.mid_price = initial_mid;
    state_.volatility = 0.0;
    state_.timestamp = std::chrono::duration_cast<Timestamp>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    );
      
}
double SimpleMarketSimulator::get_current_spread_bps() const{
    return 10.0;
}

// Calculate execution price
Price SimpleMarketSimulator::calculate_execution_price(Quantity qty) const {
    if (state_.ask_depth==0){
        return state_.ask *1.10; // If no depth, execute at a worse price
    }
    if (qty <= state_.ask_depth) {
        return state_.ask;
    } else {
        double depth_ratio = static_cast<double>(qty) / state_.ask_depth;
        return state_.ask * (1.0 + (depth_ratio - 1.0) * 0.01);
    }
}

// Execute a trade
Fill SimpleMarketSimulator::execute(Quantity qty) {
    std::lock_guard<std::mutex> lock(market_mutex_);
    Price exec_price = calculate_execution_price(qty);
    
    
    auto now = std::chrono::duration_cast<Timestamp>(
        std::chrono::high_resolution_clock::now().time_since_epoch());
    
    
    
    // If no liquidity, execution fails (0 shares filled)
    if (state_.ask_depth == 0) {
        return Fill(next_order_id_++, 0, exec_price, now, std::chrono::nanoseconds(0));
    }
    
    // Reduce depth based on fill quantity
    Quantity filled = (qty <= state_.ask_depth) ? qty : state_.ask_depth;
    state_.ask_depth -= filled;
    
    return Fill(next_order_id_++, filled, exec_price, now, std::chrono::nanoseconds(100000));
}

void SimpleMarketSimulator::update_state(std::chrono::milliseconds dt) {
    std::this_thread::sleep_for(std::chrono::microseconds(500));
    double dt_years =dt.count()/1000.0/365.25/24.0/3600.0;
    double random_shock = current_volatility_* normal_dist_(rng_)*sqrt(dt_years);

    state_.mid_price *= (1.0 + random_shock);
    

    if(state_.ask_depth < original_depth_){
        state_.ask_depth +=50;
        if(state_.ask_depth >original_depth_){
            state_.ask_depth = original_depth_;
        }
    }

    double current_spread_bps = get_current_spread_bps();
    state_.bid = state_.mid_price * (1.0 - current_spread_bps/10000.0);
    state_.ask = state_.mid_price * (1.0 + current_spread_bps/10000.0);
}

void SimpleMarketSimulator::set_regime(MarketRegime regime){
    switch(regime){
        case MarketRegime::CALM:
            current_volatility_=0.10;
            break;
        case MarketRegime::VOLATILE:
            current_volatility_=0.40;
            break;
        case MarketRegime::EVENT_WEEK:
            current_volatility_=0.60;
            break;
        case MarketRegime::ILLIQUID:
            current_volatility_=0.30;
            original_depth_= original_depth_/2;
            break;
    }
}
void SimpleMarketSimulator::apply_event_shock(double volatility_multiplier,double price_shock){
    current_volatility_ *=volatility_multiplier;

    state_.mid_price *= (1.0 +price_shock);

    double current_spread_bps = get_current_spread_bps();
    state_.bid = state_.mid_price * (1.0 - current_spread_bps/10000.0);
    state_.ask = state_.mid_price * (1.0 + current_spread_bps/10000.0);
}

} // namespace aee