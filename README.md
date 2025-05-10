<h1 style="text-align: center;">Alcove</h1>

# Introduction
Alcove is an open-source framework that randomly queries data from different API's and stores them via different caching algorithms. The purpose of this project is to compare the different caching algorithms.

# Caching Algorithms
Here are the caching algorithms I implemented:

| Algorithm | Description                                                                                                                                                                                                          |
|:----------|:---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| FIFO      | First In First Out (FIFO) operates on the idea that the first added is first to leave when the cache is full                                                                                                         |
| LRU       | Least Recently Use (LRU) operates on the idea that the least recently retrieved item is first to leave when the cache is full.                                                                                       |
| LFU       | Least Frequently Used (LFU) operates on the idea that the least frequently retrieved item is first to leave when the cache is full. If multiple items have the same least frequent counter, then it defaults to LRU. |

This [article](https://algocademy.com/blog/cache-algorithms-fifo-vs-lru-vs-lfu-a-comprehensive-guide/) is a good resource for more detailed information.

# Performance Metrics
To test each caching algorithm, I run multiple requests to an API and store them in the cache. Here are the metrics I used:

| Metric          | Description                              |
|:----------------|:-----------------------------------------|
| Hit Rate        | Queries from cache out of total queries  |
| Average Latency | Time per query                           |
| Throughput      | Requests per second                      |
| Total Duration  | Total time to go through requests        |
| Memory Usage    | Memory consumed by cache data structures |

# Results
To test each cache, I ran the program with the following settings:

| Argument | Value  |
|:---------|:-------|
| api      | stocks |
| requests | 10000  |
| size     | 75     |
| threads  | 5      |

Here are my results with uniform random generation:

| Cache      | Hit Rate | Average Latency | Throughput | Total Duration | Memory Usage |
|:-----------|:---------|:----------------|:-----------|:---------------|:-------------|
| FIFO Cache | 74.19%   | 57.48ms         | 83.23req/s | 120.16s        | 4800 bytes   |
| LRU Cache  | 73.51%   | 61.13ms         | 76.83req/s | 130.15s        | 5400 bytes   |
| LFU Cache  | 73.04%   | 59.27ms         | 81.54req/s | 122.63s        | 6140 bytes   |

Here are my results with zipf random generation:

| Cache      | Hit Rate | Average Latency | Throughput   | Total Duration | Memory Usage |
|:-----------|:---------|:----------------|:-------------|:---------------|:-------------|
| FIFO Cache | 88.03%   | 28.19ms         | 169.11req/s  | 59.13s         | 4800 bytes   |
| LRU Cache  | 91.44%   | 19.60ms         | 232.35req/s  | 43.04s         | 5400 bytes   |
| LFU Cache  | 93.29%   | 15.20ms         | 310.73 req/s | 32.18s         | 6224 bytes   |

With uniform random generation, the statistics do not differ much between the cache types. The only difference is the memory usage, which is the least for FIFO and the most for LFU. Thus, FIFO is the best cache for uniform random generation.

With zipf random generation, the statistics do differ a significant amount between the cache types. The LFU outperforms the other caches, but it takes up more space. When scaled up, the performance boost of the LFU should outweigh its larger size.

# Command Line Arguments
## --api / -a [type]
API to pull data from. This is required. For each API, there are 200 unique options that could be randomly chosen. Options are:

| Argument | Description                                                |
|:---------|:-----------------------------------------------------------|
| weather  | Daily weather data from https://weather.visualcrossing.com |
| stocks   | Daily raw stock data from https://www.alphavantage.co      |

## --requests / -req [num]
Number of requests to perform. Default is 100.

## --cache / -c [type]
Type of caching algorithm to use. This is required. Options are fifo, lru, and lfu. See section for differences. 

## --size / -s [num]
Number of items a cache can hold. Default is 10.

## --threads / -th [num]
Number of CPU threads to use for the program. Default is 1.

## --random / -r [type]
Random distribution algorithm to use. This is required. Options are:

| Argument | Description                                                                                                                                   |
|:---------|:----------------------------------------------------------------------------------------------------------------------------------------------|
| uniform  | Complete uniform distribution across options                                                                                                  |
| zipf     | [Zipfian distribution](https://en.wikipedia.org/wiki/Zipf%27s_law), which makes each option's frequency inversely proportional to their rank. |

# Building and Testing
To build the project and set up for testing, run these commands:
``` 
cmake -S . -B build
cmake --build build
```

To actually run the tests, run these commands:
```
cd build
ctest
```

# Project Structure
This is the C++ project structure I chose to go with. It's pretty standard since this project isn't very big.
```
Alcove/
|-- src/
|   |-- helper.h
|   |-- main.cpp
|-- include/
|   |-- module1/
|   |   |-- module1a.h
|   |   |-- module1b.h
|   |-- module2/
|   |   |-- module2a.h
|   |   |-- module2b.h
|-- test/
|   |-- test1.cpp
|   |-- test2.cpp
|-- build/
|-- CMakeLists.txt
|-- README.md
```
