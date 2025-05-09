// Weather proxy implementation
// Alex Ru
// 5/13/25

#ifndef WEATHER_PROXY_HPP
#define WEATHER_PROXY_HPP

#include "Proxy.hpp"

/**
 * Weather proxy class that pulls from weather API
 */
class WeatherProxy final : public Proxy<std::string, json> {
private:
    /**
     * Private helper function that gets the API key
     * @return api key
     */
    static std::string get_api_key() {
        const char* key = std::getenv("WEATHER_API_KEY");
        if (!key) throw std::runtime_error("API key not found in environment!");
        return {key};
    }

    /**
     * Runs a direct query from weather API
     * @param key key to query with
     * @return value returned by weather API
     */
    json runPrivateQuery(const std::string& key) override {
        // Create and parse the path
        httplib::Client cli("https://weather.visualcrossing.com");
        const std::string encoded_key = httplib::detail::encode_url(key);
        const std::string path = "/VisualCrossingWebServices/rest/services/timeline/"
                               + encoded_key
                               + "/today?key="
                               + get_api_key();

        // Do call
        if (auto res = cli.Get(path)) {
            if (res->status == 200) {
                try {
                    return json::parse(res->body);
                } catch (const json::parse_error& e) {
                    throw std::runtime_error("JSON parse error: " + std::string(e.what()));
                }
            }
            throw std::runtime_error("Weather API error: " + res->body + " Key: " + encoded_key);
        }
        throw std::runtime_error("Connection failed");
    }

public:
    /**
     * Explicit constructor that takes in cache as parameter
     * @param cache unique pointer to cache
     */
    explicit WeatherProxy(std::unique_ptr<Cache<std::string, nlohmann::json>> cache) : Proxy(std::move(cache)) {}

    /**
     * Default destructor
     */
    ~WeatherProxy() override = default;
};

#endif // WEATHER_PROXY_HPP
