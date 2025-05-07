<h1 style="text-align: center;">Alcove</h1>

## Introduction
Alcove is an open-source framework that randomly queries data from different API's and stores them via different caching algorithms. The purpose of this project is to compare the different caching algorithms.

## Caching Algorithms
Here are the caching algorithms I implemented:

| Algorithm | Description                                                                                                                                                                                                          |
|:----------|:---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| FIFO      | First In First Out (FIFO) operates on the idea that the first added is first to leave when the cache is full                                                                                                         |
| LRU       | Least Recently Use (LRU) operates on the idea that the least recently retrieved item is first to leave when the cache is full.                                                                                       |
| LFU       | Least Frequently Used (LFU) operates on the idea that the least frequently retrieved item is first to leave when the cache is full. If multiple items have the same least frequent counter, then it defaults to LRU. |

This [article](https://algocademy.com/blog/cache-algorithms-fifo-vs-lru-vs-lfu-a-comprehensive-guide/) is a good resource for more detailed information.

## Performance Metrics
To test each caching algorithm, I run multiple requests to an API and store them in the cache. Here are the metrics I used:

| Metric          | Description                              |
|:----------------|:-----------------------------------------|
| Hit Rate        | Queries from cache out of total queries  |
| Average Latency | Time per query                           |
| Throughput      | Requests per second                      |
| Total Duration  | Total time to go through requests        |
| Memory Usage    | Memory consumed by cache data structures |

## Command Line Arguments
### --api / -a [type]
API to pull data from. This is required. Options are:

| Argument | Description                                                |
|:---------|:-----------------------------------------------------------|
| weather  | Daily weather data from https://weather.visualcrossing.com |
| stocks   | Daily raw stock data from https://www.alphavantage.co      |

### --requests / -req [num]
Number of requests to perform. Default is 100.

### --cache / -c [type]
Type of caching algorithm to use. This is required. Options are fifo, lru, and lfu. See section for differences. 

### --size / -s [num]
Number of items a cache can hold. Default is 10.

### --threads / -th [num]
Number of CPU threads to use for the program. Default is 1.

### --random / -r [type]
Random distribution algorithm to use. This is required. Options are:

| Argument | Description                                                                                                                                   |
|:---------|:----------------------------------------------------------------------------------------------------------------------------------------------|
| uniform  | Complete uniform distribution across options                                                                                                  |
| zipf     | [Zipfian distribution](https://en.wikipedia.org/wiki/Zipf%27s_law), which makes each option's frequency inversely proportional to their rank. |

## Building and Testing
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

## Project Structure
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
