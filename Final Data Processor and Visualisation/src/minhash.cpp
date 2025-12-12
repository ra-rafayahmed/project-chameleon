#include "../include/minhash.h"
#include <sstream>
#include <climits>
#include <limits>

MinHash::MinHash(int numHashFunctions) : numHashFunctions(numHashFunctions) {
    initializeHashFunctions();
}

void MinHash::initializeHashFunctions() {
    for (int i = 0; i < numHashFunctions; i++) {
        int seed = i * 17 + 31;
        hashFunctions.push_back([seed](const std::string& s) {
            size_t hash = seed;
            for (char c : s) {
                hash = hash * 31 + c;
            }
            return hash;
        });
    }
}

std::unordered_set<std::string> MinHash::getShingles(const std::string& text, int k) {
    std::unordered_set<std::string> shingles;
    if (text.length() < k) {
        shingles.insert(text);
        return shingles;
    }
    
    for (size_t i = 0; i <= text.length() - k; i++) {
        shingles.insert(text.substr(i, k));
    }
    
    return shingles;
}

std::vector<size_t> MinHash::computeSignature(const std::string& text) {
    std::unordered_set<std::string> shingles = getShingles(text);
    std::vector<size_t> signature(numHashFunctions, std::numeric_limits<size_t>::max());
    
    for (const std::string& shingle : shingles) {
        for (int i = 0; i < numHashFunctions; i++) {
            size_t hash = hashFunctions[i](shingle);
            if (hash < signature[i]) {
                signature[i] = hash;
            }
        }
    }
    
    return signature;
}

double MinHash::jaccardSimilarity(const std::vector<size_t>& sig1, const std::vector<size_t>& sig2) {
    if (sig1.size() != sig2.size()) return 0.0;
    
    int matches = 0;
    for (size_t i = 0; i < sig1.size(); i++) {
        if (sig1[i] == sig2[i]) {
            matches++;
        }
    }
    
    return static_cast<double>(matches) / sig1.size();
}

double MinHash::textSimilarity(const std::string& text1, const std::string& text2) {
    std::vector<size_t> sig1 = computeSignature(text1);
    std::vector<size_t> sig2 = computeSignature(text2);
    return jaccardSimilarity(sig1, sig2);
}

// lsh implementation
LSH::LSH(int numBands, int numRows) : numBands(numBands), numRows(numRows), minhash(128) {}

void LSH::addDocument(const std::string& docId, const std::string& text) {
    std::vector<size_t> signature = minhash.computeSignature(text);
    signatures[docId] = signature;
    
    // hash bands into buckets
    for (int band = 0; band < numBands; band++) {
        std::string bandHash = "";
        int start = band * numRows;
        int end = std::min(start + numRows, (int)signature.size());
        
        for (int i = start; i < end; i++) {
            bandHash += std::to_string(signature[i]) + "_";
        }
        
        buckets[bandHash].push_back(docId);
    }
}

std::vector<std::string> LSH::findSimilar(const std::string& docId, double threshold) {
    if (signatures.find(docId) == signatures.end()) {
        return {};
    }
    
    std::vector<size_t> sig1 = signatures[docId];
    std::unordered_set<std::string> candidates;
    
    // find candidates from buckets
    for (int band = 0; band < numBands; band++) {
        std::string bandHash = "";
        int start = band * numRows;
        int end = std::min(start + numRows, (int)sig1.size());
        
        for (int i = start; i < end; i++) {
            bandHash += std::to_string(sig1[i]) + "_";
        }
        
        if (buckets.find(bandHash) != buckets.end()) {
            for (const std::string& candidate : buckets[bandHash]) {
                if (candidate != docId) {
                    candidates.insert(candidate);
                }
            }
        }
    }
    
    // verify similarity
    std::vector<std::string> results;
    for (const std::string& candidate : candidates) {
        if (signatures.find(candidate) != signatures.end()) {
            double similarity = minhash.jaccardSimilarity(sig1, signatures[candidate]);
            if (similarity >= threshold) {
                results.push_back(candidate);
            }
        }
    }
    
    return results;
}

std::vector<std::string> LSH::findSimilarByText(const std::string& text, double threshold) {
    std::vector<size_t> sig1 = minhash.computeSignature(text);
    std::vector<std::string> results;
    
    for (const auto& pair : signatures) {
        double similarity = minhash.jaccardSimilarity(sig1, pair.second);
        if (similarity >= threshold) {
            results.push_back(pair.first);
        }
    }
    
    return results;
}

