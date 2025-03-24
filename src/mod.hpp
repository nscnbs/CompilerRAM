#ifndef MOD_HPP
#define MOD_HPP

#include <vector>
#include "types.hpp"

using namespace std;

inline vector<Instruction> getMod() {
    vector<Instruction> code;
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
    code.push_back(Instruction("JUMP", "4"));
    code.push_back(Instruction("SET", "0"));
    code.push_back(Instruction("SUB", "_y_"));
    code.push_back(Instruction("STORE", "_y_"));
    code.push_back(Instruction("LOAD", "_x_"));
    code.push_back(Instruction("SUB", "_y_"));
    code.push_back(Instruction("JNEG", "15"));
    code.push_back(Instruction("LOAD", "_y_"));
    code.push_back(Instruction("STORE", "_pwr_"));
    code.push_back(Instruction("LOAD", "_pwr_"));
    code.push_back(Instruction("ADD", "_pwr_"));
    code.push_back(Instruction("SUB", "_x_"));
    code.push_back(Instruction("JPOS", "5"));
    code.push_back(Instruction("LOAD", "_pwr_"));
    code.push_back(Instruction("ADD", "_pwr_"));
    code.push_back(Instruction("STORE", "_pwr_"));
    code.push_back(Instruction("JUMP", "-7"));
    code.push_back(Instruction("LOAD", "_x_"));
    code.push_back(Instruction("SUB", "_pwr_"));
    code.push_back(Instruction("STORE", "_x_"));
    code.push_back(Instruction("JUMP", "-16"));
    code.push_back(Instruction("LOAD", "_sign_"));
    code.push_back(Instruction("JNEG", "2"));
    code.push_back(Instruction("JUMP", "4"));
    code.push_back(Instruction("SET", "0"));
    code.push_back(Instruction("SUB", "_x_"));
    code.push_back(Instruction("STORE", "_x_"));
    code.push_back(Instruction("LOAD", "_x_"));
    return code;
}

#endif // MOD_HPP
