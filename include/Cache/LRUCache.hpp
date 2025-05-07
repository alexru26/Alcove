// LRU Cache implementation
// Alex Ru
// 5/13/25

#ifndef LRUCACHE_HPP
#define LRUCACHE_HPP

#include "Cache.h"

/**
 * LRU Cache class that uses LRU algorithm to store keys and values
 * @tparam Key key value type
 * @tparam Value corresponding value type
 */
template<typename Key, typename Value>
class LRUCache final : public Cache<Key, Value> {
private:
    using List = std::list<Key>; // List namespace to make things cleaner
    List keys; // List of keys
    std::unordered_map<Key, std::pair<Value, typename List::iterator>> cache; // Mapping of keys to corresponding value and location
public:
    /**
     * Explicit constructor that takes in capacity as parameter
     * @param capacity size of cache, aka amount it can hold
     */
    explicit LRUCache(size_t capacity) : Cache<Key, Value>(capacity) {}

    /**
     * Default destructor
     */
    ~LRUCache() override = default;

    /**
     * Copy constructor
     * @param other other LRU cache to copy
     */
    LRUCache(const LRUCache& other): Cache<Key, Value>(other) {
        this->keys = other.keys;
        this->cache = other.cache;
    }

    /**
     * Assignment operator
     * @param other other LRU cache to copy
     * @return copied LRU cache
     */
    LRUCache& operator=(const LRUCache& other) {
        this->keys = other.keys;
        this->cache = other.cache;
        return *this;
    }

    /**
     * Checks if key exists in LRU cache
     * @param key key value to check
     * @return whether key exists
     */
    bool exists(Key key) override {
        return cache.count(key);
    }

    /**
     * Stores key and value into the LRU cache
     * @param key key value
     * @param value corresponding value
     */
    void put(Key key, Value value) override {
        std::lock_guard<std::mutex> lock(this->mutex);
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

    /**
     * Returns the value given key
     * @param key key value
     * @return corresponding value
     */
    Value get(Key key) override {
        std::lock_guard<std::mutex> lock(this->mutex);
        auto it = cache.find(key);
        if (it == cache.end()) return Value();

        keys.erase(it->second.second);
        keys.push_front(key);
        it->second.second = keys.begin();

        return it->second.first;
    }

    /**
     * Prints out the content of the LRU cache
     */
    void print() override {
        for (Key key : keys) {
            std::cout << key << " : " << cache[key].first << std::endl;
        }
    }
};

#endif //LRUCACHE_HPP