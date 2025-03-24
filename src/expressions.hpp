#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "types.hpp"
#include "mult.hpp"
#include "div.hpp"
#include "mod.hpp"

using namespace std;

class Expressions{
public:
    vector<Instruction> valAssign(const Types &value);
    vector<Instruction> plus(const Types &x, const Types &y);
    vector<Instruction> minus(const Types &x, const Types &y);
    vector<Instruction> mult(const Types &x, const Types &y);
    vector<Instruction> div(const Types &x, const Types &y);
    vector<Instruction> mod(const Types &x, const Types &y);
};

#endif // EXPRESSIONS_HPP
