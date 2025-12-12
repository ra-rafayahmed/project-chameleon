#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <vector>
#include <bitset>
#include <functional>
#include <string>

// bloom filter for fast duplicate detection in ingestion
class BloomFilter {
private:
    std::vector<bool> bitArray;
    int size;
    int numHashFunctions;
    
    // hash functions
    std::hash<std::string> hash1;
    
    // generate multiple hash values from single hash
    std::vector<size_t> getHashValues(const std::string& item);

public:
    BloomFilter(int size, int numHashFunctions = 3);
    
    // add item to filter
    void add(const std::string& item);
    
    // check if item might be in filter (may have false positives)
    bool mightContain(const std::string& item);
    
    // get current size
    int getSize() const;
    
    // get number of hash functions
    int getNumHashFunctions() const;
    
    // clear filter
    void clear();
};

#endif

