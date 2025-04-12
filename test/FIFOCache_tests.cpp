//
// Created by Alex Ru on 4/12/25.
//

#include <Cache/FIFOCache.hpp>
#include <gtest/gtest.h>

TEST(FIFOCacheTest, BasicEviction) {
    FIFOCache<int, std::string> cache(2);
    cache.put(1, "A");
    cache.put(2, "B");
    cache.put(3, "C"); // Evicts 1

    EXPECT_EQ(cache.get(1), "");   // Not found
    EXPECT_EQ(cache.get(2), "B");  // Still present
}

TEST(FIFOCacheTest, ZeroCapacity) {
    FIFOCache<int, int> cache(0);
    cache.put(1, 100); // No-op
    EXPECT_EQ(cache.get(1), 0); // Not stored
}

TEST(FIFOCacheTest, UpdateExistingKey) {
    FIFOCache<int, std::string> cache(2);
    cache.put(1, "A");
    cache.put(1, "A+"); // Update
    cache.put(2, "B");
    cache.put(3, "C"); // Evicts 1 (oldest insertion, not 2)

    EXPECT_EQ(cache.get(1), "");   // Evicted
    EXPECT_EQ(cache.get(3), "C");
}
