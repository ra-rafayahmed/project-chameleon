#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <stack>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <memory>
#include <chrono>
#include <iomanip>
#include <cstring>
#include <functional>
#include <curl/curl.h>

// ============================================================================
// SUPABASE CONNECTION & DATA LOADING
// ============================================================================

struct SupabaseConfig {
    std::string url = "https://qdsbdmikobhynjmfzweb.supabase.co";
    std::string api_key = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InFkc2JkbWlrb2JoeW5qbWZ6d2ViIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NjUzODE1NjQsImV4cCI6MjA4MDk1NzU2NH0.FAxUj8LLxdBxxBvRua_GX2d_QrhXsCQfu6uUUit8PSM";
};

// callback function for curl to write response data
struct WriteData {
    std::string data;
};

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    ((WriteData*)userp)->data.append((char*)contents, total_size);
    return total_size;
}

// fetch data from supabase using rest api
std::string fetchFromSupabase(const std::string& table_name, const SupabaseConfig& config) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "curl initialization failed" << std::endl;
        return "";
    }

    std::string url = config.url + "/rest/v1/" + table_name;
    WriteData response;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, ("apikey: " + config.api_key).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Prefer: return=representation");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "curl request failed: " << curl_easy_strerror(res) << std::endl;
        return "";
    }

    return response.data;
}

// ============================================================================
// SORTING ALGORITHMS (6+ implementations)
// ============================================================================

// 1. quicksort - divide and conquer, average o(n log n)
template<typename T>
int partition(std::vector<T>& arr, int low, int high) {
    T pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

template<typename T>
void quickSort(std::vector<T>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// 2. heapsort - heap data structure, o(n log n)
template<typename T>
void heapify(std::vector<T>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

template<typename T>
void heapSort(std::vector<T>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// 3. mergesort - divide and conquer, stable, o(n log n)
template<typename T>
void merge(std::vector<T>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<T> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

template<typename T>
void mergeSort(std::vector<T>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// 4. bubblesort - simple but slow, o(n^2)
template<typename T>
void bubbleSort(std::vector<T>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// 5. insertionsort - efficient for small arrays, o(n^2)
template<typename T>
void insertionSort(std::vector<T>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        T key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// 6. selectionsort - find min and swap, o(n^2)
template<typename T>
void selectionSort(std::vector<T>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        std::swap(arr[min_idx], arr[i]);
    }
}

// ============================================================================
// HUFFMAN COMPRESSION
// ============================================================================

struct HuffmanNode {
    char data;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char d, int f) : data(d), freq(f), left(nullptr), right(nullptr) {}
};

// comparator for priority queue
struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq;
    }
};

// build huffman tree from frequency map
HuffmanNode* buildHuffmanTree(const std::map<char, int>& freq_map) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> pq;

    for (const auto& pair : freq_map) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top();
        pq.pop();
        HuffmanNode* right = pq.top();
        pq.pop();

        HuffmanNode* merged = new HuffmanNode('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        pq.push(merged);
    }

    return pq.top();
}

// generate huffman codes by traversing tree
void generateCodes(HuffmanNode* root, std::string code, std::map<char, std::string>& codes) {
    if (!root) return;
    if (!root->left && !root->right) {
        codes[root->data] = code;
        return;
    }
    generateCodes(root->left, code + "0", codes);
    generateCodes(root->right, code + "1", codes);
}

// compress string using huffman encoding
std::string huffmanCompress(const std::string& data) {
    std::map<char, int> freq_map;
    for (char c : data) {
        freq_map[c]++;
    }

    HuffmanNode* root = buildHuffmanTree(freq_map);
    std::map<char, std::string> codes;
    generateCodes(root, "", codes);

    std::string compressed;
    for (char c : data) {
        compressed += codes[c];
    }

    // cleanup tree (simple recursive delete)
    std::function<void(HuffmanNode*)> deleteTree = [&](HuffmanNode* node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    };
    deleteTree(root);

    return compressed;
}

// save compressed data to file
void saveCompressedFile(const std::string& filename, const std::string& compressed_data) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file << compressed_data;
        file.close();
        std::cout << "compressed data saved to " << filename << std::endl;
    } else {
        std::cerr << "failed to open file for writing" << std::endl;
    }
}

// ============================================================================
// HASH TABLE IMPLEMENTATION
// ============================================================================

template<typename K, typename V>
class HashTable {
private:
    static const int TABLE_SIZE = 100;
    std::vector<std::pair<K, V>> table[TABLE_SIZE];

    int hashFunction(const K& key) {
        std::hash<K> hasher;
        return hasher(key) % TABLE_SIZE;
    }

public:
    void insert(const K& key, const V& value) {
        int index = hashFunction(key);
        for (auto& pair : table[index]) {
            if (pair.first == key) {
                pair.second = value;
                return;
            }
        }
        table[index].push_back({key, value});
    }

    V* get(const K& key) {
        int index = hashFunction(key);
        for (auto& pair : table[index]) {
            if (pair.first == key) {
                return &pair.second;
            }
        }
        return nullptr;
    }

    bool remove(const K& key) {
        int index = hashFunction(key);
        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->first == key) {
                table[index].erase(it);
                return true;
            }
        }
        return false;
    }
};

// ============================================================================
// BINARY SEARCH TREE
// ============================================================================

template<typename T>
class BinarySearchTree {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        Node(T d) : data(d), left(nullptr), right(nullptr) {}
    };

    Node* root;

    Node* insertRec(Node* node, T data) {
        if (!node) return new Node(data);
        if (data < node->data) {
            node->left = insertRec(node->left, data);
        } else if (data > node->data) {
            node->right = insertRec(node->right, data);
        }
        return node;
    }

    void inorderRec(Node* node, std::vector<T>& result) {
        if (node) {
            inorderRec(node->left, result);
            result.push_back(node->data);
            inorderRec(node->right, result);
        }
    }

public:
    BinarySearchTree() : root(nullptr) {}

    void insert(T data) {
        root = insertRec(root, data);
    }

    std::vector<T> inorder() {
        std::vector<T> result;
        inorderRec(root, result);
        return result;
    }
};

// ============================================================================
// GRAPH WITH BFS AND DFS
// ============================================================================

class Graph {
private:
    int vertices;
    std::vector<std::vector<int>> adj_list;

public:
    Graph(int v) : vertices(v) {
        adj_list.resize(v);
    }

    void addEdge(int u, int v) {
        adj_list[u].push_back(v);
        adj_list[v].push_back(u);
    }

    // breadth-first search
    std::vector<int> bfs(int start) {
        std::vector<int> result;
        std::vector<bool> visited(vertices, false);
        std::queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int node = q.front();
            q.pop();
            result.push_back(node);

            for (int neighbor : adj_list[node]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
        return result;
    }

    // depth-first search
    void dfsUtil(int node, std::vector<bool>& visited, std::vector<int>& result) {
        visited[node] = true;
        result.push_back(node);
        for (int neighbor : adj_list[node]) {
            if (!visited[neighbor]) {
                dfsUtil(neighbor, visited, result);
            }
        }
    }

    std::vector<int> dfs(int start) {
        std::vector<int> result;
        std::vector<bool> visited(vertices, false);
        dfsUtil(start, visited, result);
        return result;
    }
};

// ============================================================================
// QUEUE IMPLEMENTATION
// ============================================================================

template<typename T>
class Queue {
private:
    std::vector<T> data;
    int front_idx;
    int rear_idx;

public:
    Queue() : front_idx(0), rear_idx(-1) {}

    void enqueue(T item) {
        data.push_back(item);
        rear_idx++;
    }

    T dequeue() {
        if (isEmpty()) {
            throw std::runtime_error("queue is empty");
        }
        T item = data[front_idx];
        front_idx++;
        return item;
    }

    bool isEmpty() {
        return front_idx > rear_idx;
    }

    T front() {
        if (isEmpty()) {
            throw std::runtime_error("queue is empty");
        }
        return data[front_idx];
    }
};

// ============================================================================
// STACK IMPLEMENTATION
// ============================================================================

template<typename T>
class Stack {
private:
    std::vector<T> data;

public:
    void push(T item) {
        data.push_back(item);
    }

    T pop() {
        if (isEmpty()) {
            throw std::runtime_error("stack is empty");
        }
        T item = data.back();
        data.pop_back();
        return item;
    }

    T top() {
        if (isEmpty()) {
            throw std::runtime_error("stack is empty");
        }
        return data.back();
    }

    bool isEmpty() {
        return data.empty();
    }
};

// ============================================================================
// CIRCULAR BUFFER
// ============================================================================

template<typename T>
class CircularBuffer {
private:
    std::vector<T> buffer;
    int head;
    int tail;
    int size;
    int capacity;

public:
    CircularBuffer(int cap) : capacity(cap), head(0), tail(0), size(0) {
        buffer.resize(capacity);
    }

    void push(T item) {
        buffer[tail] = item;
        tail = (tail + 1) % capacity;
        if (size < capacity) {
            size++;
        } else {
            head = (head + 1) % capacity;
        }
    }

    T pop() {
        if (isEmpty()) {
            throw std::runtime_error("circular buffer is empty");
        }
        T item = buffer[head];
        head = (head + 1) % capacity;
        size--;
        return item;
    }

    bool isEmpty() {
        return size == 0;
    }

    bool isFull() {
        return size == capacity;
    }
};

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    std::cout << "=== PROJECT CHAMELEON - DSA IMPLEMENTATIONS ===" << std::endl;
    std::cout << std::endl;

    // initialize curl
    curl_global_init(CURL_GLOBAL_DEFAULT);

    SupabaseConfig config;
    
    // load data from supabase
    std::cout << "loading data from supabase..." << std::endl;
    std::string users_data = fetchFromSupabase("users", config);
    std::string profiles_data = fetchFromSupabase("instagram_profiles", config);
    std::string stories_data = fetchFromSupabase("instagram_stories", config);
    std::string devices_data = fetchFromSupabase("game_devices", config);
    std::string events_data = fetchFromSupabase("whatsapp_events", config);

    // create sample data for sorting and compression
    std::vector<int> numbers = {64, 34, 25, 12, 22, 11, 90, 5, 77, 88, 3, 45, 67, 89, 23};
    std::vector<int> numbers_copy;

    std::cout << "\noriginal data: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << std::endl << std::endl;

    // demonstrate all sorting algorithms
    std::cout << "=== SORTING ALGORITHMS ===" << std::endl;

    // 1. quicksort
    numbers_copy = numbers;
    auto start = std::chrono::high_resolution_clock::now();
    quickSort(numbers_copy, 0, numbers_copy.size() - 1);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "1. quicksort: ";
    for (int n : numbers_copy) std::cout << n << " ";
    std::cout << " (time: " << duration.count() << " microseconds)" << std::endl;

    // 2. heapsort
    numbers_copy = numbers;
    start = std::chrono::high_resolution_clock::now();
    heapSort(numbers_copy);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "2. heapsort: ";
    for (int n : numbers_copy) std::cout << n << " ";
    std::cout << " (time: " << duration.count() << " microseconds)" << std::endl;

    // 3. mergesort
    numbers_copy = numbers;
    start = std::chrono::high_resolution_clock::now();
    mergeSort(numbers_copy, 0, numbers_copy.size() - 1);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "3. mergesort: ";
    for (int n : numbers_copy) std::cout << n << " ";
    std::cout << " (time: " << duration.count() << " microseconds)" << std::endl;

    // 4. bubblesort
    numbers_copy = numbers;
    start = std::chrono::high_resolution_clock::now();
    bubbleSort(numbers_copy);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "4. bubblesort: ";
    for (int n : numbers_copy) std::cout << n << " ";
    std::cout << " (time: " << duration.count() << " microseconds)" << std::endl;

    // 5. insertionsort
    numbers_copy = numbers;
    start = std::chrono::high_resolution_clock::now();
    insertionSort(numbers_copy);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "5. insertionsort: ";
    for (int n : numbers_copy) std::cout << n << " ";
    std::cout << " (time: " << duration.count() << " microseconds)" << std::endl;

    // 6. selectionsort
    numbers_copy = numbers;
    start = std::chrono::high_resolution_clock::now();
    selectionSort(numbers_copy);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "6. selectionsort: ";
    for (int n : numbers_copy) std::cout << n << " ";
    std::cout << " (time: " << duration.count() << " microseconds)" << std::endl;

    std::cout << std::endl;

    // huffman compression
    std::cout << "=== HUFFMAN COMPRESSION ===" << std::endl;
    std::string sample_text = "project chameleon data compression test string with repeated characters";
    std::cout << "original text: " << sample_text << std::endl;
    std::cout << "original size: " << sample_text.length() << " bytes" << std::endl;
    
    std::string compressed = huffmanCompress(sample_text);
    std::cout << "compressed binary: " << compressed.substr(0, 100) << "..." << std::endl;
    std::cout << "compressed size: " << (compressed.length() + 7) / 8 << " bytes" << std::endl;
    
    saveCompressedFile("compressed_data.bin", compressed);
    std::cout << std::endl;

    // hash table demonstration
    std::cout << "=== HASH TABLE ===" << std::endl;
    HashTable<std::string, int> hash_table;
    hash_table.insert("user1", 100);
    hash_table.insert("user2", 200);
    hash_table.insert("user3", 300);
    
    int* value = hash_table.get("user2");
    if (value) {
        std::cout << "retrieved value for user2: " << *value << std::endl;
    }
    std::cout << std::endl;

    // binary search tree demonstration
    std::cout << "=== BINARY SEARCH TREE ===" << std::endl;
    BinarySearchTree<int> bst;
    bst.insert(50);
    bst.insert(30);
    bst.insert(70);
    bst.insert(20);
    bst.insert(40);
    bst.insert(60);
    bst.insert(80);
    
    std::vector<int> bst_result = bst.inorder();
    std::cout << "bst inorder traversal: ";
    for (int n : bst_result) std::cout << n << " ";
    std::cout << std::endl << std::endl;

    // graph with bfs and dfs
    std::cout << "=== GRAPH (BFS & DFS) ===" << std::endl;
    Graph g(5);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    
    std::vector<int> bfs_result = g.bfs(0);
    std::cout << "bfs from node 0: ";
    for (int n : bfs_result) std::cout << n << " ";
    std::cout << std::endl;
    
    std::vector<int> dfs_result = g.dfs(0);
    std::cout << "dfs from node 0: ";
    for (int n : dfs_result) std::cout << n << " ";
    std::cout << std::endl << std::endl;

    // queue demonstration
    std::cout << "=== QUEUE ===" << std::endl;
    Queue<int> queue;
    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);
    std::cout << "dequeued: " << queue.dequeue() << std::endl;
    std::cout << "front element: " << queue.front() << std::endl;
    std::cout << std::endl;

    // stack demonstration
    std::cout << "=== STACK ===" << std::endl;
    Stack<int> stack;
    stack.push(10);
    stack.push(20);
    stack.push(30);
    std::cout << "popped: " << stack.pop() << std::endl;
    std::cout << "top element: " << stack.top() << std::endl;
    std::cout << std::endl;

    // circular buffer demonstration
    std::cout << "=== CIRCULAR BUFFER ===" << std::endl;
    CircularBuffer<int> cb(5);
    for (int i = 1; i <= 7; i++) {
        cb.push(i);
    }
    std::cout << "circular buffer contents (after pushing 7 items into size 5): ";
    while (!cb.isEmpty()) {
        std::cout << cb.pop() << " ";
    }
    std::cout << std::endl << std::endl;

    // cleanup
    curl_global_cleanup();

    std::cout << "=== ALL OPERATIONS COMPLETED ===" << std::endl;
    return 0;
}

