// CLI Helper header file
// Alex Ru
// 5/13/25

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

/**
 * CLI Helper class
 */
class cliHelper {
public:
    /**
     * Parse integer arguments
     * @param arg argument
     * @param flagName name of variable name
     * @return integer argument
     */
    static int parseIntArg(const char *arg, const std::string &flagName);

    /**
     * Print banner
     */
    static void printBanner();

    /**
     * Parse all arguments
     * @param argc arg count
     * @param argv arg list
     * @param api apie type
     * @param num_requests number of requests
     * @param cache_type type of cache
     * @param cache_size size of cache
     * @param num_threads number of threads
     * @param random_type type of random
     * @param cache cache object
     * @param proxy proxy object
     * @param requests requests list
     */
    static void parseArguments(int argc, char* argv[],
                               std::string& api, int& num_requests, std::string& cache_type,
                               int& cache_size, int& num_threads, std::string& random_type,
                               std::unique_ptr<Cache<std::string, nlohmann::json>>& cache,
                               std::unique_ptr<Proxy<std::string, nlohmann::json>>& proxy,
                               std::vector<std::string>& requests);

    /**
     * Display help
     */
    static void displayHelp();
};



#endif //CLIHELPER_H
