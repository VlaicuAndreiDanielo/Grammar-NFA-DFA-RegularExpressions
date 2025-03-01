#include "PostfixConverter.h"

std::string PostfixConverter::ConvertToPostfix(const std::string& regex)
{
    std::string augmented_regex = AddConcatenation(regex);

    std::string postfix;
    std::stack<char> operators;
    std::unordered_map<char, int> precedence = { {'*', 3}, {'.', 2}, {'|', 1}, {'(', 0} };
    int balance = 0;

    for (size_t i = 0; i < augmented_regex.size(); ++i) {
        char c = augmented_regex[i];

        if (IsOperand(c)) {
            postfix += c;
        }
        else if (c == '(') {
            operators.push(c);
            balance++;
        }
        else if (c == ')') {
            if (balance <= 0) {
                throw std::runtime_error("Eroare: Paranteze inchise fara a fi deschise.");
            }
            while (!operators.empty() && operators.top() != '(') {
                postfix += operators.top();
                operators.pop();
            }
            if (!operators.empty()) operators.pop(); 
            balance--;
        }
        else if (c == '*' || c == '.' || c == '|') {

            if (i == 0 && c != '(') { 
                throw std::runtime_error("Eroare: Operatorul " + std::string(1, c) + " nu poate aparea la inceput.");
            }
            if ((c == '|' || c == '.') && i == augmented_regex.size() - 1) { 
                throw std::runtime_error("Eroare: Operatorul " + std::string(1, c) + " nu poate aparea la sfarsit.");
            }
            if (i > 0 && (augmented_regex[i - 1] == '(' || augmented_regex[i - 1] == '|')) {
                throw std::runtime_error("Eroare: Operatorul " + std::string(1, c) + " nu poate aparea dupa '(' sau '|'.");
            }
            if (c == '|' && i + 1 < augmented_regex.size() && augmented_regex[i + 1] == ')') {
                throw std::runtime_error("Eroare: Operatorul '|' nu poate aparea inainte de ')'.");
            }
            while (!operators.empty() && precedence[operators.top()] >= precedence[c]) {
                postfix += operators.top();
                operators.pop();
            }
            operators.push(c);
        }
        else {

            throw std::runtime_error("Eroare: Caracter necunoscut '" + std::string(1, c) + "' in expresie.");
        }
    }

    if (balance != 0) {
        throw std::runtime_error("Eroare: Parantezele nu sunt echilibrate.");
    }

    while (!operators.empty()) {
        if (operators.top() == '(' || operators.top() == ')') {
            throw std::runtime_error("Eroare: Parantezele nu sunt echilibrate.");
        }
        postfix += operators.top();
        operators.pop();
    }

    return postfix;
}


std::string PostfixConverter::AddConcatenation(const std::string& regex)
{
    std::string result;
    for (size_t i = 0; i < regex.length(); ++i) {
        char c = regex[i];
        result += c;

        if (i + 1 < regex.length()) {
            char next = regex[i + 1];

            if ((std::isalnum(c) || c == ')' || c == '*') &&
                (std::isalnum(next) || next == '(')) {
                result += '.'; 
            }
        }
    }
    return result;
}

bool PostfixConverter::IsOperand(char c)
{
    return std::isalnum(c); 
}
