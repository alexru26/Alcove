//
// Created by Alex Ru on 4/12/25.
//

#include <Cache/LFUCache.hpp>
#include <gtest/gtest.h>

TEST(LFUCacheTest, BasicEviction) {
    LFUCache<int, std::string> cache(2);
    cache.put(1, "A");
    cache.put(2, "B");
    cache.put(3, "C");

    EXPECT_EQ(cache.get(1), "");
    EXPECT_EQ(cache.get(2), "B");
    EXPECT_EQ(cache.get(3), "C");

    cache.get(2);
    cache.put(4, "D");

    EXPECT_EQ(cache.get(1), "");
    EXPECT_EQ(cache.get(2), "B");
    EXPECT_EQ(cache.get(3), "");
    EXPECT_EQ(cache.get(4), "D");

    cache.get(4);
    cache.get(4);
    cache.put(5, "E");

    EXPECT_EQ(cache.get(1), "");
    EXPECT_EQ(cache.get(2), "");
    EXPECT_EQ(cache.get(3), "");
    EXPECT_EQ(cache.get(4), "D");
    EXPECT_EQ(cache.get(5), "E");
}

TEST(LFUCacheTest, ZeroCapacity) {
    LFUCache<int, std::string> cache(0);
    cache.put(1, "A");
    EXPECT_EQ(cache.get(1), "");
}

TEST(LFUCacheTest, UpdateExistingKey) {
    LFUCache<int, std::string> cache(2);
    cache.put(1, "A");
    cache.put(1, "A+");
    cache.put(2, "B");

    EXPECT_EQ(cache.get(1), "A+");
    EXPECT_EQ(cache.get(2), "B");

    cache.get(1);
    cache.put(3, "C");

    EXPECT_EQ(cache.get(1), "A+");
    EXPECT_EQ(cache.get(2), "");
    EXPECT_EQ(cache.get(3), "C");
}

TEST(LFUCacheTest, MemoryComputation) {
    LFUCache<int, int> cache(2);
    cache.put(1, 1);
    EXPECT_EQ(cache.memoryUsed(), 28);
    cache.put(2, 2);
    EXPECT_EQ(cache.memoryUsed(), 52);
    cache.get(1);
    cache.put(3, 3);
    EXPECT_EQ(cache.memoryUsed(), 56);
    cache.put(2, 4);
    EXPECT_EQ(cache.memoryUsed(), 56);
    cache.get(2);
    EXPECT_EQ(cache.memoryUsed(), 52);
    cache.get(2);
    EXPECT_EQ(cache.memoryUsed(), 56);
}