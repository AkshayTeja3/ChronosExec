#include "aee/core/order.hpp"
#include <stdexcept>
#include <iostream>

namespace aee {

// Constructor
Order::Order(OrderId id, Side side, Quantity quantity,Price price, Price limit_price)
    : id_(id)
    , side_(side)
    , initial_quantity_(quantity)
    , remaining_(quantity)
    , limit_price_(limit_price)
    , created_at_(std::chrono::duration_cast<Timestamp>(
          std::chrono::high_resolution_clock::now().time_since_epoch()))
    , fills_() {
}

// fill method
void Order::fill(Quantity qty, Price price, Timestamp exec_time, Timestamp latency) {
    if (qty <= remaining_) {
        Fill new_fill(id_, qty, price, exec_time, latency);
        fills_.push_back(new_fill);
        remaining_ -= qty;
    } else {
        throw std::runtime_error("Fill quantity exceeds remaining quantity");
    }
}

// average_price method
double Order::average_price() const {
    if (fills_.empty()) {
        return 0.0;
    }
    
    double total_cost = 0.0;
    Quantity total_qty = 0;
    
    for (const auto& fill : fills_) {
        total_cost += static_cast<double>(fill.filled_quantity) * fill.execution_price;
        total_qty += fill.filled_quantity;
    }
    
    return total_cost / static_cast<double>(total_qty);
}

} // namespace aee