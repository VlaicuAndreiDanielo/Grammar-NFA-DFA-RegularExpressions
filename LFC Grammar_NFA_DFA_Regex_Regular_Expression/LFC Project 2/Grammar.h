#pragma once
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
#include "AFN.h"
#include <io.h>
#include <fcntl.h>

class Grammar {
private:
    std::unordered_map<std::string, std::vector<std::string>> productions;
    int non_terminal_count = 0;
    std::string postfix;

    std::string GenerateNewNonTerminal();

public:
    Grammar(const std::string& postfix_expression) : postfix(postfix_expression) {}

    void GenerateGrammar();
    void PrintGrammar() const;

    AFN GenerateAFN();
};
