[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://isocpp.org/)
[![Speedup](https://img.shields.io/badge/Speedup-4.03x-brightgreen.svg)]()
[![Cost Reduction](https://img.shields.io/badge/Cost%20Reduction-64%25-red.svg)]()

# 🚀 ChronosExec — Adaptive Order Execution Engine

*Named for Chronos — the personification of time in Greek mythology. Because in execution, timing is everything.*

---

## Executive Summary

This engine solves a classic trading problem: **executing large orders without destroying your own price.**

Imagine buying 50,000 apples. Buy all at once → sellers raise prices. Buy slowly → price might move against you. My engine finds the sweet spot by **adapting to market conditions** — watching spread, depth, and volatility — then slicing orders intelligently.

**Result:** 64% lower execution cost than naive strategies, running 4x faster using all CPU cores.

---

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                      CLIENT / CONFIG                        │
└─────────────────────────┬───────────────────────────────────┘
                          │
                          ▼
┌─────────────────────────────────────────────────────────────┐
│                   SIMULATION LOOP                           │
│         (Orchestrator — runs until order fills)             │
└─────────┬──────────────────────────────┬────────────────────┘
          │                              │
          ▼                              ▼
┌─────────────────────┐        ┌─────────────────────────────┐
│    INTERFACES       │        │       IMPLEMENTATIONS        │
│                     │        │                             │
│ IMarketSimulator    │◄───────│ SimpleMarketSimulator       │
│ IScheduler          │◄───────│ NaiveScheduler              │
│                     │        │ AdaptiveScheduler           │
└─────────────────────┘        └─────────────────────────────┘
                                          │
                                          ▼
                              ┌─────────────────────────────┐
                              │       THREAD POOL            │
                              │   (4 workers · std::thread)  │
                              └─────────────────────────────┘
```

### Layer Breakdown

| Layer | Components | Responsibility |
|-------|------------|----------------|
| **Core** | `Order`, `Fill`, `MarketState`, `ThreadPool` | Types, threading utilities |
| **Interfaces** | `IMarketSimulator`, `IScheduler` | Pure virtual contracts (pluggable) |
| **Simulation** | `SimpleMarketSimulator`, `NaiveScheduler`, `AdaptiveScheduler` | Concrete implementations |
| **Source** | All `.cpp` files | Implementation of declared headers |

---

## Tech Stack — Why C++?

| Requirement | C++ Solution |
|-------------|---------------|
| Low latency | No GC pauses, deterministic performance |
| Memory control | RAII, smart pointers, manual management |
| True parallelism | `std::thread`, `std::mutex`, `std::condition_variable` |
| Zero-cost abstractions | Virtual functions compile to vtable lookups |

**Could I have used Python?** Yes, for prototyping. But the 4.03x speedup and real-time simulation require C++.

---

## Key Innovation: Adaptive Scheduler

### The Problem

Naive scheduler: **500 shares every 100ms** — regardless of market conditions.

When spread widens (market becomes expensive), naive keeps buying 500 shares → pays premium prices.

### The Solution

My adaptive scheduler monitors **spread in basis points**:

```
spread_bps = (ask - bid) / mid_price * 10000
```

**Trigger:** Spread > 15 bps (configurable threshold)

**Response:** Slice size drops from 500 → 250 shares

### The Result

| Metric | Naive | Adaptive | Improvement |
|--------|-------|----------|-------------|
| Implementation Shortfall | $13,263 | $4,751 | **64% lower** |
| Average Execution Price | $102.75 | $101.05 | **$1.70 better** |
| Slices executed | 41 | 41 | Same granularity |

**Why adaptive wins:** Smaller slices prevent price spikes when liquidity is thin.

---

## Performance: 4.03x Parallel Speedup

### Thread Pool Architecture

```cpp
class ThreadPool {
    std::vector<std::thread> workers_;     // 4 workers
    std::queue<std::function<void()>> tasks_;  // Task queue
    std::mutex queue_mutex_;               // Safe queue access
    std::condition_variable condition_;    // Wake sleeping workers
};
```

### Benchmark Setup

- **4 orders × 200,000 shares each**
- **Depth:** 50,000 shares (sufficient for all orders)
- **Each order runs independently** (no shared market state → no mutex contention)
- **500μs sleep** per `update_state()` to simulate real processing

### Results

| Mode | Time | Speedup |
|------|------|---------|
| Sequential (1 thread) | 7.56 seconds | 1.00x |
| Parallel (4 threads) | 1.87 seconds | **4.03x** |

**Near-perfect scaling** — my implementation has zero bottleneck.

---

## Market Impact Model

### Depth Consumption

```
Depth: 3000 shares at ask = $100.10

Slice 1: Buy 500 → depth = 2500, price = $100.10
Slice 2: Buy 500 → depth = 2000, price = $100.10
Slice 3: Buy 500 → depth = 1500, price = $100.10
Slice 4: Buy 500 → depth = 1000, price = $100.10
Slice 5: Buy 500 → depth = 500,  price = $100.10
Slice 6: Buy 500 → depth = 0,    price = $110.11 (10% jump)
```

### Spread Widening

| Remaining Depth | Spread Multiplier |
|----------------|-------------------|
| > 50% | 1.0x (10 bps) |
| 20% - 50% | 2.0x (20 bps) |
| < 20% | 3.0x (30 bps) |

### Depth Replenishment

- **50 shares returned** per 100ms time step (models new limit orders)
- Prevents permanent lock-up after depth exhaustion

### Volatility

- Random price drift using `std::normal_distribution`
- Regimes: `CALM` (10% vol), `VOLATILE` (40% vol), `EVENT_WEEK` (60% vol)
- Event shocks: temporary volatility spikes + price jumps

---

## Testing & Validation

### Metrics Tracked

```
Implementation Shortfall = (Actual Avg Price - Arrival Price) × Quantity
```

| Metric | What It Measures |
|--------|------------------|
| Shortfall | Total cost over expected |
| Slices executed | Granularity of execution |
| Average price | Effective fill quality |
| Total time | Sequential vs parallel |

### Test Methodology

1. Run naive scheduler on 5000-share order
2. Run adaptive scheduler on same parameters
3. Compare shortfall
4. Run both in thread pool, measure speedup
5. Repeat across volatility regimes

### Validation Results

| Test Case | Naive Shortfall | Adaptive Shortfall | Improvement |
|-----------|-----------------|--------------------|-------------|
| Normal market | $13,263 | $4,751 | **64%** |
| Volatile regime | $4,772 | $4,765 | 0.1% (statistically tied) |
| Low depth (3000) | $13,263 | $4,751 | **64%** |

---

## Future Work

With another week, I would add:

| Feature | Why |
|---------|-----|
| **Python visualization** | Plot price evolution, spread widening, slice timing (matplotlib) |
| **5+ level order book** | Realistic liquidity beyond top of book |
| **Monte Carlo simulation** | 1000 runs → mean/variance of shortfall |
| **Multi-symbol support** | Independent markets (AAPL, TSLA, GOOGL) |
| **Real market data backtest** | Validate against historical tick data |

---

## War Stories: The Hardest Bugs

### Bug #1: Infinite Loop (`inf`)

**Symptom:** Simulation never terminated, kept printing `inf`.

**Cause:** `calculate_execution_price()` divided by zero when `ask_depth == 0`.

**Fix:** Check `if (ask_depth == 0) return ask * 1.10` (10% price jump).

---

### Bug #2: `inf` Shortfall

**Symptom:** `Implementation shortfall: inf`

**Cause:** `average_price()` returned `inf` because `total_qty` was 0 — fills weren't recorded.

**Fix:** Ensure `order.fill()` called with correct parameters before calculating metrics.

---

### Bug #3: Parallel Output Corruption

**Symptom:** Terminal output looked like: `=== SIMULATIO=== SIMULATION REPORTN REPORT===`

**Cause:** 4 threads printing to `std::cout` simultaneously.

**Fix:** Added `std::mutex cout_mutex` with `lock_guard` around all output.

---

### Bug #4: No Actual Parallelism

**Symptom:** Speedup was 0.43x (parallel slower than sequential).

**Cause:** All orders shared the same market instance → mutex serialized everything.

**Fix:** Give each order its own market copy (no shared state = no contention).

---

## How to Build & Run

```bash
git clone https://github.com/AkshayTeja3/ChronosExec.git
cd ChronosExec
mkdir build && cd build
cmake ..
make

# Run main simulation
./aee_sim

# Run benchmark (sequential vs parallel)
./benchmark
```

### Requirements

- C++20 compiler (g++ 13+ or clang 16+)
- CMake 3.20+
- Ubuntu 22.04+ / WSL

---

## Results Summary

| Achievement | Number |
|-------------|--------|
| Lines of code | ~1200 |
| Parallel speedup | **4.03x** (near-perfect scaling) |
| Cost reduction | **64%** (adaptive vs naive) |
| Thread pool workers | 4 |
| Max order size | 200,000 shares |
| Market regimes | 4 (Calm, Volatile, Event Week, Illiquid) |

---

## Author

Built by [Akshay Teja]
```
