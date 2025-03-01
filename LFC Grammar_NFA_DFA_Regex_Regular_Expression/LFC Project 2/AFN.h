#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <queue>
#include <string>
#include "SetHashUtils.h"
#include "AFD.h"
#include <io.h>
#include <fcntl.h>

class AFN {
private:
    struct Transition {
        int from;
        int to;
        std::string symbol; 
    };

    int state_count = 0;
    std::set<int> states;
    std::set<std::string> alphabet;
    std::vector<Transition> transitions;
    int start_state = 0;
    std::set<int> final_states;

public:
    AFN() = default;

    void PrintAFN() const;

    int CreateNewState();

    void AddTransition(int from, int to, const std::string& symbol);


    void SetStartState(int state);

    void AddFinalState(int state);

    AFD ToDeterministic();

    std::set<int> lambda_closure(const std::set<int>& states) const;
    std::set<int> move(const std::set<int>& states, const std::string& symbol) const;
};
