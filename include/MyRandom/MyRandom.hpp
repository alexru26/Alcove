//
// Created by Alex Ru on 4/14/25.
//

#ifndef MYRANDOM_HPP
#define MYRANDOM_HPP

#include <random>
#include <vector>

class MyRandom {
private:
    static inline thread_local std::random_device rd;
    static inline thread_local std::mt19937 gen{rd()};

    inline static const std::vector<std::string> cities = {
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

    inline static const std::vector<std::string> stocks = {
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

public:
    static std::vector<std::string> generateUniformRandomRequests(size_t num_requests, std::string& api) {
        std::vector<std::string> requests;
        requests.reserve(num_requests);

        const std::vector<std::string> options = api == "weather" ? cities : stocks;

        std::uniform_int_distribution<size_t> dist(0, options.size() - 1);

        for (size_t i = 0; i < num_requests; ++i) {
            requests.push_back(options[dist(gen)]);
        }
        return requests;
    }

    static std::vector<std::string> generateZipfRandomRequests(size_t num_requests, std::string& api) {
        std::vector<std::string> requests;
        requests.reserve(num_requests);

        const std::vector<std::string> options = api == "weather" ? cities : stocks;

        const double s = 1.0;
        std::vector<double> weights;
        weights.reserve(options.size());

        // Calculate Zipf weights
        for (size_t i = 0; i < options.size(); ++i) {
            weights.push_back(1.0 / std::pow(i + 1, s));
        }

        // Create distribution
        std::discrete_distribution<size_t> dist(weights.begin(), weights.end());

        // Generate requests
        for (size_t i = 0; i < num_requests; ++i) {
            requests.push_back(options[dist(gen)]);
        }
        return requests;
    }

    static std::vector<std::string> generateRandomRequests(size_t num_requests, std::string& api, std::string& random_type) {
        if (random_type == "uniform") {
            return generateUniformRandomRequests(num_requests, api);
        }
        else if (random_type == "zipf") {
            return generateZipfRandomRequests(num_requests, api);
        }
        else {
            return std::vector<std::string>();
        }
    }
};

#endif //MYRANDOM_HPP