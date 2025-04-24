//
// Created by Alex Ru on 4/12/25.
//

#include <Cache/FIFOCache.hpp>
#include <gtest/gtest.h>

TEST(FIFOCacheTest, BasicEviction) {
    FIFOCache<int, std::string> cache(2);
    cache.put(1, "A");
    cache.put(2, "B");
    cache.put(3, "C");

    EXPECT_EQ(cache.get(1), "");
    EXPECT_EQ(cache.get(2), "B");
    EXPECT_EQ(cache.get(3), "C");

    cache.put(4, "D");
    EXPECT_EQ(cache.get(1), "");
    EXPECT_EQ(cache.get(2), "");
    EXPECT_EQ(cache.get(3), "C");
    EXPECT_EQ(cache.get(4), "D");
}

TEST(FIFOCacheTest, ZeroCapacity) {
    FIFOCache<int, std::string> cache(0);
    cache.put(1, "A");
    EXPECT_EQ(cache.get(1), "");
}

TEST(FIFOCacheTest, UpdateExistingKey) {
    FIFOCache<int, std::string> cache(2);
    cache.put(1, "A");
    cache.put(1, "A+");
    cache.put(2, "B");

    EXPECT_EQ(cache.get(1), "A+");
    EXPECT_EQ(cache.get(2), "B");

    cache.put(3, "C");

    EXPECT_EQ(cache.get(1), "");
    EXPECT_EQ(cache.get(2), "B");
    EXPECT_EQ(cache.get(3), "C");
}

TEST(FIFOCacheTest, MemoryComputation) {
    FIFOCache<int, int> cache(2);
    cache.put(1, 1);
    EXPECT_EQ(cache.memoryUsed(), 12);
    cache.put(2, 2);
    EXPECT_EQ(cache.memoryUsed(), 24);
    cache.put(3, 3);
    EXPECT_EQ(cache.memoryUsed(), 24);
    cache.put(2, 4);
    EXPECT_EQ(cache.memoryUsed(), 24);
}
