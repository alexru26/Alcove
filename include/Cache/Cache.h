// Cache abstract class header file
// Alex Ru
// 5/13/25

#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <list>
#include <queue>
#include <mutex>
#include <unordered_map>

/**
 * Abstract Cache class that stores a key and a value
 * @tparam Key key value type
 * @tparam Value corresponding value type
 */
template<typename Key, typename Value>
class Cache {
protected:
    size_t capacity; // The amount of data cache can hold
    size_t memory = 0; // The amount of memory cache takes up
    mutable std::mutex mutex; // Mutex object to handle multi-thread logic
public:
    /**
     * Explicit constructor that takes in capacity as parameter
     * @param capacity size of cache, aka amount it can hold
     */
    explicit Cache(const size_t capacity) : capacity(capacity) {}

    /**
     * Abstract default destructor
     */
    virtual ~Cache() = default;

    /**
     * Abstract method that checks if key exists in cache
     * @param key key value to check
     * @return whether key exists
     */
    virtual bool exists(Key key) = 0;

    /**
     * Abstract method that stores key and value into the cache
     * @param key key value
     * @param value corresponding value
     */
    virtual void put(Key key, Value value) = 0;

    /**
     * Abstract method that returns the value given key
     * @param key key value
     * @return corresponding value
     */
    virtual Value get(Key key) = 0;

    /**
     * Abstract method that prints out the content of the cache
     */
    virtual void print() = 0;

    /**
     * Getter for memory usage
     * @return Memory usage
     */
    [[nodiscard]] size_t memoryUsed() const { return memory; }
};

#endif //CACHE_H
