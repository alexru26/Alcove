#include <Cache/FIFOCache.hpp>
#include <Cache/LFUCache.hpp>
#include <Cache/LRUCache.hpp>

#include <Proxy/WeatherProxy.hpp>

int main() {
    auto fifo = std::make_unique<FIFOCache<std::string, nlohmann::json>>(10);
    auto lru = std::make_unique<LRUCache<std::string, nlohmann::json>>(10);
    auto lfu = std::make_unique<LFUCache<std::string, nlohmann::json>>(10);

    WeatherProxy fifo_proxy(std::move(fifo));
    WeatherProxy lru_proxy(std::move(lru));
    WeatherProxy lfu_proxy(std::move(lfu));

    const std::vector<std::string> requests = WeatherProxy::generateRandomRequests(100);

    fifo_proxy.runBenchmark(requests);
    lru_proxy.runBenchmark(requests);
    lfu_proxy.runBenchmark(requests);

    return 0;
}
