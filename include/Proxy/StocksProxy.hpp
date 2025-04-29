//
// Created by Alex Ru on 4/24/25.
//

#ifndef STOCKSPROXY_H
#define STOCKSPROXY_H

#include "Proxy.hpp"

class StocksProxy final : public Proxy<std::string, json> {
private:
    static std::string get_api_key() {
        const char* key = std::getenv("STOCKS_API_KEY");
        if (!key) throw std::runtime_error("API key not found in environment!");
        return {key};
    }

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
            throw std::runtime_error("Stock API error: " + res->body);
        }
        throw std::runtime_error("Connection failed");
    }

public:
    explicit StocksProxy(std::unique_ptr<Cache<std::string, nlohmann::json>> cache) : Proxy(std::move(cache)) {}

    ~StocksProxy() override = default;

    static std::vector<std::string> getRandomRequests(const std::string& type, const size_t num_requests) {
        static const std::vector<std::string> STOCKS = {
            "AAPL",   // Apple Inc.
            "MSFT",   // Microsoft Corporation
            "GOOGL",  // Alphabet Inc. (Class A)
            "AMZN",   // Amazon.com Inc.
            "TSLA",   // Tesla Inc.
            "META",   // Meta Platforms Inc.
            "NVDA",   // NVIDIA Corporation
            "JPM",    // JPMorgan Chase & Co.
            "JNJ",    // Johnson & Johnson
            "V",      // Visa Inc.
            "PG",     // Procter & Gamble Co.
            "UNH",    // UnitedHealth Group Inc.
            "HD",     // Home Depot Inc.
            "MA",     // Mastercard Inc.
            "DIS",    // The Walt Disney Company
            "BAC",    // Bank of America Corporation
            "PEP",    // PepsiCo Inc.
            "KO",     // The Coca-Cola Company
            "XOM",    // Exxon Mobil Corporation
            "NFLX"    // Netflix Inc.
        };

        return MyRandom<std::string>::generateRandomRequests(type, num_requests, STOCKS);
    }
};

#endif // STOCKSPROXY_H