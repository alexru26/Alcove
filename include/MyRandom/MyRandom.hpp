// MyRandom class implementation
// Alex Ru
// 5/13/25

#ifndef MYRANDOM_HPP
#define MYRANDOM_HPP

#include <random>
#include <vector>
#include "Constants.hpp"

/**
 * MyRandom class to manage random request generation
 */
class MyRandom {
private:
    static inline thread_local std::random_device rd; // Random device
    static inline thread_local std::mt19937 gen{rd()}; // Generator
public:
    /**
     * Generate random requests with uniform distribution
     * @param num_requests number of requests
     * @param options list of options
     * @return random requests
     */
    static std::vector<std::string> generateUniformRandomRequests(const size_t num_requests, const std::vector<std::string>& options) {
        std::vector<std::string> requests;
        requests.reserve(num_requests);

        std::uniform_int_distribution<size_t> dist(0, options.size() - 1);

        for (size_t i = 0; i < num_requests; ++i) {
            requests.push_back(options[dist(gen)]);
        }
        return requests;
    }

    /**
     * Generate random requests with zipfian distribution
     * @param num_requests number of requests
     * @param options list of options
     * @return random requests
     */
    static std::vector<std::string> generateZipfRandomRequests(const size_t num_requests, const std::vector<std::string>& options) {
        std::vector<std::string> requests;
        requests.reserve(num_requests);

        const double s = 1.0;
        std::vector<double> weights;
        weights.reserve(options.size());

        for (size_t i = 0; i < options.size(); ++i) {
            weights.push_back(1.0 / std::pow(i + 1, s));
        }

        std::discrete_distribution<size_t> dist(weights.begin(), weights.end());

        for (size_t i = 0; i < num_requests; ++i) {
            requests.push_back(options[dist(gen)]);
        }
        return requests;
    }

    /**
     * Generates random requests based on api and random type
     * @param num_requests number of requests
     * @param api api to use (weather or stocks)
     * @param random_type type of random (uniform or zipf)
     * @return random requests
     */
    static std::vector<std::string> generateRandomRequests(const size_t num_requests, const std::string& api, const std::string& random_type) {
        const std::vector<std::string> options = api == "weather" ? CITIES : STOCKS;
        if (random_type == "uniform") {
            return generateUniformRandomRequests(num_requests, options);
        }
        else if (random_type == "zipf") {
            return generateZipfRandomRequests(num_requests, options);
        }
        else {
            return {};
        }
    }
};

#endif //MYRANDOM_HPP