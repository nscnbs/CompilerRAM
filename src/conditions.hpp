#ifndef CONDITIONS_HPP
#define CONDITIONS_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "types.hpp"

using namespace std;


enum SignCode {SIG_EQ, SIG_NE, SIG_GT, SIG_LT, SIG_GE, SIG_LE};

class Conditions{
public:
    Types types;
    int extractSignCode(const vector<Instruction>& cond);
    vector<Instruction> equal(const Types &x, const Types &y);
    vector<Instruction> nequal(const Types &x, const Types &y);
    vector<Instruction> grater(const Types &x, const Types &y);
    vector<Instruction> lower(const Types &x, const Types &y);
    vector<Instruction> graterEq(const Types &x, const Types &y);
    vector<Instruction> lowerEq(const Types &x, const Types &y);
};

#endif // CONDITIONS_HPP
