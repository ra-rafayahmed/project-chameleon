#include "../include/bloom_filter.h"
#include <algorithm>

BloomFilter::BloomFilter(int size, int numHashFunctions) 
    : size(size), numHashFunctions(numHashFunctions) {
    bitArray.resize(size, false);
}

std::vector<size_t> BloomFilter::getHashValues(const std::string& item) {
    std::vector<size_t> hashes;
    size_t hash1_val = hash1(item);
    
    // generate multiple hash values using different seeds
    for (int i = 0; i < numHashFunctions; i++) {
        size_t hash = hash1_val;
        // simple hash combination
        for (size_t j = 0; j < item.length(); j++) {
            hash = hash * 31 + item[j] + i * 17;
        }
        hashes.push_back(hash % size);
    }
    
    return hashes;
}

void BloomFilter::add(const std::string& item) {
    std::vector<size_t> hashes = getHashValues(item);
    for (size_t hash : hashes) {
        bitArray[hash] = true;
    }
}

bool BloomFilter::mightContain(const std::string& item) {
    std::vector<size_t> hashes = getHashValues(item);
    for (size_t hash : hashes) {
        if (!bitArray[hash]) {
            return false; // definitely not in set
        }
    }
    return true; // might be in set (could be false positive)
}

int BloomFilter::getSize() const {
    return size;
}

int BloomFilter::getNumHashFunctions() const {
    return numHashFunctions;
}

void BloomFilter::clear() {
    std::fill(bitArray.begin(), bitArray.end(), false);
}

