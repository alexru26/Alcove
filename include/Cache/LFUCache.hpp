//
// Created by Alex Ru on 4/12/25.
//

#ifndef LFUCACHE_HPP
#define LFUCACHE_HPP

#include "Cache.h"

template<typename Key, typename Value>
class LFUCache final : public Cache<Key, Value> {
private:
    struct Node {
        Value value;
        int freq;
        typename std::list<Key>::iterator listIt;
    };

    int minFreq = 0;

    std::unordered_map<Key, Node> cache;
    std::unordered_map<int, std::list<Key>> freqList;
public:
    explicit LFUCache(size_t capacity) : Cache<Key, Value>(capacity) {}

    ~LFUCache() override = default;

    bool exists(Key key) override {
        return cache.count(key);
    }

    void put(Key key, Value value) override {
        if (this->capacity == 0) return;

        if (exists(key)) {
            Node& node = cache[key];
            this->memory -= sizeof(node.value);
            node.value = value;
            this->memory += sizeof(value);
            return;
        }

        if (cache.size() == this->capacity) {
            Key evict = freqList[minFreq].back();
            Node& evicted_node = cache[evict];
            freqList[minFreq].pop_back();
            cache.erase(evict);
            this->memory -= 2 * sizeof(evict) + sizeof(evicted_node);
            if (freqList[minFreq].empty()) {
                freqList.erase(minFreq);
                this->memory -= sizeof(minFreq);
            }
        }

        freqList[1].push_front(key);
        if (freqList[1].size() == 1) this->memory += sizeof(int);
        cache[key] = {value, 1, freqList[1].begin()};
        this->memory += 2 * sizeof(key) + sizeof(cache[key]);
        minFreq = 1;
    }

    Value get(Key key) override {
        if (!exists(key)) return Value();

        Node& node = cache[key];
        int oldFreq = node.freq;

        freqList[oldFreq].erase(node.listIt);
        this->memory -= sizeof(key);
        if (freqList[oldFreq].empty()) {
            freqList.erase(oldFreq);
            this->memory -= sizeof(oldFreq);
            if (minFreq == oldFreq) minFreq++;
        }

        ++node.freq;
        freqList[node.freq].push_front(key);
        if (freqList[node.freq].size() == 1) this->memory += sizeof(int);
        this->memory += sizeof(key);
        node.listIt = freqList[node.freq].begin();

        return node.value;
    }

    void print() override {
        for (const auto& [key, node] : cache) {
            std::cout << key << " : " << node.value << " (freq: " << node.freq << ")" << std::endl;
        }
    }
};

#endif //LFUCACHE_HPP
