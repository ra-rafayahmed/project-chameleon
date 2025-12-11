#ifndef CLI_INTERFACE_H
#define CLI_INTERFACE_H

#include <string>
#include <vector>

// beautiful cli interface for project chameleon
class CLIInterface {
private:
    void printHeader();
    void printSeparator();
    void printMenu();
    void printSubMenu(const std::string& title, const std::vector<std::string>& options);
    void clearScreen();
    
public:
    CLIInterface();
    void waitForEnter();
    
    // main menu
    int showMainMenu();
    
    // sub menus
    void showUnionFindMenu();
    void showTrieMenu();
    void showInvertedIndexMenu();
    void showSegmentTreeMenu();
    void showBloomFilterMenu();
    void showMinHashMenu();
    void showJsonParserMenu();
    void showLevenshteinMenu();
    void showDataAnalysisMenu();
    void showSortingMenu();
    void showHuffmanMenu();
    
    // utility functions
    std::string getInput(const std::string& prompt);
    int getChoice(int maxChoice);
    void printResult(const std::string& title, const std::string& content);
    void printTable(const std::vector<std::vector<std::string>>& data);
};

#endif

