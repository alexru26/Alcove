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

    bool exists(Key key) override {
        return cache.count(key);
    }

    void put(Key key, Value value) override {
        if (this->capacity == 0) return;

        auto it = cache.find(key);
        if (it != cache.end()) {
            this->memory -= sizeof(it->second.first);
            it->second.first = value;
            this->memory += sizeof(value);
            keys.erase(it->second.second);
        } else {
            if (keys.size() >= this->capacity) {
                Key evict = keys.back();
                keys.pop_back();
                cache.erase(evict);
                this->memory -= (sizeof(Key) + sizeof(cache[evict].first));
            }
        }

        keys.push_front(key);
        cache[key] = {value, keys.begin()};

        this->memory += sizeof(Key) + sizeof(value);
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