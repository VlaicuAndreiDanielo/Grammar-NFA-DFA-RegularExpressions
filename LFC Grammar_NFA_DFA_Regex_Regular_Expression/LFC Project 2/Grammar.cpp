#include "Grammar.h"

std::string Grammar::GenerateNewNonTerminal()
{
    return "N" + std::to_string(non_terminal_count++);
}

void Grammar::GenerateGrammar()
{
    std::stack<std::string> stack;
    int nt_count = 0;
    std::unordered_map<std::string, std::string> terminal_map;

    auto generate_new_non_terminal = [&]() {
        return "N" + std::to_string(nt_count++);
        };

    for (char c : postfix) {
        if (isalnum(static_cast<unsigned char>(c))) {
            std::string terminal(1, c);
            if (terminal_map.find(terminal) == terminal_map.end()) {
                std::string nt = generate_new_non_terminal();
                productions[nt] = { terminal };
                terminal_map[terminal] = nt;
            }
            stack.push(terminal_map[terminal]);
        }
        else if (c == '|') {
            if (stack.size() < 2) {
                throw std::runtime_error("Eroare: Alternarea necesita doua elemente pe stiva.");
            }
            std::string b = stack.top(); stack.pop();
            std::string a = stack.top(); stack.pop();
            std::string nt = generate_new_non_terminal();
            productions[nt] = { a, b };
            stack.push(nt);
        }
        else if (c == '.') {
            if (stack.size() < 2) {
                throw std::runtime_error("Eroare: Concatenarea necesita doua elemente pe stiva.");
            }
            std::string b = stack.top(); stack.pop();
            std::string a = stack.top(); stack.pop();
            std::string nt = generate_new_non_terminal();
            productions[nt] = { a + " " + b };
            stack.push(nt);
        }
        else if (c == '*') {
            if (stack.size() < 1) {
                throw std::runtime_error("Eroare: Kleene star necesita un element pe stiva.");
            }
            std::string a = stack.top(); stack.pop();
            std::string nt = generate_new_non_terminal();
            productions[nt] = { a + " " + nt, "ε" };
            stack.push(nt);
        }
    }

    if (!stack.empty()) {
        std::vector<std::string> start_productions;

        while (!stack.empty()) {
            start_productions.push_back(stack.top());
            stack.pop();
        }

        std::reverse(start_productions.begin(), start_productions.end());

        productions["S"] = start_productions;
    }
    else {
        throw std::runtime_error("Eroare: Stiva este goala la final.");
    }
}

void Grammar::PrintGrammar() const
{

    _setmode(_fileno(stdout), _O_U16TEXT);

    std::wcout << L"Gramatica generată:\n";
    for (const auto& [non_terminal, productions] : productions) {
        std::wcout << non_terminal.c_str() << L" -> ";
        for (size_t i = 0; i < productions.size(); ++i) {
            if (productions[i] == "λ") {
                std::wcout << L"λ";
            }
            else {
                std::wcout << productions[i].c_str();
            }
            if (i != productions.size() - 1) {
                std::wcout << L" | ";
            }
        }
        std::wcout << L"\n";
    }

    _setmode(_fileno(stdout), _O_TEXT);
}
AFN Grammar::GenerateAFN()
{
    AFN afn;
    std::stack<int> state_stack;

    for (char c : postfix) {
        if (isalnum(c)) {
            int start = afn.CreateNewState();
            int end = afn.CreateNewState();
            afn.AddTransition(start, end, std::string(1, c));
            state_stack.push(start);
            state_stack.push(end);
        }
        else if (c == '|') {
            int b_end = state_stack.top(); state_stack.pop();
            int b_start = state_stack.top(); state_stack.pop();
            int a_end = state_stack.top(); state_stack.pop();
            int a_start = state_stack.top(); state_stack.pop();

            int new_start = afn.CreateNewState();
            int new_end = afn.CreateNewState();
            afn.AddTransition(new_start, a_start, "λ");
            afn.AddTransition(new_start, b_start, "λ");
            afn.AddTransition(a_end, new_end, "λ");
            afn.AddTransition(b_end, new_end, "λ");

            state_stack.push(new_start);
            state_stack.push(new_end);
        }
        else if (c == '.') {
            int b_end = state_stack.top(); state_stack.pop();
            int b_start = state_stack.top(); state_stack.pop();
            int a_end = state_stack.top(); state_stack.pop();
            int a_start = state_stack.top(); state_stack.pop();

            afn.AddTransition(a_end, b_start, "λ");
            state_stack.push(a_start);
            state_stack.push(b_end);
        }
        else if (c == '*') {
            int end = state_stack.top(); state_stack.pop();
            int start = state_stack.top(); state_stack.pop();

            int new_start = afn.CreateNewState();
            int new_end = afn.CreateNewState();
            afn.AddTransition(new_start, start, "λ");
            afn.AddTransition(end, start, "λ");
            afn.AddTransition(new_start, new_end, "λ");
            afn.AddTransition(end, new_end, "λ");

            state_stack.push(new_start);
            state_stack.push(new_end);
        }
    }

    int final_end = state_stack.top(); state_stack.pop();
    int final_start = state_stack.top(); state_stack.pop();

    afn.SetStartState(final_start);
    afn.AddFinalState(final_end);

    return afn;
}
