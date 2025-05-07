// LFU Cache implementation
// Alex Ru
// 5/13/25

#ifndef LFUCACHE_HPP
#define LFUCACHE_HPP

#include "Cache.h"

/**
 * LFU Cache class that uses LFU algorithm to store keys and values
 * @tparam Key key value type
 * @tparam Value corresponding value type
 */
template<typename Key, typename Value>
class LFUCache final : public Cache<Key, Value> {
private:
    // Node that contains the value, the frequency, and the location in the frequency list
    struct Node {
        Value value; // Corresponding value
        int freq; // Frequency of key and value
        typename std::list<Key>::iterator listIt; // Location of key in the freqList
    };

    int minFreq = 0; // Minimum frequency

    std::unordered_map<Key, Node> cache; // Cache node pairing
    std::unordered_map<int, std::list<Key>> freqList; // Frequency list where frequency maps to list of keys
public:
    /**
     * Explicit constructor that takes in capacity as parameter
     * @param capacity size of cache, aka amount it can hold
     */
    explicit LFUCache(size_t capacity) : Cache<Key, Value>(capacity) {}

    /**
     * Default destructor
     */
    ~LFUCache() override = default;

    /**
     * Copy constructor
     * @param other other LFU cache to copy
     */
    LFUCache(const LFUCache& other): Cache<Key, Value>(other) {
        this->cache = other.cache;
        this->freqList = other.freqList;
    }

    /**
     * Assignment operator
     * @param other other LFU cache to copy
     * @return copied LFU cache
     */
    LFUCache& operator=(const LFUCache& other) {
        this->cache = other.keys;
        this->freqList = other.freqList;
        return *this;
    }

    /**
     * Checks if key exists in LFU cache
     * @param key key value to check
     * @return whether key exists
     */
    bool exists(Key key) override {
        return cache.count(key);
    }

    /**
     * Stores key and value into the LFU cache
     * @param key key value
     * @param value corresponding value
     */
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

    /**
     * Returns the value given key
     * @param key key value
     * @return corresponding value
     */
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

    /**
     * Prints out the content of the LFU cache
     */
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
