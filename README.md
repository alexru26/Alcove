<h1 style="text-align: center;">Alcove</h1>
## Introduction
Alcove is an open-source framework that semi-randomly queries data from a SQL database and stores them via different caching algorithms.

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
