#include "AFD.h"

int AFD::CreateNewState()
{
    int new_state = state_count++;
    states.insert(new_state);
    return new_state;
}

void AFD::AddTransition(int from, int to, const std::string& symbol)
{
    transitions.push_back({ from, to, symbol });
    states.insert(from);
    states.insert(to);
    alphabet.insert(symbol); 
}

void AFD::SetStartState(int state)
{
    start_state = state;
}

void AFD::AddFinalState(int state)
{
    final_states.insert(state);
}

void AFD::PrintAFD() const
{
    std::cout << "AFD Details:\n";
    std::cout << "States: ";
    for (int state : states) std::cout << state << " ";
    std::cout << "\nAlphabet: ";
    for (const auto& symbol : alphabet) std::cout << symbol << " ";
    std::cout << "\nStart State: " << start_state << "\nFinal States: ";
    for (int state : final_states) std::cout << state << " ";
    std::cout << "\nTransitions:\n";
    for (const auto& t : transitions) {
        std::cout << t.from << " --" << t.symbol << "--> " << t.to << "\n";
    }
}

bool AFD::CheckWord(const std::string& word) const
{
    int current_state = start_state;

    for (char symbol : word) {
        bool transition_found = false;
        for (const auto& t : transitions) {
            if (t.from == current_state && t.symbol == std::string(1, symbol)) {
                current_state = t.to;
                transition_found = true;
                break;
            }
        }
        if (!transition_found) {
            return false; 
        }
    }

    return final_states.find(current_state) != final_states.end();
}

void AFD::SaveToFile(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Eroare: Nu s-a putut deschide fișierul " << filename << " pentru scriere.\n";
        return;
    }

    file << "AFD Details:\n";

    file << "States: ";
    for (int state : states) {
        file << state << " ";
    }
    file << "\n";

    file << "Alphabet: ";
    for (const auto& symbol : alphabet) {
        file << symbol << " ";
    }
    file << "\n";

    file << "Start State: " << start_state << "\n";

    file << "Final States: ";
    for (int state : final_states) {
        file << state << " ";
    }
    file << "\n";

    file << "Transitions:\n";
    for (const auto& t : transitions) {
        file << t.from << " --" << t.symbol << "--> " << t.to << "\n";
    }

    file.close(); 
    std::cout << "AFD-ul a fost salvat in fisierul: " << filename << "\n";
}

void AFD::PrintAFDTable() const {
    const int cell_width = 5; 

    auto center_text = [](const std::wstring& text, int width) {
        int padding = width - static_cast<int>(text.size());
        int pad_left = padding / 2;
        int pad_right = padding - pad_left;
        return std::wstring(pad_left, L' ') + text + std::wstring(pad_right, L' ');
        };

    auto format_state = [&](int state) -> std::wstring {
        if (state == start_state) {
            return L"S" + std::to_wstring(state);
        }
        else if (final_states.find(state) != final_states.end()) {
            return L"T" + std::to_wstring(state); 
        }
        else {
            return L"q" + std::to_wstring(state); 
        }
        };

    _setmode(_fileno(stdout), _O_U16TEXT);

    std::wcout << L"╔" << std::wstring(cell_width, L'═');
    for (size_t i = 0; i < alphabet.size(); ++i) {
        std::wcout << L"╦" << std::wstring(cell_width, L'═');
    }
    std::wcout << L"╗\n";

    std::wcout << L"║" << center_text(L"δ'", cell_width);
    for (const auto& symbol : alphabet) {
        std::wstring w_symbol(symbol.begin(), symbol.end()); 
        std::wcout << L"║" << center_text(w_symbol, cell_width);
    }
    std::wcout << L"║\n";

    std::wcout << L"╠" << std::wstring(cell_width, L'═');
    for (size_t i = 0; i < alphabet.size(); ++i) {
        std::wcout << L"╬" << std::wstring(cell_width, L'═');
    }
    std::wcout << L"╣\n";

    for (int state : states) {
        std::wstring state_name = format_state(state); 
        std::wcout << L"║" << center_text(state_name, cell_width);
        for (const auto& symbol : alphabet) {
            bool found = false;
            for (const auto& t : transitions) {
                if (t.from == state && t.symbol == symbol) {
                    std::wstring target_state = format_state(t.to); 
                    std::wcout << L"║" << center_text(target_state, cell_width);
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::wcout << L"║" << center_text(L"-", cell_width);
            }
        }
        std::wcout << L"║\n";
    }

    std::wcout << L"╚" << std::wstring(cell_width, L'═');
    for (size_t i = 0; i < alphabet.size(); ++i) {
        std::wcout << L"╩" << std::wstring(cell_width, L'═');
    }
    std::wcout << L"╝\n";

    _setmode(_fileno(stdout), _O_TEXT);
}

void AFD::VerifyAutomaton() const {
    if (states.find(start_state) == states.end()) {
        throw std::runtime_error("Automatul nu are o stare initiala valida.");
    }

    if (final_states.empty()) {
        throw std::runtime_error("Automatul nu are stari finale definite.");
    }

    for (int state : final_states) {
        if (states.find(state) == states.end()) {
            throw std::runtime_error("Stare finala invalida detectata: q" + std::to_string(state));
        }
    }

    for (const auto& t : transitions) {
        if (states.find(t.from) == states.end() || states.find(t.to) == states.end()) {
            throw std::runtime_error("Tranzitie invalida: foloseste stari care nu sunt definite.");
        }
    }

    for (const auto& t : transitions) {
        if (alphabet.find(t.symbol) == alphabet.end()) {
            throw std::runtime_error("Simbol necunoscut in tranzitie: " + t.symbol);
        }
    }

    std::map<std::pair<int, std::string>, int> seen_transitions;
    for (const auto& t : transitions) {
        auto key = std::make_pair(t.from, t.symbol);
        if (seen_transitions.find(key) != seen_transitions.end()) {
            throw std::runtime_error("AFD invalid: Doua tranzitii pentru acelasi simbol detectate.");
        }
        seen_transitions[key] = t.to;
    }

    std::set<int> reachable_states;
    std::queue<int> queue;
    queue.push(start_state);
    reachable_states.insert(start_state);

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        for (const auto& t : transitions) {
            if (t.from == current && reachable_states.find(t.to) == reachable_states.end()) {
                reachable_states.insert(t.to);
                queue.push(t.to);
            }
        }
    }

    for (int state : states) {
        if (reachable_states.find(state) == reachable_states.end()) {
            throw std::runtime_error("Automatul contine stari inaccesibile: q" + std::to_string(state));
        }
    }

    std::cout << "Automatul este valid.\n";
}
