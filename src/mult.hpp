#ifndef MULT_HPP
#define MULT_HPP

#include <vector>
#include "types.hpp"

using namespace std;


inline vector<Instruction> getMult() {
    vector<Instruction> code;
    code.push_back(Instruction("SET", "0"));
    code.push_back(Instruction("STORE", "_rslt_"));
    code.push_back(Instruction("SET", "1"));
    code.push_back(Instruction("STORE", "_sign_"));
    code.push_back(Instruction("STORE", "_pwr_"));
    code.push_back(Instruction("LOAD", "_x_"));
    code.push_back(Instruction("JNEG", "2"));
    code.push_back(Instruction("JUMP", "7"));
    code.push_back(Instruction("SET", "0"));
    code.push_back(Instruction("SUB", "_x_"));
    code.push_back(Instruction("STORE", "_x_"));
    code.push_back(Instruction("SET", "0"));
    code.push_back(Instruction("SUB", "_sign_"));
    code.push_back(Instruction("STORE", "_sign_"));
    code.push_back(Instruction("LOAD", "_y_"));
    code.push_back(Instruction("JNEG", "2"));
    code.push_back(Instruction("JUMP", "7"));
    code.push_back(Instruction("SET", "0"));
    code.push_back(Instruction("SUB", "_y_"));
    code.push_back(Instruction("STORE", "_y_"));
    code.push_back(Instruction("SET", "0"));
    code.push_back(Instruction("SUB", "_sign_"));
    code.push_back(Instruction("STORE", "_sign_"));
    code.push_back(Instruction("LOAD", "_pwr_"));
    code.push_back(Instruction("SUB", "_y_"));
    code.push_back(Instruction("JPOS", "8"));
    code.push_back(Instruction("LOAD", "_pwr_"));
    code.push_back(Instruction("ADD", "_pwr_"));
    code.push_back(Instruction("STORE", "_pwr_"));
    code.push_back(Instruction("LOAD", "_x_"));
    code.push_back(Instruction("ADD", "_x_"));
    code.push_back(Instruction("STORE", "_x_"));
    code.push_back(Instruction("JUMP", "-9"));
    code.push_back(Instruction("LOAD", "_y_"));
    code.push_back(Instruction("JNEG", "18"));
    code.push_back(Instruction("JZERO", "17"));
    code.push_back(Instruction("LOAD", "_y_"));
    code.push_back(Instruction("SUB", "_pwr_"));
    code.push_back(Instruction("JNEG", "7"));
    code.push_back(Instruction("LOAD", "_y_"));
    code.push_back(Instruction("SUB", "_pwr_"));
    code.push_back(Instruction("STORE", "_y_"));
    code.push_back(Instruction("LOAD", "_rslt_"));
    code.push_back(Instruction("ADD", "_x_"));
    code.push_back(Instruction("STORE", "_rslt_"));
    code.push_back(Instruction("LOAD", "_pwr_"));
    code.push_back(Instruction("HALF", ""));
    code.push_back(Instruction("STORE", "_pwr_"));
    code.push_back(Instruction("LOAD", "_x_"));
    code.push_back(Instruction("HALF", ""));
    code.push_back(Instruction("STORE", "_x_"));
    code.push_back(Instruction("JUMP", "-18"));
    code.push_back(Instruction("LOAD", "_sign_"));
    code.push_back(Instruction("JNEG", "2"));
    code.push_back(Instruction("JUMP", "4"));
    code.push_back(Instruction("SET", "0"));
    code.push_back(Instruction("SUB", "_rslt_"));
    code.push_back(Instruction("STORE", "_rslt_"));
    code.push_back(Instruction("LOAD", "_rslt_"));
    return code;
}

#endif // MULT_HPP
