#ifndef ADAPTIVE_SCHEDULER_HPP
#define ADAPTIVE_SCHEDULER_HPP

#include "aee/interfaces/i_scheduler.hpp"

namespace aee{
    class AdaptiveScheduler : public IScheduler{
        public :
            AdaptiveScheduler(Quantity normal_slice_size, Quantity reduced_slice_size,double spread_threshold_bps);
            ~AdaptiveScheduler()=default;
            Quantity next_slice(Quantity remaining,const MarketState& state) override;    
            void reset() override;

        private:
            Quantity normal_slice_size_;
            Quantity reduced_slice_size_;
            double spread_threshold_bps_;
            //u have to write like this when they aasked u to write or store variables like this 

    };
}
#endif