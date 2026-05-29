#ifndef SYMBOL_REGISTRY_HPP
#define SYMBOL_REGISTRY_HPP

#include "aee/simulation/simple_market_simulator.hpp"
#include <unordered_map>
#include <memory>
#include <mutex>

namespace aee {

class SymbolRegistry {
public:
    static SymbolRegistry& instance() {
        static SymbolRegistry registry;
        return registry;
    }
    
    // Get or create market for a symbol
    std::shared_ptr<SimpleMarketSimulator> get_market(const Symbol& symbol) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        auto it = markets_.find(symbol);
        if (it != markets_.end()) {
            return it->second;
        }
        
        // Create new market for this symbol
        auto market = std::make_shared<SimpleMarketSimulator>(symbol, 100.0, 3000, 10.0);
        markets_[symbol] = market;
        return market;
    }
    
private:
    SymbolRegistry() = default;
    std::unordered_map<Symbol, std::shared_ptr<SimpleMarketSimulator>> markets_;
    std::mutex mutex_;
};

} // namespace aee

#endif