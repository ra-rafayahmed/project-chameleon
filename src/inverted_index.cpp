#include "../include/inverted_index.h"
#include <sstream>
#include <algorithm>
#include <cctype>

InvertedIndex::InvertedIndex() : nextDocId(1) {}

std::vector<std::string> InvertedIndex::tokenize(const std::string& text) {
    std::vector<std::string> tokens;
    std::istringstream iss(text);
    std::string token;
    
    while (iss >> token) {
        // convert to lowercase and remove punctuation
        std::transform(token.begin(), token.end(), token.begin(), ::tolower);
        token.erase(std::remove_if(token.begin(), token.end(), 
            [](char c) { return !std::isalnum(c); }), token.end());
        
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    
    return tokens;
}

int InvertedIndex::addDocument(const std::string& content) {
    int docId = nextDocId++;
    documents[docId] = content;
    
    std::vector<std::string> tokens = tokenize(content);
    for (const std::string& token : tokens) {
        index[token].insert(docId);
    }
    
    return docId;
}

std::vector<int> InvertedIndex::search(const std::string& query) {
    std::vector<std::string> terms = tokenize(query);
    if (terms.empty()) return {};
    
    if (terms.size() == 1) {
        if (index.find(terms[0]) != index.end()) {
            return std::vector<int>(index[terms[0]].begin(), index[terms[0]].end());
        }
        return {};
    }
    
    return searchAnd(terms);
}

std::vector<int> InvertedIndex::searchAnd(const std::vector<std::string>& terms) {
    if (terms.empty()) return {};
    
    std::set<int> result;
    bool first = true;
    
    for (const std::string& term : terms) {
        if (index.find(term) == index.end()) {
            return {}; // term not found, no results
        }
        
        if (first) {
            result = index[term];
            first = false;
        } else {
            std::set<int> intersection;
            std::set_intersection(result.begin(), result.end(),
                                index[term].begin(), index[term].end(),
                                std::inserter(intersection, intersection.begin()));
            result = intersection;
        }
    }
    
    return std::vector<int>(result.begin(), result.end());
}

std::vector<int> InvertedIndex::searchOr(const std::vector<std::string>& terms) {
    std::set<int> result;
    
    for (const std::string& term : terms) {
        if (index.find(term) != index.end()) {
            result.insert(index[term].begin(), index[term].end());
        }
    }
    
    return std::vector<int>(result.begin(), result.end());
}

std::string InvertedIndex::getDocument(int docId) {
    if (documents.find(docId) != documents.end()) {
        return documents[docId];
    }
    return "";
}

std::vector<std::string> InvertedIndex::getAllTerms() {
    std::vector<std::string> terms;
    for (const auto& pair : index) {
        terms.push_back(pair.first);
    }
    return terms;
}

int InvertedIndex::getTermFrequency(const std::string& term) {
    if (index.find(term) != index.end()) {
        return index[term].size();
    }
    return 0;
}

