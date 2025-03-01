#pragma once
#ifndef SETHASHUTILS_H
#define SETHASHUTILS_H

#include <set>
#include <functional>

struct SetHash {
    size_t operator()(const std::set<int>& s) const {
        size_t hash = 0;
        for (int elem : s) {
            hash ^= std::hash<int>{}(elem)+0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

struct SetEqual {
    bool operator()(const std::set<int>& a, const std::set<int>& b) const {
        return a == b; 
    }
};

#endif // SETHASHUTILS_H
