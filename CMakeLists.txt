cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0135 NEW)

project(Alcove)

set(CMAKE_CXX_STANDARD 20)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# Avoid overriding parent compiler settings (especially useful on Windows)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Add main project executable
add_executable(Alcove
        src/main.cpp
        include/Cache.h
        include/FIFOCache.hpp
        include/LRUCache.hpp
        include/LFUCache.hpp
)

target_include_directories(Alcove PRIVATE include)

# Add test executable
add_executable(CacheTests test/FIFOCache_tests.cpp
        test/LRUCache_tests.cpp
        test/LFUCache_tests.cpp)

target_include_directories(CacheTests PRIVATE include)

target_link_libraries(CacheTests
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(CacheTests)
