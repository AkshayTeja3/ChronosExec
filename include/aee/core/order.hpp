#ifndef AEE_CORE_ORDER_HPP
#define AEE_CORE_ORDER_HPP

#include "types.hpp"
#include<vector>

namespace aee{

    class Order{
        public:
            Order(OrderId id,Side side, Quantity qty, Price price, Price limit_price=0.0);
            OrderId id() const{ return id_;}
            Side side() const{ return side_;}
            Quantity remaining() const{return remaining_;}
            Quantity filled() const{ return initial_quantity_-remaining_;}
            bool is_filled() const{ return remaining_==0;}
            bool is_buy() const{ return side_==Side::BUY;}
            bool is_sell() const{ return side_==Side::SELL;}

            void fill(Quantity qty, Price price, Timestamp exec_time, Timestamp latency);

            double average_price() const;
    
            const std::vector<Fill>& fills() const { return fills_; }
        private:
            OrderId id_;
            Side side_;
            Quantity initial_quantity_;
            Quantity remaining_;      // what's left to fill
            Price limit_price_;       // 0.0 = market order
            Timestamp created_at_;
            std::vector<Fill> fills_; // history of executions
            

    };


}
#endif //AEE_CORE_ORDER_HPP