#include "../include/union_find.h"
#include <string>
#include <unordered_map>

std::string UnionFind::find(const std::string& x) {
    if (parent.find(x) == parent.end()) {
        parent[x] = x;
        rank[x] = 0;
        return x;
    }

    // Path compression
    if (parent[x] != x) {
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

void UnionFind::Initialize(size_t n) {
    parent.clear();
    rank.clear();

    parent.reserve(n);
    rank.reserve(n);

    // Create n anonymous elements: "0", "1", ..., "n-1"
    for (size_t i = 0; i < n; ++i) {
        std::string id = std::to_string(i);
        parent[id] = id;
        rank[id] = 0;
    }
}

void UnionFind::unite(const std::string& x, const std::string& y) {
    std::string rootX = find(x);
    std::string rootY = find(y);

    if (rootX == rootY) return;

    // Union by rank
    if (rank[rootX] < rank[rootY]) {
        parent[rootX] = rootY;
    } 
    else if (rank[rootX] > rank[rootY]) {
        parent[rootY] = rootX;
    } 
    else {
        parent[rootY] = rootX;
        rank[rootX]++;
    }
}

// --- Compatibility wrappers for integer-based API ---
int UnionFind::Find(int x) {
    std::string id = std::to_string(x);
    std::string root = find(id);
    try {
        return std::stoi(root);
    } catch (...) {
        return -1;
    }
}

void UnionFind::Union(int x, int y) {
    unite(std::to_string(x), std::to_string(y));
}

bool UnionFind::connected(const std::string& x, const std::string& y) {
    return find(x) == find(y);
}

void UnionFind::addElement(const std::string& x) {
    if (parent.find(x) == parent.end()) {
        parent[x] = x;
        rank[x] = 0;
    }
}

std::string UnionFind::getRoot(const std::string& x) {
    return find(x);
}

std::vector<std::vector<std::string>> UnionFind::getGroups() {
    std::unordered_map<std::string, std::vector<std::string>> groups;

    for (const auto& pair : parent) {
        std::string root = find(pair.first);
        groups[root].push_back(pair.first);
    }

    std::vector<std::vector<std::string>> result;
    result.reserve(groups.size());

    for (const auto& pair : groups) {
        result.push_back(pair.second);
    }

    return result;
}
