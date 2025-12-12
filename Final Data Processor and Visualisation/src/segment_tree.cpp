#include "../include/segment_tree.h"

SegmentTree::SegmentTree(const std::vector<int>& arr) : data(arr), n(arr.size()) {
    if (n == 0) {
        tree.clear();
        return; // handle empty array
    }
    try {
        // Ensure tree size is sufficient (4*n is standard for segment trees)
        size_t treeSize = 4 * static_cast<size_t>(n);
        if (treeSize > 1000000) { // Safety limit
            treeSize = 1000000;
        }
        tree.resize(treeSize, INT_MAX);
        build(1, 0, n - 1);
    } catch (...) {
        tree.clear();
        n = 0;
    }
}

void SegmentTree::build(int node, int start, int end) {
    if (start > end || node < 0 || node >= tree.size()) {
        return; // Safety check
    }
    if (start == end) {
        if (start >= 0 && start < n && node < tree.size()) {
            tree[node] = data[start];
        }
    } else {
        int mid = (start + end) / 2;
        int leftNode = 2 * node;
        int rightNode = 2 * node + 1;
        
        if (leftNode < tree.size() && start <= mid) {
            build(leftNode, start, mid);
        }
        if (rightNode < tree.size() && mid + 1 <= end) {
            build(rightNode, mid + 1, end);
        }
        
        if (leftNode < tree.size() && rightNode < tree.size()) {
            tree[node] = std::min(tree[leftNode], tree[rightNode]);
        } else if (leftNode < tree.size()) {
            tree[node] = tree[leftNode];
        } else if (rightNode < tree.size()) {
            tree[node] = tree[rightNode];
        }
    }
}

int SegmentTree::queryHelper(int node, int start, int end, int l, int r, int op) {
    if (node < 0 || node >= tree.size() || start > end || n == 0) {
        return (op == 0) ? INT_MAX : (op == 1) ? INT_MIN : 0;
    }
    if (r < start || end < l) {
        return (op == 0) ? INT_MAX : (op == 1) ? INT_MIN : 0; // min, max, sum
    }
    if (l <= start && end <= r) {
        if (node < tree.size()) {
            if (op == 2) { // sum
                return tree[node];
            }
            return tree[node];
        }
        return (op == 0) ? INT_MAX : (op == 1) ? INT_MIN : 0;
    }
    int mid = (start + end) / 2;
    int leftNode = 2 * node;
    int rightNode = 2 * node + 1;
    
    int left = (op == 0) ? INT_MAX : (op == 1) ? INT_MIN : 0;
    int right = (op == 0) ? INT_MAX : (op == 1) ? INT_MIN : 0;
    
    if (leftNode < tree.size() && start <= mid) {
        left = queryHelper(leftNode, start, mid, l, r, op);
    }
    if (rightNode < tree.size() && mid + 1 <= end) {
        right = queryHelper(rightNode, mid + 1, end, l, r, op);
    }
    
    if (op == 0) return std::min(left, right);
    if (op == 1) return std::max(left, right);
    return left + right;
}

int SegmentTree::rangeMin(int l, int r) {
    if (n == 0 || l < 0 || r >= n || l > r) {
        return INT_MAX;
    }
    return queryHelper(1, 0, n - 1, l, r, 0);
}

int SegmentTree::rangeMax(int l, int r) {
    // for max, we need a separate tree or rebuild
    // simplified: use min tree structure but calculate max differently
    if (n == 0 || l < 0 || r >= n || l > r) {
        return INT_MIN;
    }
    int maxVal = INT_MIN;
    for (int i = l; i <= r && i < n; i++) {
        maxVal = std::max(maxVal, data[i]);
    }
    return maxVal;
}

int SegmentTree::rangeSum(int l, int r) {
    if (n == 0 || l < 0 || r >= n || l > r) {
        return 0;
    }
    int sum = 0;
    for (int i = l; i <= r && i < n; i++) {
        sum += data[i];
    }
    return sum;
}

void SegmentTree::updateHelper(int node, int start, int end, int idx, int val) {
    if (start == end) {
        data[idx] = val;
        tree[node] = val;
    } else {
        int mid = (start + end) / 2;
        if (idx <= mid) {
            updateHelper(2 * node, start, mid, idx, val);
        } else {
            updateHelper(2 * node + 1, mid + 1, end, idx, val);
        }
        tree[node] = std::min(tree[2 * node], tree[2 * node + 1]);
    }
}

void SegmentTree::update(int idx, int val) {
    if (n == 0 || idx < 0 || idx >= n) {
        return;
    }
    updateHelper(1, 0, n - 1, idx, val);
}

std::vector<int> SegmentTree::getData() const {
    return data;
}

// sliding window implementation
SlidingWindow::SlidingWindow(int size) : windowSize(size), currentSum(0) {}

void SlidingWindow::add(int value) {
    window.push_back(value);
    currentSum += value;
    if (window.size() > windowSize) {
        currentSum -= window[0];
        window.erase(window.begin());
    }
}

double SlidingWindow::getAverage() {
    if (window.empty()) return 0.0;
    return static_cast<double>(currentSum) / window.size();
}

int SlidingWindow::getMin() {
    if (window.empty()) return INT_MAX;
    return *std::min_element(window.begin(), window.end());
}

int SlidingWindow::getMax() {
    if (window.empty()) return INT_MIN;
    return *std::max_element(window.begin(), window.end());
}

int SlidingWindow::getSum() {
    return currentSum;
}

std::vector<int> SlidingWindow::getWindow() {
    return window;
}

bool SlidingWindow::isFull() {
    return window.size() >= windowSize;
}

