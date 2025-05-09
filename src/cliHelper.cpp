// CLI Helper implementation
// Alex Ru
// 5/13/25

#include "cliHelper.h"

int cliHelper::parseIntArg(const char* arg, const std::string& flagName) {
    try {
        // Parse int argument
        return std::stoi(arg);
    } catch (const std::invalid_argument&) {
        throw std::runtime_error("Error: Invalid number for " + flagName + ".");
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Error: Number out of range for " + flagName + ".");
    }
}

void cliHelper::printBanner() {
    std::cout << R"(====================================================)" << std::endl;
    std::cout << R"(       d8888 888                                    )" << std::endl;
    std::cout << R"(      d88888 888                                    )" << std::endl;
    std::cout << R"(     d88P888 888                                    )" << std::endl;
    std::cout << R"(    d88P 888 888  .d8888b .d88b.  888  888  .d88b.  )" << std::endl;
    std::cout << R"(   d88P  888 888 d88P"   d88""88b 888  888 d8P  Y8b )" << std::endl;
    std::cout << R"(  d88P   888 888 888     888  888 Y88  88P 88888888 )" << std::endl;
    std::cout << R"( d8888888888 888 Y88b.   Y88..88P  Y8bd8P  Y8b.     )" << std::endl;
    std::cout << R"(d88P     888 888  "Y8888P "Y88P"    Y88P    "Y8888  )" << std::endl;
    std::cout << R"(====================================================)" << std::endl;
}

void cliHelper::parseArguments(int argc, char *argv[],
    std::string &api, int &num_requests, std::string &cache_type,
    int &cache_size, int &num_threads, std::string &random_type,
    std::unique_ptr<Cache<std::string, nlohmann::json>>& cache,
    std::unique_ptr<Proxy<std::string, nlohmann::json>>& proxy,
    std::vector<std::string>& requests) {

    // For every argument
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        // Parse the correct argument
        if (arg == "--help" || arg == "-h") {
            displayHelp();
            exit(0);
        } else if (arg == "--api" || arg == "-a") {
            if (++i >= argc) throw std::runtime_error("Missing value for API");
            api = argv[i];
            if (api != "weather" && api != "stocks")
                throw std::runtime_error("Invalid API type. Use 'weather' or 'stocks'.");
        } else if (arg == "--requests" || arg == "-req") {
            if (++i >= argc) throw std::runtime_error("Missing value for --requests");
            num_requests = parseIntArg(argv[i], "requests");
        } else if (arg == "--cache" || arg == "-c") {
            if (++i >= argc) throw std::runtime_error("Missing value for --cache");
            cache_type = argv[i];
            if (cache_type != "fifo" && cache_type != "lru" && cache_type != "lfu")
                throw std::runtime_error("Invalid cache type. Use 'fifo', 'lru', or 'lfu'.");
        } else if (arg == "--size" || arg == "-s") {
            if (++i >= argc) throw std::runtime_error("Missing value for --size");
            cache_size = parseIntArg(argv[i], "size");
        } else if (arg == "--threads" || arg == "-th") {
            if (++i >= argc) throw std::runtime_error("Missing value for --threads");
            num_threads = parseIntArg(argv[i], "threads");
        } else if (arg == "--random" || arg == "-r") {
            if (++i >= argc) throw std::runtime_error("Missing value for --random");
            random_type = argv[i];
            if (random_type != "uniform" && random_type != "zipf")
                throw std::runtime_error("Invalid random type. Use 'uniform' or 'zipf'.");
        } else {
            throw std::runtime_error("Unknown arg '" + arg + "'. Use --help or -h for usage.");
        }
    }

    // Check required arguments
    if (api.empty() || cache_type.empty() || random_type.empty())
        throw std::runtime_error("Missing required arguments.");

    // Create cache
    if (cache_type == "fifo") cache = std::make_unique<FIFOCache<std::string, nlohmann::json>>(cache_size);
    else if (cache_type == "lru") cache = std::make_unique<LRUCache<std::string, nlohmann::json>>(cache_size);
    else if (cache_type == "lfu") cache = std::make_unique<LFUCache<std::string, nlohmann::json>>(cache_size);

    // Create proxy
    if (api == "weather") {
        proxy = std::make_unique<WeatherProxy>(std::move(cache));
    }
    else if (api == "stocks") {
        proxy = std::make_unique<StocksProxy>(std::move(cache));
    }

    // Create requests
    requests = MyRandom::generateRandomRequests(num_requests, api, random_type);
}

void cliHelper::displayHelp() {
    std::cout << "Usage: ./Alcove [options] {value}\n"
            << "Options:\n"
            << "  -h, --help                Show this help message\n"
            << "  -a, --api [type]          REQUIRED: API to pull from (weather, stocks)\n"
            << "  -req, --requests [num]    OPTIONAL: Number of requests to perform (default: 100)\n"
            << "  -c, --cache [type]        REQUIRED: Type of cache (fifo, lru, lfu)\n"
            << "  -s, --size [num]          OPTIONAL: Size of cache (default: 10)\n"
            << "  -th, --threads [num]      OPTIONAL: Number of threads to run (default: 1)\n"
            << "  -r, --random [type]       REQUIRED: Random generation algorithm (uniform, zipf)\n";
}
