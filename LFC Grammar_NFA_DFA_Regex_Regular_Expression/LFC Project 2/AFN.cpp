#include "AFN.h"


void AFN::PrintAFN() const
{
    _setmode(_fileno(stdout), _O_U16TEXT);

    std::wcout << L"AFN Details:\n";
    std::wcout << L"States: ";
    for (int state : states) std::wcout << state << L" ";
    std::wcout << L"\nAlphabet: ";
    for (const auto& symbol : alphabet) {
        if (symbol == "λ") {
            std::wcout << L"λ "; 
        }
        else {
            std::wcout << symbol.c_str() << L" ";
        }
    }
    std::wcout << L"\nStart State: " << start_state << L"\nFinal States: ";
    for (int state : final_states) std::wcout << state << L" ";
    std::wcout << L"\nTransitions:\n";
    for (const auto& t : transitions) {
        std::wcout << t.from << L" --";
        if (t.symbol == "λ") {
            std::wcout << L"λ"; 
        }
        else {
            std::wcout << t.symbol.c_str();
        }
        std::wcout << L"--> " << t.to << L"\n";
    }

    _setmode(_fileno(stdout), _O_TEXT);
}

int AFN::CreateNewState()
{
    int new_state = state_count++; 
    states.insert(new_state);     
    return new_state;            
}

void AFN::AddTransition(int from, int to, const std::string& symbol)
{
    transitions.push_back({ from, to, symbol });
    states.insert(from);
    states.insert(to);
    if (symbol != "λ") {
        alphabet.insert(symbol); 
    }
}

void AFN::SetStartState(int state)
{
    start_state = state;
}

void AFN::AddFinalState(int state)
{
    final_states.insert(state);
}

AFD AFN::ToDeterministic()
{
    AFD deterministic;
    std::queue<std::set<int>> state_queue;
    std::unordered_map<std::set<int>, int, SetHash, SetEqual> state_map;
    int next_state_id = 0;

    std::set<int> start_closure = lambda_closure({ start_state });
    state_queue.push(start_closure);
    state_map[start_closure] = deterministic.CreateNewState();
    deterministic.SetStartState(state_map[start_closure]);

    while (!state_queue.empty()) {
        auto current_set = state_queue.front();
        state_queue.pop();

        for (const auto& symbol : alphabet) {
            std::set<int> next_set = move(current_set, symbol);
            next_set = lambda_closure(next_set);

            if (next_set.empty()) continue;

            if (state_map.find(next_set) == state_map.end()) {
                state_map[next_set] = deterministic.CreateNewState();
                state_queue.push(next_set);
            }

            deterministic.AddTransition(state_map[current_set], state_map[next_set], symbol);
        }

        for (int state : current_set) {
            if (final_states.find(state) != final_states.end()) {
                deterministic.AddFinalState(state_map[current_set]);
                break;
            }
        }
    }
    return deterministic;
}

std::set<int> AFN::lambda_closure(const std::set<int>& states) const
{
    std::set<int> closure = states;
    std::queue<int> queue;

    for (int state : states) {
        queue.push(state);
    }

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        for (const auto& transition : transitions) {
            if (transition.from == current && transition.symbol == "λ") {
                if (closure.find(transition.to) == closure.end()) {
                    closure.insert(transition.to);
                    queue.push(transition.to);
                }
            }
        }
    }
    return closure;
}

std::set<int> AFN::move(const std::set<int>& states, const std::string& symbol) const
{
    std::set<int> result;

    for (int state : states) {
        for (const auto& transition : transitions) {
            if (transition.from == state && transition.symbol == symbol) {
                result.insert(transition.to);
            }
        }
    }
    return result;
}
