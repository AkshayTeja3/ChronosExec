#ifndef AEE_CORE_TYPES_HPP
#define AEE_CORE_TYPES_HPP

#include <cstdint>
#include <string>
#include <chrono>
 
namespace aee {
    using OrderId = uint64_t;
    using Quantity = uint64_t;
    using Price = double;
    using Timestamp = std::chrono::nanoseconds;
    using Symbol=std::string;


enum class Side {
    BUY,
    SELL
};

enum class MarketRegime : uint8_t{
    CALM,
    VOLATILE,
    EVENT_WEEK,
    ILLIQUID
};

struct Fill{
    OrderId order_id;
    Quantity filled_quantity;
    Price execution_price;
    Timestamp execution_time;
    Timestamp latency_ns;

        Fill(OrderId oid, Quantity qty, Price price, Timestamp exec_time, Timestamp latency)
        : order_id(oid)
        , filled_quantity(qty)
        , execution_price(price)
        , execution_time(exec_time)
        , latency_ns(latency) {

        }

    double cost() const{
        return filled_quantity*execution_price;
    }
};
struct MarketState {
    Price mid_price;
    Price bid;          // best bid price
    Price ask;          // best ask price
    Quantity bid_depth;  // shares available at bid
    Quantity ask_depth;  // shares available at ask
    double volatility;   // annualized volatility estimate
    Timestamp timestamp;
    
    // Helper method
    double spread() const { return ask - bid; }
};
} // namespace aee
#endif //AEE_CORE_TYPES_HPP



