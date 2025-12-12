#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <unordered_map>
#include <vector>
#include <string>
#include <set>

// inverted index for analyzing bios, captions, and json content
class InvertedIndex {
private:
    // word -> document ids that contain this word
    std::unordered_map<std::string, std::set<int>> index;
    
    // document id -> document content
    std::unordered_map<int, std::string> documents;
    
    int nextDocId;
    
    // tokenize text into words
    std::vector<std::string> tokenize(const std::string& text);

public:
    InvertedIndex();
    
    // add document to index
    int addDocument(const std::string& content);
    
    // search for documents containing query
    std::vector<int> search(const std::string& query);
    
    // search with multiple terms (AND)
    std::vector<int> searchAnd(const std::vector<std::string>& terms);
    
    // search with multiple terms (OR)
    std::vector<int> searchOr(const std::vector<std::string>& terms);
    
    // get document by id
    std::string getDocument(int docId);
    
    // get all indexed words
    std::vector<std::string> getAllTerms();
    
    // get document count for a term
    int getTermFrequency(const std::string& term);
};

#endif

