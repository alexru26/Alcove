#include <thread>

#include "cliHelper.h"

int main(int argc, char* argv[]) {
    int num_requests = 100, cache_size = 10, num_threads = 1;
    std::string api, cache_type, random_type;

    std::unique_ptr<Cache<std::string, nlohmann::json>> cache;
    std::unique_ptr<Proxy<std::string, nlohmann::json>> proxy;

    std::vector<std::string> requests;

    cliHelper::parseArguments(argc, argv, api, num_requests, cache_type, cache_size, num_threads, random_type, cache, proxy, requests);
    cliHelper::printBanner();

    proxy->runBenchmark(requests, num_threads);

    std::cout << "Program has ended" << std::endl;

    return 0;
}
