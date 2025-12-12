#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include <vector>
#include <algorithm>
#include <climits>

// segment tree for range queries on rtt data
class SegmentTree {
private:
    std::vector<int> tree;
    std::vector<int> data;
    int n;
    
    // build segment tree
    void build(int node, int start, int end);
    
    // query helper
    int queryHelper(int node, int start, int end, int l, int r, int op);
    
    // update helper
    void updateHelper(int node, int start, int end, int idx, int val);

public:
    SegmentTree(const std::vector<int>& arr);
    
    // range minimum query
    int rangeMin(int l, int r);
    
    // range maximum query
    int rangeMax(int l, int r);
    
    // range sum query
    int rangeSum(int l, int r);
    
    // update value at index
    void update(int idx, int val);
    
    // get all data
    std::vector<int> getData() const;
};

// sliding window for rtt analysis
class SlidingWindow {
private:
    std::vector<int> window;
    int windowSize;
    int currentSum;
    
public:
    SlidingWindow(int size);
    
    // add new value to window
    void add(int value);
    
    // get current window average
    double getAverage();
    
    // get current window min
    int getMin();
    
    // get current window max
    int getMax();
    
    // get current window sum
    int getSum();
    
    // get all values in window
    std::vector<int> getWindow();
    
    // check if window is full
    bool isFull();
};

#endif

