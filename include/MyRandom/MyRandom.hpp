//
// Created by Alex Ru on 4/14/25.
//

#ifndef MYRANDOM_HPP
#define MYRANDOM_HPP

#include <random>
#include <vector>

template<typename Key>
class MyRandom {
private:
    static inline thread_local std::random_device rd;
    static inline thread_local std::mt19937 gen{rd()};
public:
    static std::vector<Key> generateUniformRandomRequests(size_t num_requests, std::vector<Key> options) {
        std::vector<Key> requests;
        requests.reserve(num_requests);

        std::uniform_int_distribution<size_t> dist(0, options.size() - 1);

        for (size_t i = 0; i < num_requests; ++i) {
            requests.push_back(options[dist(gen)]);
        }
        return requests;
    }

    static std::vector<Key> generateZipfRandomRequests(size_t num_requests, std::vector<Key> options) {
        std::vector<Key> requests;
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

    static std::vector<Key> generateRandomRequests(const std::string &type, size_t num_requests, std::vector<Key> options) {
        if (type == "uniform") {
            return generateUniformRandomRequests(num_requests, options);
        }
        else if (type == "zipf") {
            return generateZipfRandomRequests(num_requests, options);
        }
        else {
            return std::vector<Key>();
        }
    }
};

#endif //MYRANDOM_HPP