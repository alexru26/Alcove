//
// Created by Alex Ru on 5/7/25.
//

#include <Cache/FIFOCache.hpp>
#include <Proxy/WeatherProxy.hpp>
#include <gtest/gtest.h>
#include <MyRandom/Constants.hpp>

auto cache = std::make_unique<FIFOCache<std::string, nlohmann::json>>(10);
auto proxy = std::make_unique<WeatherProxy>(std::move(cache));

TEST(WeatherProxyTest, KeysQuery) {
    for (const std::string& key : CITIES) {
        proxy->query(key);
    }
}