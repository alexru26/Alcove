// Stocks proxy implementation
// Alex Ru
// 5/13/25

#ifndef STOCKSPROXY_H
#define STOCKSPROXY_H

#include "Proxy.hpp"

/**
 * Stocks proxy class that pulls from stocks API
 */
class StocksProxy final : public Proxy<std::string, json> {
private:
    /**
     * Private helper function that gets the API key
     * @return api key
     */
    static std::string get_api_key() {
        const char* key = std::getenv("STOCKS_API_KEY");
        if (!key) throw std::runtime_error("API key not found in environment!");
        return {key};
    }

    /**
     * Runs a direct query from stocks API
     * @param key key to query with
     * @return value returned by stocks API
     */
    json runPrivateQuery(const std::string& key) override {
        httplib::Client cli("https://www.alphavantage.co");
        const std::string encoded_key = httplib::detail::encode_url(key);
        const std::string path = "/query?function=TIME_SERIES_DAILY&symbol="
                               + encoded_key
                               + "&apikey="
                               + get_api_key();

        if (auto res = cli.Get(path)) {
            if (res->status == 200) {
                try {
                    return json::parse(res->body);
                } catch (const json::parse_error& e) {
                    throw std::runtime_error("JSON parse error: " + std::string(e.what()));
                }
            }
            throw std::runtime_error("Stock API error: " + res->body + " Key: " + encoded_key);
        }
        throw std::runtime_error("Connection failed");
    }

public:
    /**
     * Explicit constructor that takes in cache as parameter
     * @param cache unique pointer to cache
     */
    explicit StocksProxy(std::unique_ptr<Cache<std::string, nlohmann::json>> cache) : Proxy(std::move(cache)) {}

    /**
     * Default destructor
     */
    ~StocksProxy() override = default;
};

#endif // STOCKSPROXY_H