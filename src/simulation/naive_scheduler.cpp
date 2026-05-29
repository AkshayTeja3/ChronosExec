#include "aee/simulation/naive_scheduler.hpp"

namespace aee{
    NaiveScheduler::NaiveScheduler(Quantity slice_size,std::chrono::milliseconds interval)
        
    :slice_size_(slice_size), interval_(interval){
        //u can do like this too slice_size_slice_size, interval_=interval
        //when referencing constructors or methods,use their class before their name just like in this case NaiveScheduler::NaiveScheduler, NaiveScheduler::next_slice, NaiveScheduler::reset

    }
    Quantity NaiveScheduler::next_slice(Quantity remaining, const MarketState& state){
        if(remaining >= slice_size_){
            return slice_size_;
        }
        else{
            return remaining;
        }

    }
    void NaiveScheduler::reset(){

    }
}