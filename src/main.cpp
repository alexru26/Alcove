#include <Cache/FIFOCache.hpp>
#include <Cache/LFUCache.hpp>
#include <Cache/LRUCache.hpp>

#include <Proxy/WeatherProxy.hpp>
#include "Proxy/StocksProxy.hpp"

int main() {
    using proxy = StocksProxy;
    int cache_size = 10;
    constexpr int num_requests = 10;

    auto fifo = std::make_unique<FIFOCache<std::string, nlohmann::json>>(cache_size);
    auto lru = std::make_unique<LRUCache<std::string, nlohmann::json>>(cache_size);
    auto lfu = std::make_unique<LFUCache<std::string, nlohmann::json>>(cache_size);

    proxy fifo_proxy(std::move(fifo));
    proxy lru_proxy(std::move(lru));
    proxy lfu_proxy(std::move(lfu));

    const std::vector<std::string> requests = proxy::getRandomRequests("zipf", num_requests);

    fifo_proxy.runBenchmark(requests);
    lru_proxy.runBenchmark(requests);
    lfu_proxy.runBenchmark(requests);

    return 0;
}
