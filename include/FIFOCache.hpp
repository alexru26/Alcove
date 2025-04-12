//
// Created by Alex Ru on 3/31/25.
//

#ifndef FIFOCACHE_HPP
#define FIFOCACHE_HPP

#include "Cache.h"
#include <queue>

template<typename Key, typename Value>
class FIFOCache final : public Cache<Key, Value> {
private:
    std::queue<Key> keys;
    std::unordered_map<Key, Value> cache;
public:
    explicit FIFOCache(size_t capacity) : Cache<Key, Value>(capacity) {}

    ~FIFOCache() override = default;

    FIFOCache(const FIFOCache& other): Cache<Key, Value>(other) {
        this->keys = other.keys;
        this->cache = other.cache;
    }

    FIFOCache& operator=(const FIFOCache& other) {
        this->keys = other.keys;
        this->cache = other.cache;
        return *this;
    }

    void put(Key key, Value value) override {
        if (this->capacity == 0) return;

        if (cache.count(key)) {
            cache[key] = value;
            return;
        }

        if (keys.size() >= this->capacity) {
            Key evict = keys.front();
            keys.pop();
            cache.erase(evict);
        }

        keys.push(key);
        cache[key] = value;

    }

    Value get(Key key) override {
        return cache.count(key) ? cache[key] : Value();
    }

    void print() override {
        std::queue<Key> temp = keys;
        while (!temp.empty()) {
            Key key = temp.front();
            temp.pop();
            std::cout << key << " : " << cache[key] << std::endl;
        }
    }
};

#endif //FIFOCACHE_HPP
