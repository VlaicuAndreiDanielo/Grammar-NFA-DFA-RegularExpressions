#pragma once
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>

class PostfixConverter {
public:
    static std::string ConvertToPostfix(const std::string& regex);

private:
    static std::string AddConcatenation(const std::string& regex);

    static bool IsOperand(char c);
};
