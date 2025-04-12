# Alcove
## Introduction
Alcove is an open-source framework that semi-randomly queries data from a SQL database and stores them via different caching algorithms.

## Building and Testing
``` 
cmake -S . -B build
cmake --build build
```

```
cd build
ctest
```

## Project Structure
```
Alcove/
|-- src/
|   |-- main.cpp
|-- include/
|   |-- module1.h
|   |-- module2.h
|-- tests/
|   |-- test1.cpp
|   |-- test2.cpp
|-- build/
|-- CMakeLists.txt
|-- README.md
```
