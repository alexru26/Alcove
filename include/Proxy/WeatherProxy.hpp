//
// Created by Alex Ru on 4/12/25.
//

#ifndef WEATHER_PROXY_HPP
#define WEATHER_PROXY_HPP

#include "Proxy.hpp"

class WeatherProxy final : public Proxy<std::string, json> {
private:
    static std::string get_api_key() {
        const char* key = std::getenv("WEATHER_API_KEY");
        if (!key) throw std::runtime_error("API key not found in environment!");
        return {key};
    }

    json runPrivateQuery(const std::string& key) override {
        httplib::Client cli("https://weather.visualcrossing.com");
        const std::string encoded_key = httplib::detail::encode_url(key);
        const std::string path = "/VisualCrossingWebServices/rest/services/timeline/"
                               + encoded_key
                               + "/today?key="
                               + get_api_key();

        if (auto res = cli.Get(path)) {
            if (res->status == 200) {
                try {
                    return json::parse(res->body);
                } catch (const json::parse_error& e) {
                    throw std::runtime_error("JSON parse error: " + std::string(e.what()));
                }
            }
            throw std::runtime_error("Weather API error: " + res->body);
        }
        throw std::runtime_error("Connection failed");
    }

public:
    explicit WeatherProxy(std::unique_ptr<Cache<std::string, nlohmann::json>> cache) : Proxy(std::move(cache)) {}

    ~WeatherProxy() override = default;

    static std::vector<std::string> getRandomRequests(const std::string& type, const size_t num_requests) {
        static const std::vector<std::string> CITIES = {
            "Tokyo",      // rank 1
            "New York",   // rank 2
            "London",     // rank 3
            "Paris",      // rank 4
            "Los Angeles",// rank 5
            "Beijing",    // rank 6
            "Shanghai",   // rank 7
            "Moscow",     // rank 8
            "São Paulo",  // rank 9
            "Mumbai",     // rank 10
            "Berlin",     // rank 11
            "Dubai",      // rank 12
            "Singapore",  // rank 13
            "Rome",       // rank 14
            "Madrid",     // rank 15
            "Istanbul",   // rank 16
            "Seoul",      // rank 17
            "Toronto",    // rank 18
            "Sydney",     // rank 19
            "Mexico City" // rank 20
        };

        return MyRandom<std::string>::generateRandomRequests(type, num_requests, CITIES);
    }
};

#endif // WEATHER_PROXY_HPP
