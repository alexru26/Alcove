//
// Created by Alex Ru on 5/7/25.
//

#include <Cache/FIFOCache.hpp>
#include <Proxy/StocksProxy.hpp>
#include <gtest/gtest.h>
#include <MyRandom/Constants.hpp>

TEST(StocksProxyTest, KeysQuery) {
    auto cache = std::make_unique<FIFOCache<std::string, nlohmann::json>>(0);
    const auto proxy = std::make_unique<StocksProxy>(std::move(cache));

    for (const std::string& key : STOCKS) {
        proxy->query(key);
    }
}