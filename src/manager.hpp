#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "procedure.hpp"
#include "value.hpp"
#include "types.hpp"

using namespace std;

extern void yyerror(const char* s);

class Manager
{
private:
    int memoryAdr = 1;
    string currProc;
    vector<Value> buff;
    
public:
    int rtrnLine = 1;
    unordered_map<string, Procedure> declProc;
    vector<string> buffCallArgs;
    void initProc(const string& nameProc);
    void addProc(const string& nameProc);
    void bufferVar(const string& name);
    void bufferArr(const string& name, const string& low, const string& up);
    void bufferArg(const string& name, Type type);
    void bufferProcCall(const string& name);
    int getAdr(const string &name, const string &nameProc);
    int initIter(const string& iter);
    int addAcc(const string& name);
    bool findArg(const string &name, const string &nameProc);
    void initVar(const Types &ident);
    void checkVarUsage(const string &name, bool isArrayUsage);
    void checkProcCall(const string &procName);
    void checkInit();
};

#endif // MANAGER_HPP
