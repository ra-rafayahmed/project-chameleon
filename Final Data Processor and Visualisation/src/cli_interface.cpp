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
    std::cout << "  ============================================\n";
    std::cout << "  PROJECT CHAMELEON - Data Analysis System\n";
    std::cout << "  ============================================\n";
    std::cout << "\n";
}

void CLIInterface::printSeparator() {
    std::cout << "  " << std::string(60, '-') << "\n";
}

void CLIInterface::printMenu() {
    // Menu is now in main.cpp with the correct structure
}

int CLIInterface::showMainMenu() {
    // Main menu is handled in main.cpp
    return 0;
}

void CLIInterface::printSubMenu(const std::string& title, const std::vector<std::string>& options) {
    std::cout << "\n  ============================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "  ============================================\n\n";
    for (size_t i = 0; i < options.size(); i++) {
        std::cout << "  " << (i + 1) << ". " << options[i] << "\n";
    }
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
    std::getline(std::cin, dummy);
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
    // Deprecated - functionality moved to main.cpp
}

void CLIInterface::showTrieMenu() {
    // Deprecated - functionality moved to main.cpp
}

void CLIInterface::showInvertedIndexMenu() {
    // Deprecated - functionality moved to main.cpp
}

void CLIInterface::showSegmentTreeMenu() {
    // Deprecated - functionality moved to main.cpp
}

void CLIInterface::showBloomFilterMenu() {
    // Deprecated - functionality moved to main.cpp
}

void CLIInterface::showMinHashMenu() {
    // Deprecated - functionality moved to main.cpp
}

void CLIInterface::showJsonParserMenu() {
    // Deprecated - functionality moved to main.cpp
}

void CLIInterface::showLevenshteinMenu() {
    // Deprecated - functionality moved to main.cpp
}

void CLIInterface::showDataAnalysisMenu() {
    // Deprecated - functionality moved to main.cpp
}

void CLIInterface::showSortingMenu() {
    // Deprecated - functionality moved to main.cpp
}

void CLIInterface::showHuffmanMenu() {
    // Deprecated - functionality moved to main.cpp
}

