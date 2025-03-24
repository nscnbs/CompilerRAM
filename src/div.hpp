#ifndef DIV_HPP
#define DIV_HPP

#include <vector>
#include "types.hpp"

using namespace std;


inline vector<Instruction> getDiv() {
    vector<Instruction> code;
    code.push_back(Instruction("SET", "0"));
    code.push_back(Instruction("STORE", "_rslt_"));
    code.push_back(Instruction("SET", "1"));
    code.push_back(Instruction("STORE", "_sign_"));
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
    code.push_back(Instruction("LOAD", "_x_"));
    code.push_back(Instruction("SUB", "_y_"));
    code.push_back(Instruction("JNEG", "23"));
    code.push_back(Instruction("LOAD", "_y_"));
    code.push_back(Instruction("STORE", "_pwr_"));
    code.push_back(Instruction("SET", "1"));
    code.push_back(Instruction("STORE", "_cnt_"));
    code.push_back(Instruction("LOAD", "_pwr_"));
    code.push_back(Instruction("ADD", "_pwr_"));
    code.push_back(Instruction("SUB", "_x_"));
    code.push_back(Instruction("JPOS", "8"));
    code.push_back(Instruction("LOAD", "_pwr_"));
    code.push_back(Instruction("ADD", "_pwr_"));
    code.push_back(Instruction("STORE", "_pwr_"));
    code.push_back(Instruction("LOAD", "_cnt_"));
    code.push_back(Instruction("ADD", "_cnt_"));
    code.push_back(Instruction("STORE", "_cnt_"));
    code.push_back(Instruction("JUMP", "-10"));
    code.push_back(Instruction("LOAD", "_x_"));
    code.push_back(Instruction("SUB", "_pwr_"));
    code.push_back(Instruction("STORE", "_x_"));
    code.push_back(Instruction("LOAD", "_rslt_"));
    code.push_back(Instruction("ADD", "_cnt_"));
    code.push_back(Instruction("STORE", "_rslt_"));
    code.push_back(Instruction("JUMP", "-24"));
    code.push_back(Instruction("LOAD", "_sign_"));
    code.push_back(Instruction("JNEG", "2"));
    code.push_back(Instruction("JUMP", "4"));
    code.push_back(Instruction("SET", "0"));
    code.push_back(Instruction("SUB", "_rslt_"));
    code.push_back(Instruction("STORE", "_rslt_"));
    code.push_back(Instruction("LOAD", "_rslt_"));
    return code;
}

#endif // DIV_HPP
