#include <iostream>
#include <fstream>
#include "PostfixConverter.h"
#include "Grammar.h"
#include <io.h>
#include <fcntl.h>
#include "AFN.h"
#include "AFD.h"
static std::string remove_bom(const std::string& input) {
    const std::string BOM = "\xEF\xBB\xBF";
    if (input.compare(0, BOM.size(), BOM) == 0) {
        return input.substr(BOM.size()); 
    }
    return input;
}
int main() {

    std::ifstream input("regex.txt");
    if (!input.is_open()) {
        std::cerr << "Eroare: Nu s-a putut deschide fisierul de intrare 'regex.txt'.\n";
        return 1;
    }

    std::string regex;
    input >> regex;
    input.close();
    regex = remove_bom(regex);


    std::cout << "Regex curatat: " << regex << "\n";

    std::string postfix;
    try {
        postfix = PostfixConverter::ConvertToPostfix(regex);
        std::cout << "Forma postfixata a expresiei: " << postfix << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    Grammar grammar(postfix);
    try {
        grammar.GenerateGrammar();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1; 
    }
    // grammar.PrintGrammar();
    AFN afn = grammar.GenerateAFN();
    // afn.PrintAFN();

    AFD afd = afn.ToDeterministic();
    try
    {
        afd.VerifyAutomaton();
    }
    catch(const std::exception & e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    std::cout << "Automatul finit determinist (AFD):\n";

    afd.PrintAFDTable();

    std::ofstream output("AFD.txt");
    if (!output.is_open()) {
        std::cerr << "Eroare: Nu s-a putut deschide fisierul de iesire 'afd_output.txt'.\n";
        return 1;
    }
    output.close();

    afd.SaveToFile("AFD.txt");

    std::cout << "Introduceti cuvinte pentru verificare (sau 'EXIT' pentru a iesi):\n";
    std::string word;
    while (true) {
        std::cout << "> ";
        std::cin >> word;
        if (word == "EXIT") break;

        if (afd.CheckWord(word)) {
            std::cout << "Cuvantul este ACCEPTAT.\n";
        }
        else {
            std::cout << "Cuvantul este RESPINS.\n";
        }
    }

    return 0;
}
