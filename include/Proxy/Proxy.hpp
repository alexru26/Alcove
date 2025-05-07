// Proxy class implementation
// Alex Ru
// 5/13/25

#ifndef PROXY_HPP
#define PROXY_HPP

#include <Cache/Cache.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <iomanip>
#include <random>
#include <httplib.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using Clock = std::chrono::high_resolution_clock;
using DoubleDuration = std::chrono::duration<double>;

/**
 * Proxy class that queries and pulls data based on cache
 * @tparam Key key value type
 * @tparam Value corresponding value type
 */
template<typename Key, typename Value>
class Proxy {
protected:
    std::unique_ptr<Cache<Key, Value>> cache; // Cache to use
    std::atomic<int> hits = 0; // Number of times data retrieved from cache
    std::atomic<int> misses = 0; // Number of times data retrieved from API

    /**
     * Abstract helper method that runs direct query from API
     * @param key key to query with
     * @return value returned by API
     */
    virtual Value runPrivateQuery(const Key& key) = 0;
public:
    /**
     * Default constructor
     */
    Proxy() = default;

    /**
     * Explicit constructor that takes in cache as parameter
     * @param cache unique pointer to cache
     */
    explicit Proxy(std::unique_ptr<Cache<Key, Value>> cache) : cache(std::move(cache)) {}

    /**
     * Default constructor
     */
    virtual ~Proxy() = default;

    /**
     * Retrieves corresponding value of key. First tries cache, then directly queries from API.
     * @param key key to query with
     * @return corresponding value
     */
    Value query(const Key& key) {
        if (cache->exists(key)) {
            ++hits;
            return cache->get(key);
        } else {
            ++misses;
            Value result = runPrivateQuery(key);
            cache->put(key, result);
            return result;
        }
    }

    /**
     * Perform requests with multi-threading, and report results
     * @param requests list of requests
     * @param num_threads number of threads to run requests
     */
    void runBenchmark(const std::vector<Key>& requests, const int num_threads) {
        hits = 0;
        misses = 0;
        std::atomic processed = 0;
        std::mutex latency_mutex;
        std::mutex cout_mutex;
        std::vector<double> latencies;
        latencies.reserve(requests.size());

        constexpr std::chrono::milliseconds print_interval(200);
        auto last_print_time = Clock::now();
        std::mutex last_print_mutex;

        const auto start_time = Clock::now();

        auto worker = [&](size_t start_idx, size_t end_idx) {
            for (size_t i = start_idx; i < end_idx; ++i) {
                const auto req_start = Clock::now();
                query(requests[i]);
                const auto req_end = Clock::now();

                double latency = std::chrono::duration_cast<DoubleDuration>(req_end - req_start).count();

                {
                    std::lock_guard lock(latency_mutex);
                    latencies.push_back(latency);
                }

                size_t current = ++processed;

                bool print = false;
                const auto now = Clock::now();
                {
                    std::lock_guard last_print_lock(last_print_mutex);
                    if (now - last_print_time >= print_interval || current == requests.size()) {
                        last_print_time = now;
                        print = true;
                    }
                }

                if (print) {
                    double avg_latency;
                    {
                        std::lock_guard latency_lock(latency_mutex);
                        avg_latency = std::accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size();
                    }

                    double throughput = current / std::chrono::duration_cast<DoubleDuration>(now - start_time).count();
                    double hit_rate = 100.0 * hits / current;

                    std::lock_guard lock(cout_mutex);
                    std::cout << "\rRequests: " << current
                              << " | Hit Rate: " << std::fixed << std::setprecision(2) << hit_rate << "%"
                              << " | Avg Latency: " << avg_latency * 1e3 << " ms"
                              << " | Throughput: " << throughput << " req/s"
                              << std::flush;
                }
            }
        };

        std::vector<std::thread> threads;
        size_t chunk_size = (requests.size() + num_threads - 1) / num_threads;

        for (int t = 0; t < num_threads; ++t) {
            size_t start_idx = t * chunk_size;
            size_t end_idx = std::min(start_idx + chunk_size, requests.size());
            threads.emplace_back(worker, start_idx, end_idx);
        }

        for (auto& thread : threads) {
            thread.join();
        }

        const auto end_time = Clock::now();
        const DoubleDuration total_duration = end_time - start_time;

        double avg_latency = std::accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size();
        double throughput = requests.size() / total_duration.count();
        double hit_rate = 100.0 * hits / requests.size();

        std::cout << "\n\n=== Final Benchmark Results ===\n"
                  << "Total Requests:    " << requests.size() << "\n"
                  << "Hit Rate:          " << std::fixed << std::setprecision(2) << hit_rate << "%\n"
                  << "Average Latency:   " << avg_latency * 1e3 << " ms\n"
                  << "Throughput:        " << throughput << " req/s\n"
                  << "Total Duration:    " << total_duration.count() << " s\n"
                  << "Memory Usage:      " << cache->memoryUsed() << " bytes\n";
    }
};

#endif // PROXY_HPP
