//
// Created by Alex Ru on 4/12/25.
//

#ifndef PROXY_HPP
#define PROXY_HPP

#include <Cache/Cache.h>
#include <chrono>
#include <iomanip>
#include <random>
#include <httplib.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

template<typename Key, typename Value>
class Proxy {
protected:
    std::unique_ptr<Cache<Key, Value>> cache;
    size_t hits = 0;
    size_t misses = 0;
    virtual Value runPrivateQuery(const Key& key) = 0;
public:
    Proxy() = default;

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

        using Clock = std::chrono::high_resolution_clock;
        using Duration = std::chrono::duration<double>;

        std::vector<double> latencies;
        latencies.reserve(requests.size());

        const auto start_time = Clock::now();

        for (size_t i = 0; i < requests.size(); ++i) {
            const auto req_start = Clock::now();
            query(requests[i]);
            const auto req_end = Clock::now();

            double latency = std::chrono::duration_cast<Duration>(req_end - req_start).count();
            latencies.push_back(latency);

            double avg_latency = std::accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size();
            double throughput = (i + 1) / std::chrono::duration_cast<Duration>(req_end - start_time).count();
            double hit_rate = 100.0 * hits / (i + 1);

            // Clear previous line and print in-place
            std::cout << "\rRequests: " << (i + 1)
                      << " | Hit Rate: " << std::fixed << std::setprecision(2) << hit_rate << "%"
                      << " | Avg Latency: " << avg_latency * 1e3 << " ms"
                      << " | Throughput: " << throughput << " req/s"
                      << std::flush;
        }

        const auto end_time = Clock::now();
        const Duration total_duration = end_time - start_time;

        std::cout << "\n\n=== Final Benchmark Results ===\n"
                  << "Total Requests:    " << requests.size() << "\n"
                  << "Hit Rate:          " << std::fixed << std::setprecision(2)
                  << 100.0 * hits / requests.size() << "%\n"
                  << "Average Latency:   " << std::accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size() * 1e3 << " ms\n"
                  << "Throughput:        " << requests.size() / total_duration.count() << " req/s\n"
                  << "Total Duration:    " << total_duration.count() << " s\n"
                  << "Memory Usage:      " << cache->memoryUsed() << " bytes\n";
    }
};

#endif // PROXY_HPP
