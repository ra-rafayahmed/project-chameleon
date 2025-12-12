#ifndef MINHASH_H
#define MINHASH_H

#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <functional>

// minhash for similarity detection across text/json
class MinHash {
private:
    int numHashFunctions;
    std::vector<std::function<size_t(const std::string&)>> hashFunctions;
    
    // generate shingles (k-grams) from text
    std::unordered_set<std::string> getShingles(const std::string& text, int k = 3);
    
    // generate hash functions
    void initializeHashFunctions();

public:
    MinHash(int numHashFunctions = 128);
    
    // compute minhash signature for text
    std::vector<size_t> computeSignature(const std::string& text);
    
    // compute jaccard similarity between two signatures
    double jaccardSimilarity(const std::vector<size_t>& sig1, const std::vector<size_t>& sig2);
    
    // compute similarity between two texts
    double textSimilarity(const std::string& text1, const std::string& text2);
};

// locality sensitive hashing for approximate similarity search
class LSH {
private:
    int numBands;
    int numRows;
    std::unordered_map<std::string, std::vector<size_t>> signatures;
    std::unordered_map<std::string, std::vector<std::string>> buckets;
    MinHash minhash;

public:
    LSH(int numBands = 20, int numRows = 5);
    
    // add document to lsh index
    void addDocument(const std::string& docId, const std::string& text);
    
    // find similar documents
    std::vector<std::string> findSimilar(const std::string& docId, double threshold = 0.5);
    
    // find similar documents by text
    std::vector<std::string> findSimilarByText(const std::string& text, double threshold = 0.5);
};

#endif

