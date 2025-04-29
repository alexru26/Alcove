//
// Created by Alex Ru on 4/29/25.
//

#ifndef CLIHELPER_H
#define CLIHELPER_H

#include <iostream>
#include <string>

class cliHelper {
public:
    static int parseIntArg(const char *arg, const std::string &flagName);

    static void printBanner();

    static void parseArguments(int argc, char* argv[],
        std::string& api, int& num_requests, std::string& cache_type,
        int& cache_size, int& threads, std::string& random_type);

    static void displayHelp();
};



#endif //CLIHELPER_H
