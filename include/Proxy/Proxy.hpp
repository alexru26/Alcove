//
// Created by Alex Ru on 4/12/25.
//

#ifndef PROXY_HPP
#define PROXY_HPP

#include <Cache/Cache.h>
#include <memory>

template<typename Key, typename Value>
class Proxy {
protected:
    std::unique_ptr<Cache<Key, Value>> cache;
    virtual Value runPrivateQuery(const Key& key) = 0;
public:
    explicit Proxy(std::unique_ptr<Cache<Key, Value>> cache) : cache(std::move(cache)) {}

    virtual ~Proxy() = default;

    Value query(const Key& key) {
        if (cache->exists(key)) {
            return cache->get(key);
        } else {
            Value result = runPrivateQuery(key);
            cache->put(key, result);
            return result;
        }
    }
};

#endif // PROXY_HPP
