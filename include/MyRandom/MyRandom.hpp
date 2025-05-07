//
// Created by Alex Ru on 4/14/25.
//

#ifndef MYRANDOM_HPP
#define MYRANDOM_HPP

#include <random>
#include <vector>
#include "Constants.hpp"

class MyRandom {
private:
    static inline thread_local std::random_device rd;
    static inline thread_local std::mt19937 gen{rd()};
public:
    static std::vector<std::string> generateUniformRandomRequests(const size_t num_requests, const std::vector<std::string>& options) {
        std::vector<std::string> requests;
        requests.reserve(num_requests);

        std::uniform_int_distribution<size_t> dist(0, options.size() - 1);

        for (size_t i = 0; i < num_requests; ++i) {
            requests.push_back(options[dist(gen)]);
        }
        return requests;
    }

    static std::vector<std::string> generateZipfRandomRequests(const size_t num_requests, const std::vector<std::string>& options) {
        std::vector<std::string> requests;
        requests.reserve(num_requests);

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

    static std::vector<std::string> generateRandomRequests(const size_t num_requests, const std::string& api, const std::string& random_type) {
        const std::vector<std::string> options = api == "weather" ? CITIES : STOCKS;
        if (random_type == "uniform") {
            return generateUniformRandomRequests(num_requests, options);
        }
        else if (random_type == "zipf") {
            return generateZipfRandomRequests(num_requests, options);
        }
        else {
            return std::vector<std::string>();
        }
    }
};

#endif //MYRANDOM_HPP