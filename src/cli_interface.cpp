#include "../include/cli_interface.h"
#include <iostream>
#include <iomanip>
#include <limits>
#define NOMINMAX
#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

CLIInterface::CLIInterface() {}

void CLIInterface::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void CLIInterface::printHeader() {
    std::cout << "\n";
    std::cout << "================================================================\n";
    std::cout << "                                                                \n";
    std::cout << "        PROJECT CHAMELEON - DSA ANALYZER                        \n";
    std::cout << "                                                                \n";
    std::cout << "        Advanced Data Structures & Algorithms Suite            \n";
    std::cout << "                                                                \n";
    std::cout << "================================================================\n";
    std::cout << "\n";
}

void CLIInterface::printSeparator() {
    std::cout << "----------------------------------------------------------------\n";
}

void CLIInterface::printMenu() {
    printHeader();
    std::cout << "  MAIN MENU:\n";
    printSeparator();
    std::cout << "  1.  Graph + Union-Find (DSU) - Identity Unification\n";
    std::cout << "  2.  Trie - Username/Phone/Device ID Indexing\n";
    std::cout << "  3.  Inverted Index - Bio/Caption/JSON Analysis\n";
    std::cout << "  4.  Segment Tree + Sliding Window - RTT Analysis\n";
    std::cout << "  5.  Bloom Filter - Duplicate Detection\n";
    std::cout << "  6.  MinHash / LSH - Similarity Detection\n";
    std::cout << "  7.  JSON Parser (DFS) - Normalization\n";
    std::cout << "  8.  Levenshtein Distance - Fuzzy Matching\n";
    std::cout << "  9.  Data Analysis Dashboard\n";
    std::cout << "  10. Sorting Algorithms - Performance Analysis\n";
    std::cout << "  11. Huffman Compression - File Compression\n";
    std::cout << "  12. Exit\n";
    printSeparator();
}

int CLIInterface::showMainMenu() {
    clearScreen();
    printMenu();
    std::cout << "  Enter your choice (1-12): ";
    return getChoice(12);
}

void CLIInterface::printSubMenu(const std::string& title, const std::vector<std::string>& options) {
    printSeparator();
    std::cout << "  " << title << "\n";
    printSeparator();
    for (size_t i = 0; i < options.size(); i++) {
        std::cout << "  " << (i + 1) << ". " << options[i] << "\n";
    }
    std::cout << "  " << (options.size() + 1) << ". Back to Main Menu\n";
    printSeparator();
}

std::string CLIInterface::getInput(const std::string& prompt) {
    std::cout << "  " << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

int CLIInterface::getChoice(int maxChoice) {
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (choice < 1 || choice > maxChoice) {
        std::cout << "  Invalid choice. Please try again.\n";
        return -1;
    }
    
    return choice;
}

void CLIInterface::waitForEnter() {
    std::cout << "\n  Press Enter to continue...";
    std::string dummy;
    std::getline(std::cin, dummy);  // Use getline to handle both Enter key and newline from file
}

void CLIInterface::printResult(const std::string& title, const std::string& content) {
    printSeparator();
    std::cout << "  " << title << "\n";
    printSeparator();
    std::cout << content << "\n";
    printSeparator();
}

void CLIInterface::printTable(const std::vector<std::vector<std::string>>& data) {
    if (data.empty()) return;
    
    printSeparator();
    for (const auto& row : data) {
        std::cout << "  ";
        for (const auto& cell : row) {
            std::cout << std::setw(20) << std::left << cell;
        }
        std::cout << "\n";
    }
    printSeparator();
}

void CLIInterface::showUnionFindMenu() {
    clearScreen();
    printHeader();
    std::cout << "  UNION-FIND (DSU) - Identity Unification\n";
    // implementation will be in main.cpp
}

void CLIInterface::showTrieMenu() {
    clearScreen();
    printHeader();
    std::cout << "  TRIE - Indexing System\n";
    // implementation will be in main.cpp
}

void CLIInterface::showInvertedIndexMenu() {
    clearScreen();
    printHeader();
    std::cout << "  INVERTED INDEX - Text Analysis\n";
    // implementation will be in main.cpp
}

void CLIInterface::showSegmentTreeMenu() {
    clearScreen();
    printHeader();
    std::cout << "  SEGMENT TREE + SLIDING WINDOW - RTT Analysis\n";
    // implementation will be in main.cpp
}

void CLIInterface::showBloomFilterMenu() {
    clearScreen();
    printHeader();
    std::cout << "  BLOOM FILTER - Duplicate Detection\n";
    // implementation will be in main.cpp
}

void CLIInterface::showMinHashMenu() {
    clearScreen();
    printHeader();
    std::cout << "  MINHASH / LSH - Similarity Detection\n";
    // implementation will be in main.cpp
}

void CLIInterface::showJsonParserMenu() {
    clearScreen();
    printHeader();
    std::cout << "  JSON PARSER (DFS) - Normalization\n";
    // implementation will be in main.cpp
}

void CLIInterface::showLevenshteinMenu() {
    clearScreen();
    printHeader();
    std::cout << "  LEVENSHTEIN DISTANCE - Fuzzy Matching\n";
    // implementation will be in main.cpp
}

void CLIInterface::showDataAnalysisMenu() {
    clearScreen();
    printHeader();
    std::cout << "  DATA ANALYSIS DASHBOARD\n";
    // implementation will be in main.cpp
}

void CLIInterface::showSortingMenu() {
    clearScreen();
    printHeader();
    std::cout << "  SORTING ALGORITHMS - Performance Analysis\n";
    // implementation will be in main.cpp
}

void CLIInterface::showHuffmanMenu() {
    clearScreen();
    printHeader();
    std::cout << "  HUFFMAN COMPRESSION - File Compression\n";
    // implementation will be in main.cpp
}

