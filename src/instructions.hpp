#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include "conditions.hpp"
#include "expressions.hpp"
#include "manager.hpp"
#include "types.hpp"

using namespace std;

extern Manager manager;

class GenerateInstruction{
private:
    Expressions expression;
    Conditions condition;
    int codeLine = 0;
    vector<Instruction> mergedCode;
    vector<Instruction> procCode;
public:
    vector<Instruction> connectRead(const Types &ident);
    vector<Instruction> connectWrite(const Types &value);
    vector<Instruction> connectAssign(const Types &ident, const vector<Instruction> &exprs);
    vector<Instruction> connectIf(const vector<Instruction> &cond, const vector<Instruction> &then);
    vector<Instruction> connectIfElse(const vector<Instruction> &cond,
                                        const vector<Instruction> &then,
                                        const vector<Instruction> &elsee);
    vector<Instruction> connectWhile(const vector<Instruction> &cond, 
                                        const vector<Instruction> &com);
    vector<Instruction> connectRepeat(const vector<Instruction> &com, const vector<Instruction> &cond);
    vector<Instruction> connectFor(const string &iter, const Types &low, const Types &up, const vector<Instruction> &com);
    vector<Instruction> connectForDown(const string &iter, const Types &low, const Types &up, const vector<Instruction> &com);
    vector<Instruction> connectCall(const string &nameProc);
    vector<Instruction> connectEq(const Types &x, const Types &y);
    vector<Instruction> connectNeq(const Types &x, const Types &y);
    vector<Instruction> connectGt(const Types &x, const Types &y);
    vector<Instruction> connectLt(const Types &x, const Types &y);
    vector<Instruction> connectGte(const Types &x, const Types &y);
    vector<Instruction> connectLte(const Types &x, const Types &y);
    vector<Instruction> connectVal(const Types &value);
    vector<Instruction> connectPlus(const Types &x, const Types &y);
    vector<Instruction> connectMinus(const Types &x, const Types &y);
    vector<Instruction> connectMult(const Types &x, const Types &y);
    vector<Instruction> connectDiv(const Types &x, const Types &y);
    vector<Instruction> connectMod(const Types &x, const Types &y);
    vector<Instruction> connectProc(const string &nameProc, const vector<Instruction> &com);
    vector<Instruction> connectMain(const vector<Instruction> &com);
    vector<Instruction> replaceNamesAdrs(const string &nameProc, vector<Instruction> &code);
    vector<Instruction> replaceCall(vector<Instruction>& code);
    vector<Instruction> declarateArr();
    void generateCode(const vector<Instruction> &proc, const vector<Instruction> &main);
    const vector<Instruction>& getCode() const;
};

#endif // INSTRUCTIONS_HPP
