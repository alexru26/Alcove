#include "FIFOCache.hpp"
#include "LFUCache.hpp"
#include "LRUCache.hpp"

int main() {
    FIFOCache<int, int> fifo(2);
    fifo.put(1, 1);
    fifo.put(2, 2);
    fifo.put(3, 3);
    fifo.print();
    std::cout << std::endl;

    LRUCache<int, int> lru(2);
    lru.put(1, 1);
    lru.put(2, 2);
    lru.get(1);
    lru.put(3, 3);
    lru.print();
    std::cout << std::endl;

    LFUCache<int, int> lfu(2);
    lfu.put(1, 1);
    lfu.put(2, 2);
    lfu.get(1);
    lfu.put(3, 3);
    lfu.print();

    return 0;
}
