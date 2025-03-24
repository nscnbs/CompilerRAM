#ifndef PROCEDURE_HPP
#define PROCEDURE_HPP

#include <iostream>
#include <string>
#include <vector>
#include "value.hpp"

using namespace std;

class Procedure {
    int adr;
    string nameProc;
    vector<Value> var;
    vector<Value> arg;
    int rtrnLine;
public:
    Procedure() : adr(0), nameProc("") {}

    Procedure(int adr, const string& nameProc)
        : adr(adr), nameProc(nameProc) {}

    int getAddress() {
        return adr;
    }

    string getName(){
        return nameProc;
    }

    vector<Value>& getVar() {
        return var;
    }

    vector<Value>& getArg() {
        return arg;
    }

    void setReturn(int line) {
        rtrnLine = line;
    }

    int getReturn() {
        return rtrnLine;
    }

    void addVar(Value& v) {
        var.push_back(v);
    }

    void addArg(Value& v) {
        arg.push_back(v);
    }
};

#endif // PROCEDURE_HPP
