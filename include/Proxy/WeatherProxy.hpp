//
// Created by Alex Ru on 4/12/25.
//

#ifndef WEATHER_PROXY_HPP
#define WEATHER_PROXY_HPP

#include "Proxy.hpp"
#include <cstdlib>
#include <httplib.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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
    using Proxy::Proxy;

    ~WeatherProxy() override = default;

    static std::vector<std::string> generateRandomRequests(const size_t num_requests) {
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

        // Set the exponent parameter. Higher s makes the distribution even more skewed.
        const double s = 1.0;
        std::vector<double> weights;
        weights.reserve(CITIES.size());

        // Calculate weights based on Zipf's law: weight ∝ 1/(rank^s)
        for (size_t i = 0; i < CITIES.size(); ++i) {
            weights.push_back(1.0 / std::pow(static_cast<double>(i + 1), s));
        }

        // Reserve enough space for the random requests.
        std::vector<std::string> requests;
        requests.reserve(num_requests);

        // Initialize random number generator.
        std::mt19937 rng(std::random_device{}());

        // Create a discrete distribution with the provided weights.
        std::discrete_distribution<size_t> dist(weights.begin(), weights.end());

        // Generate the requests.
        for (size_t i = 0; i < num_requests; ++i) {
            size_t index = dist(rng);
            requests.push_back(CITIES[index]);
        }

        return requests;
    }
};

#endif // WEATHER_PROXY_HPP
