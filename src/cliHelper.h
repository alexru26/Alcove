//
// Created by Alex Ru on 4/29/25.
//

#ifndef CLIHELPER_H
#define CLIHELPER_H

#include <iostream>
#include <string>

#include <Cache/FIFOCache.hpp>
#include <Cache/LRUCache.hpp>
#include <Cache/LFUCache.hpp>

#include <Proxy/WeatherProxy.hpp>
#include <Proxy/StocksProxy.hpp>

#include <MyRandom/MyRandom.hpp>

class cliHelper {
public:
    static int parseIntArg(const char *arg, const std::string &flagName);

    static void printBanner();

    static void parseArguments(int argc, char* argv[],
        std::string& api, int& num_requests, std::string& cache_type,
        int& cache_size, int& num_threads, std::string& random_type,
        std::unique_ptr<Cache<std::string, nlohmann::json>>& cache,
        std::unique_ptr<Proxy<std::string, nlohmann::json>>& proxy,
        std::vector<std::string>& requests);

    static void displayHelp();
};



#endif //CLIHELPER_H
