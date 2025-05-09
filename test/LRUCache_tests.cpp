// Tests for LRU Cache
// Alex Ru
// 5/13/25

#include <Cache/LRUCache.hpp>
#include <gtest/gtest.h>

TEST(LRUCacheTest, BasicEviction) {
    LRUCache<int, std::string> cache(2);
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
}

TEST(LRUCacheTest, ZeroCapacity) {
    LRUCache<int, std::string> cache(0);
    cache.put(1, "A");
    EXPECT_EQ(cache.get(1), "");
}

TEST(LRUCacheTest, UpdateExistingKey) {
    LRUCache<int, std::string> cache(2);
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

TEST(LRUCacheTest, MemoryComputation) {
    LRUCache<int, int> cache(2);
    cache.put(1, 1);
    EXPECT_EQ(cache.memoryUsed(), 24);
    cache.put(2, 2);
    EXPECT_EQ(cache.memoryUsed(), 48);
    cache.get(1);
    cache.put(3, 3);
    EXPECT_EQ(cache.memoryUsed(), 48);
    cache.put(2, 4);
    EXPECT_EQ(cache.memoryUsed(), 48);
}
