//
// Created by Alex Ru on 5/7/25.
//

#include <Cache/FIFOCache.hpp>
#include <Proxy/StocksProxy.hpp>
#include <gtest/gtest.h>
#include <MyRandom/Constants.hpp>

auto cache = std::make_unique<FIFOCache<std::string, nlohmann::json>>(10);
auto proxy = std::make_unique<StocksProxy>(std::move(cache));

TEST(StocksProxyTest, KeysQuery) {
    for (const std::string& key : STOCKS) {
        proxy->query(key);
    }
}