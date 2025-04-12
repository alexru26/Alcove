//
// Created by Alex Ru on 4/12/25.
//

#include "LRUCache.hpp"
#include <gtest/gtest.h>

TEST(LRUCacheTest, BasicEviction) {
    LRUCache<int, std::string> cache(2);
    cache.put(1, "A");
    cache.put(2, "B");
    cache.get(1);       // Marks 1 as recently used
    cache.put(3, "C");  // Evicts 2 (least recently used)

    EXPECT_EQ(cache.get(2), "");   // Evicted
    EXPECT_EQ(cache.get(1), "A");  // Still present
}

TEST(LRUCacheTest, UpdateExistingKey) {
    LRUCache<int, std::string> cache(2);
    cache.put(1, "A");
    cache.put(1, "A+"); // Update and mark as recent
    cache.put(2, "B");
    cache.put(3, "C");  // Evicts 2 (1 is still recent)

    EXPECT_EQ(cache.get(1), ""); // Updated and retained
    EXPECT_EQ(cache.get(2), "B");   // Evicted
}
