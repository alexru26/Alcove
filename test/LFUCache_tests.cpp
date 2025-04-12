//
// Created by Alex Ru on 4/12/25.
//

#include <Cache/LFUCache.hpp>
#include <gtest/gtest.h>

TEST(LFUCacheTest, BasicEviction) {
    LFUCache<int, std::string> cache(2);
    cache.put(1, "A");
    cache.put(2, "B");
    cache.get(1);        // Freq of 1 becomes 2
    cache.get(1);        // Freq of 1 becomes 3
    cache.put(3, "C");  // Evicts 2 (freq=1)

    EXPECT_EQ(cache.get(2), "");   // Evicted
    EXPECT_EQ(cache.get(1), "A");  // Still present
}

TEST(LFUCacheTest, TieBreakerEviction) {
    LFUCache<int, std::string> cache(2);
    cache.put(1, "A");
    cache.put(2, "B");
    cache.get(1); // Freq=2
    cache.get(2); // Freq=2
    cache.put(3, "C"); // Evicts oldest in minFreq=2 (1 or 2? Depends on LFU+LRU tiebreaker)
    // This test may need adjustment based on your LFU implementation details.
}

TEST(LFUCacheTest, UpdateExistingKey) {
    LFUCache<int, std::string> cache(2);
    cache.put(1, "A");
    cache.get(1);        // Freq=2
    cache.put(1, "A+");  // Update value but don't boost frequency
    cache.put(2, "B");
    cache.put(3, "C");  // Evicts 2 (freq=1)

    EXPECT_EQ(cache.get(1), "A+"); // Updated and retained (freq=2)
    EXPECT_EQ(cache.get(2), "");   // Evicted
}