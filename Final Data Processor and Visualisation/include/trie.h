#ifndef TRIE_H
#define TRIE_H

#include <unordered_map>
#include <vector>
#include <string>

// trie for indexing usernames, phones, and device ids
class Trie {
private:
    struct TrieNode {
        std::unordered_map<char, TrieNode*> children;
        bool isEndOfWord;
        std::vector<std::string> data; // store associated data
        
        TrieNode() : isEndOfWord(false) {}
    };
    
    TrieNode* root;

    // helper for prefix search
    void getAllWords(TrieNode* node, const std::string& prefix, std::vector<std::string>& results);
    
    // cleanup helper
    void deleteNode(TrieNode* node);

public:
    Trie();
    ~Trie();
    
    // insert a key with associated data
    void insert(const std::string& key, const std::string& data);
    
    // search for exact match
    bool search(const std::string& key);
    
    // get all data associated with key
    std::vector<std::string> getData(const std::string& key);
    
    // prefix search - find all keys starting with prefix
    std::vector<std::string> prefixSearch(const std::string& prefix);
    
    // delete a key
    bool remove(const std::string& key);
    
    // get all keys
    std::vector<std::string> getAllKeys();
};

#endif

