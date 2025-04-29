#include <Cache/FIFOCache.hpp>
#include <Cache/LFUCache.hpp>
#include <Cache/LRUCache.hpp>

#include "cliHelper.h"

#include <Proxy/WeatherProxy.hpp>
#include <Proxy/StocksProxy.hpp>

#include <MyRandom/MyRandom.hpp>

int main(int argc, char* argv[]) {
    int num_requests = 100, cache_size = 10, threads = 1;
    std::string api, cache_type, random_type;

    cliHelper::parseArguments(argc, argv, api, num_requests, cache_type, cache_size, threads, random_type);
    cliHelper::printBanner();

    std::unique_ptr<Cache<std::string, nlohmann::json>> cache;

    if (cache_type == "fifo") cache = std::make_unique<FIFOCache<std::string, nlohmann::json>>(cache_size);
    else if (cache_type == "lru") cache = std::make_unique<LRUCache<std::string, nlohmann::json>>(cache_size);
    else if (cache_type == "lfu") cache = std::make_unique<LFUCache<std::string, nlohmann::json>>(cache_size);

    std::unique_ptr<Proxy<std::string, nlohmann::json>> proxy;
    if (api == "weather") {
        proxy = std::make_unique<WeatherProxy>(std::move(cache));
    }
    else if (api == "stocks") {
        proxy = std::make_unique<StocksProxy>(std::move(cache));
    }

    const std::vector<std::string> requests = MyRandom::generateRandomRequests(num_requests, api, random_type);

    proxy->runBenchmark(requests);

    return 0;
}
