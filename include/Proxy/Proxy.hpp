//
// Created by Alex Ru on 4/12/25.
//

#ifndef PROXY_HPP
#define PROXY_HPP

#include <Cache/Cache.h>
#include <chrono>
#include <iomanip>
#include <random>

template<typename Key, typename Value>
class Proxy {
protected:
    std::unique_ptr<Cache<Key, Value>> cache;
    size_t hits = 0;
    size_t misses = 0;
    virtual Value runPrivateQuery(const Key& key) = 0;
public:
    explicit Proxy(std::unique_ptr<Cache<Key, Value>> cache) : cache(std::move(cache)) {}

    virtual ~Proxy() = default;

    Value query(const Key& key) {
        if (cache->exists(key)) {
            hits++;
            return cache->get(key);
        } else {
            misses++;
            Value result = runPrivateQuery(key);
            cache->put(key, result);
            return result;
        }
    }

    void runBenchmark(const std::vector<Key>& requests) {
        hits = 0;
        misses = 0;

        // Timing variables
        using Clock = std::chrono::high_resolution_clock;
        using Duration = std::chrono::duration<double>;

        // Start timing
        const auto start_time = Clock::now();

        // Track individual request latencies
        std::vector<double> latencies;
        latencies.reserve(requests.size());

        for (const Key& request : requests) {
            const auto req_start = Clock::now();
            query(request); // Actual cache/database interaction
            const auto req_end = Clock::now();

            latencies.push_back(
                std::chrono::duration_cast<Duration>(req_end - req_start).count()
            );
        }

        // Stop timing
        const auto end_time = Clock::now();
        const Duration total_duration = end_time - start_time;

        // Calculate metrics
        const double avg_latency = std::accumulate(latencies.begin(), latencies.end(), 0.0)
                                 / latencies.size();
        const double throughput = requests.size() / total_duration.count();

        // Print formatted results
        std::cout << "\n=== Benchmark Results ===\n"
                  << "Total Requests:    " << requests.size() << "\n"
                  << "Hit Rate:          " << std::fixed << std::setprecision(2)
                  << 100.0 * hits / requests.size() << "%\n"
                  << "Average Latency:   " << avg_latency * 1e3 << " ms\n"
                  << "Throughput:        " << throughput << " req/s\n"
                  << "Total Duration:    " << total_duration.count() << "s\n"
                  << "Memory Usage:      " << cache->memoryUsed() << " bytes\n";
    }
};

#endif // PROXY_HPP
