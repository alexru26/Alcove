// Tests for weather proxy
// Alex Ru
// 5/13/25

#include <Cache/FIFOCache.hpp>
#include <Proxy/WeatherProxy.hpp>
#include <gtest/gtest.h>
#include <MyRandom/Constants.hpp>

TEST(WeatherProxyTest, KeysQuery) {
    auto cache = std::make_unique<FIFOCache<std::string, nlohmann::json>>(0);
    const auto proxy = std::make_unique<WeatherProxy>(std::move(cache));

    for (const std::string& key : CITIES) {
        proxy->query(key);
    }
}