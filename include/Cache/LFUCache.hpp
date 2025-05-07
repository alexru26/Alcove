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
        std::lock_guard<std::mutex> lock(this->mutex);
        if (this->capacity == 0) return;

        if (exists(key)) {
            Node& node = cache[key];
            node.value = value;
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
                minFreq++;
                this->memory -= sizeof(minFreq);
            }
        }

        freqList[1].push_front(key);
        if (minFreq != 1) this->memory += sizeof(minFreq);
        cache[key] = {value, 1, freqList[1].begin()};
        minFreq = 1;
        this->memory += 2 * sizeof(key) + sizeof(cache[key]);
    }

    Value get(Key key) override {
        std::lock_guard<std::mutex> lock(this->mutex);
        if (!exists(key)) return Value();

        Node& node = cache[key];
        int oldFreq = node.freq;

        freqList[oldFreq].erase(node.listIt);
        if (freqList[oldFreq].empty()) {
            freqList.erase(oldFreq);
            this->memory -= sizeof(oldFreq);
            if (minFreq == oldFreq) minFreq++;
        }

        ++node.freq;
        freqList[node.freq].push_front(key);
        if (freqList[node.freq].size() == 1) this->memory += sizeof(int);
        node.listIt = freqList[node.freq].begin();

        return node.value;
    }

    void print() override {
        std::cout << "Keys and Freqs: " << std::endl;
        for (const auto& [key, node] : cache) {
            std::cout << key << " : " << node.value << " (freq: " << node.freq << ")" << std::endl;
        }
        std::cout << "Frequency map: " << std::endl;
        for (const auto& [freq, node] : freqList) {
            std::cout << freq << " : ";
            for (const auto& key : node) {
                std::cout << key << " ";
            }
            std::cout << std::endl;
        }
    }
};

#endif //LFUCACHE_HPP
