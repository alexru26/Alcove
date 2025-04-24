//
// Created by Alex Ru on 3/31/25.
//

#ifndef LRUCACHE_HPP
#define LRUCACHE_HPP

#include "Cache.h"

template<typename Key, typename Value>
class LRUCache final : public Cache<Key, Value> {
private:
    using List = std::list<Key>;
    List keys;
    std::unordered_map<Key, std::pair<Value, typename List::iterator>> cache;
public:
    explicit LRUCache(size_t capacity) : Cache<Key, Value>(capacity) {}

    ~LRUCache() override = default;

    LRUCache(const LRUCache& other): Cache<Key, Value>(other) {
        this->keys = other.keys;
        this->cache = other.cache;
    }

    LRUCache& operator=(const LRUCache& other) {
        this->keys = other.keys;
        this->cache = other.cache;
        return *this;
    }

    bool exists(Key key) override {
        return cache.count(key);
    }

    void put(Key key, Value value) override {
        if (this->capacity == 0) return;

        if (exists(key)) {
            keys.erase(cache[key].second);
            keys.push_front(key);
            cache[key].first = value;
            cache[key].second = keys.begin();
            return;
        }

        if (keys.size() >= this->capacity) {
            Key evict = keys.back();
            keys.pop_back();
            this->memory -= 2 * sizeof(key) + sizeof(cache[evict]);
            cache.erase(evict);
        }

        keys.push_front(key);
        cache[key] = {value, keys.begin()};
        this->memory += 2 * sizeof(key) + sizeof(cache[key]);
    }

    Value get(Key key) override {
        auto it = cache.find(key);
        if (it == cache.end()) return Value();

        keys.erase(it->second.second);
        keys.push_front(key);
        it->second.second = keys.begin();

        return it->second.first;
    }

    void print() override {
        for (Key key : keys) {
            std::cout << key << " : " << cache[key].first << std::endl;
        }
    }
};

#endif //LRUCACHE_HPP