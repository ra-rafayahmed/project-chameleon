#include "../include/trie.h"

Trie::Trie() {
    root = new TrieNode();
}

Trie::~Trie() {
    deleteNode(root);
}

void Trie::deleteNode(TrieNode* node) {
    if (!node) return;
    for (auto& pair : node->children) {
        deleteNode(pair.second);
    }
    delete node;
}

void Trie::insert(const std::string& key, const std::string& data) {
    TrieNode* current = root;
    for (char c : key) {
        if (current->children.find(c) == current->children.end()) {
            current->children[c] = new TrieNode();
        }
        current = current->children[c];
    }
    current->isEndOfWord = true;
    current->data.push_back(data);
}

bool Trie::search(const std::string& key) {
    TrieNode* current = root;
    for (char c : key) {
        if (current->children.find(c) == current->children.end()) {
            return false;
        }
        current = current->children[c];
    }
    return current->isEndOfWord;
}

std::vector<std::string> Trie::getData(const std::string& key) {
    TrieNode* current = root;
    for (char c : key) {
        if (current->children.find(c) == current->children.end()) {
            return {};
        }
        current = current->children[c];
    }
    if (current->isEndOfWord) {
        return current->data;
    }
    return {};
}

void Trie::getAllWords(TrieNode* node, const std::string& prefix, std::vector<std::string>& results) {
    if (!node) return;
    
    if (node->isEndOfWord) {
        results.push_back(prefix);
    }
    
    for (const auto& pair : node->children) {
        getAllWords(pair.second, prefix + pair.first, results);
    }
}

std::vector<std::string> Trie::prefixSearch(const std::string& prefix) {
    std::vector<std::string> results;
    TrieNode* current = root;
    
    // navigate to prefix node
    for (char c : prefix) {
        if (current->children.find(c) == current->children.end()) {
            return results; // prefix not found
        }
        current = current->children[c];
    }
    
    // get all words from this node
    getAllWords(current, prefix, results);
    return results;
}

bool Trie::remove(const std::string& key) {
    // simplified removal - just mark as not end of word
    TrieNode* current = root;
    for (char c : key) {
        if (current->children.find(c) == current->children.end()) {
            return false;
        }
        current = current->children[c];
    }
    if (current->isEndOfWord) {
        current->isEndOfWord = false;
        current->data.clear();
        return true;
    }
    return false;
}

std::vector<std::string> Trie::getAllKeys() {
    std::vector<std::string> results;
    getAllWords(root, "", results);
    return results;
}

