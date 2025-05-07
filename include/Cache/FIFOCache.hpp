// FIFO Cache implementation
// Alex Ru
// 5/13/25

#ifndef FIFOCACHE_HPP
#define FIFOCACHE_HPP

#include "Cache.h"

/**
 * FIFO Cache class that uses FIFO algorithm to store keys and values
 * @tparam Key key value type
 * @tparam Value corresponding value type
 */
template<typename Key, typename Value>
class FIFOCache final : public Cache<Key, Value> {
private:
    std::queue<Key> keys; // Queue of stored keys
    std::unordered_map<Key, Value> cache; // Mapping of keys to corresponding value
public:
    /**
     * Explicit constructor that takes in capacity as parameter
     * @param capacity size of cache, aka amount it can hold
     */
    explicit FIFOCache(size_t capacity) : Cache<Key, Value>(capacity) {}

    /**
     * Default destructor
     */
    ~FIFOCache() override = default;

    /**
     * Copy constructor
     * @param other other FIFO cache to copy
     */
    FIFOCache(const FIFOCache& other): Cache<Key, Value>(other) {
        this->keys = other.keys;
        this->cache = other.cache;
    }

    /**
     * Assignment operator
     * @param other other FIFO cache to copy
     * @return copied FIFO cache
     */
    FIFOCache& operator=(const FIFOCache& other) {
        this->keys = other.keys;
        this->cache = other.cache;
        return *this;
    }

    /**
     * Checks if key exists in FIFO cache
     * @param key key value to check
     * @return whether key exists
     */
    bool exists(Key key) override {
        return cache.count(key);
    }

    /**
     * Stores key and value into the FIFO cache
     * @param key key value
     * @param value corresponding value
     */
    void put(Key key, Value value) override {
        std::lock_guard<std::mutex> lock(this->mutex);
        if (this->capacity == 0) return;

        if (exists(key)) {
            cache[key] = value;
            return;
        }

        if (keys.size() >= this->capacity) {
            Key evict = keys.front();
            keys.pop();
            cache.erase(evict);

            this->memory -= 2 * sizeof(evict) + sizeof(cache[evict]);
        }

        keys.push(key);
        cache[key] = value;
        this->memory += 2 * sizeof(key) + sizeof(value);
    }

    /**
     * Returns the value given key
     * @param key key value
     * @return corresponding value
     */
    Value get(Key key) override {
        std::lock_guard<std::mutex> lock(this->mutex);
        return cache.count(key) ? cache[key] : Value();
    }

    /**
     * Prints out the content of the FIFO cache
     */
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
