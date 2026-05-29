#include "aee/core/thread_pool.hpp"
#include <iostream>
#include <chrono>

int main(){
    aee::ThreadPool pool(4);

    std::cout<< "Submitting 10 tasks..."<<std::endl;

    std::vector<std::future<int>> results;

    for(int i=0; i<10;i++){
        results.push_back(pool.submit([i]{
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return i*i;
        }));

    }
    for(auto& result:results){
        std::cout<< "Results: "<<result.get()<<std::endl;

    }

    std::cout<<"All tasks complete!"<< std::endl;
    return 0;
}
