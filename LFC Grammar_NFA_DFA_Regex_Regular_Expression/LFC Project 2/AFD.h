#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include "SetHashUtils.h"
#include <io.h> 
#include <fcntl.h>
#include <iomanip> 

class AFD {
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
    int start_state;
    std::set<int> final_states;

public:
    AFD() = default;

    int CreateNewState();

    void AddTransition(int from, int to, const std::string& symbol);

    void SetStartState(int state);

    void AddFinalState(int state);
    void PrintAFDTable() const;
    void PrintAFD() const;
    bool CheckWord(const std::string& word) const;
    void SaveToFile(const std::string& filename) const;

    void VerifyAutomaton() const;

};