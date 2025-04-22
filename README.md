<h1 style="text-align: center;">Alcove</h1>

## Introduction
Alcove is an open-source framework that semi-randomly queries data from a SQL database and stores them via different caching algorithms.

## Performance Metrics
| Metric          | Description                              |
|:----------------|:-----------------------------------------|
| Hit Rate        | Queries from cache out of total queries  |
| Average Latency | Time per query                           |
| Throughput      | Requests per second                      |
| Total Duration  | Total time to go through requests        |
| Memory Usage    | Memory consumed by cache data structures |

## Command Line Arguments
### --api [arg]
Specifies which API to pull data from. Options are:

| Argument | Description                                                                        |
|:---------|:-----------------------------------------------------------------------------------|
| weather  | Daily weather data from https://weather.visualcrossing.com                         |

### --threads [arg]
Specifies the number of CPU threads to use for the program.

### --random [arg]
Specifies which random distribution algorithm to use. Options are:

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
|   |-- main.cpp
|-- include/
|   |-- module1/
|   |   |-- module1a.h
|   |   |-- module1b.h
|   |-- module2/
|   |   |-- module2a.h
|   |   |-- module2b.h
|-- tests/
|   |-- test1.cpp
|   |-- test2.cpp
|-- build/
|-- CMakeLists.txt
|-- README.md
```
