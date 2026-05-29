#include "aee/simulation/adaptive_scheduler.hpp"


namespace aee{
    AdaptiveScheduler::AdaptiveScheduler(Quantity normal_slice_size,Quantity reduced_slice_size,double spread_threshold_bps)
    :normal_slice_size_(normal_slice_size),reduced_slice_size_(reduced_slice_size),spread_threshold_bps_(spread_threshold_bps){

    }
    Quantity AdaptiveScheduler::next_slice(Quantity remaining,const MarketState& state){
        double current_spread_bps = (state.ask-state.bid) / state.mid_price *10000;

        Quantity slice_size;
        if(current_spread_bps > spread_threshold_bps_){
            slice_size=reduced_slice_size_;
        }
        else{
            slice_size=normal_slice_size_;
        }
        
        if(slice_size > remaining){
            return remaining;
        }
        return slice_size;
    }
    void AdaptiveScheduler::reset(){

    }

}