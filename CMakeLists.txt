cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0135 NEW)

project(Alcove)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_Declare(
        httplib
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
        GIT_TAG v0.15.3  # or latest stable tag
)
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3  # or latest stable release
)
# Avoid overriding parent compiler settings (especially useful on Windows)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(
        googletest
        httplib
        nlohmann_json
)

enable_testing()

# Add main project executable
add_executable(Alcove
        src/main.cpp
        include/Cache/Cache.h
        include/Cache/FIFOCache.hpp
        include/Cache/LRUCache.hpp
        include/Cache/LFUCache.hpp
        include/Proxy/WeatherProxy.hpp
        include/Proxy/Proxy.hpp
        include/myRandom/myRandom.hpp
)

target_include_directories(Alcove PUBLIC include)
target_include_directories(Alcove PRIVATE src)

target_link_libraries(Alcove
        sqlite3
        httplib
        nlohmann_json::nlohmann_json
)

# Add test executable
add_executable(CacheTests test/FIFOCache_tests.cpp
        test/LRUCache_tests.cpp
        test/LFUCache_tests.cpp)

target_include_directories(CacheTests PUBLIC include)

target_link_libraries(CacheTests
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(CacheTests)
