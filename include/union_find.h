#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <vector>
#include <unordered_map>
#include <string>

// disjoint set union (dsu) for unifying multi-source identities
class UnionFind {
private:
    std::unordered_map<std::string, std::string> parent;
    std::unordered_map<std::string, int> rank;

public:
    // find root of element with path compression
    std::string find(const std::string& x);
    
    // union two sets by rank
    void unite(const std::string& x, const std::string& y);
    
    // check if two elements are in same set
    bool connected(const std::string& x, const std::string& y);
    
    // get all groups (connected components)
    std::vector<std::vector<std::string>> getGroups();
    
    // add element to structure
    void addElement(const std::string& x);
    
    // get root representative for an element
    std::string getRoot(const std::string& x);
};

#endif

