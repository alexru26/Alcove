cmake_minimum_required(VERSION 3.30)
project(Alcove)

set(CMAKE_CXX_STANDARD 20)

add_executable(Alcove src/main.cpp
        src/Cache.h
        src/FIFOCache.hpp
        src/LRUCache.hpp
        src/LFUCache.hpp)
