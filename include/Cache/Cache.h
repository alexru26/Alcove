//
// Created by Alex Ru on 3/31/25.
//

#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <list>
#include <queue>
#include <mutex>
#include <unordered_map>

template<typename Key, typename Value>
class Cache {
protected:
    size_t capacity;
    size_t memory = 0;
    mutable std::mutex mutex;
public:
    explicit Cache(const size_t capacity) : capacity(capacity) {}
    virtual ~Cache() = default;

    virtual bool exists(Key key) = 0;
    virtual void put(Key key, Value value) = 0;
    virtual Value get(Key key) = 0;
    virtual void print() = 0;

    [[nodiscard]] size_t memoryUsed() const { return memory; }
};

#endif //CACHE_H
