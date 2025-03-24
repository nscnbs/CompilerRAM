#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include "value.hpp"

using namespace std;

struct Types {
    string name;    
    Type type;
    string index;
    Type indexType;

    Types() 
      : name(""), type(val), index(""), indexType(val) {}

    Types(const string &b, Type t) 
      : name(b), type(t), index(""), indexType(val) {}

    Types(const string &b, Type t, const string &idx, Type idxT)
      : name(b), type(t), index(idx), indexType(idxT) {}
};

class Instruction {
public:
  string op;
  string arg;

  Instruction(const string &op, const string &arg)
      : op(op), arg(arg) {}

};

#endif // TYPES_HPP
