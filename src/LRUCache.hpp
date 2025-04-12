//
// Created by Alex Ru on 3/31/25.
//

#ifndef LRUCACHE_HPP
#define LRUCACHE_HPP

#include "Cache.h"
#include <list>

template<typename Key, typename Value>
class LRUCache final : public Cache<Key, Value> {
private:
    using List = std::list<Key>;
    List keys;
    std::unordered_map<Key, std::pair<Value, typename List::iterator>> cache; // DeepSeek
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

    void put(Key key, Value value) override {
        if (this->capacity == 0) return;

        auto it = cache.find(key);
        if (it != cache.end()) {
            it->second.first = value;
            keys.erase(it->second.second);
        } else {
            if (keys.size() >= this->capacity) {
                Key evict = keys.back();
                keys.pop_back();
                cache.erase(evict);
            }
        }

        keys.push_front(key);
        cache[key] = {value, keys.begin()};
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