#ifndef VALUE_HPP
#define VALUE_HPP

#include <string>

using namespace std;

enum Type {
    num, val, arr, acc
};

class Value {
    int adr = 0;
    string name;
    Type type;
    int low = 0;
    int up = 0;
    bool arg = false;
    bool init = false;

public:
    Value(int adr, const string& name, Type type) 
        : adr(adr), name(name), type(type) {}

    Value(int adr, const string& name, Type type, bool arg) 
        : adr(adr), name(name), type(type), arg(arg) {}

    Value(int adr, const string& name, int low, int up) 
        : adr(adr), name(name), type(Type::arr), low(low), up(up) {}

    Value(int adr, const string& name, int low, int up, bool init) 
        : adr(adr), name(name), type(Type::arr), low(low), up(up), init(init) {}

    int getAddress() {
        return adr;
    }

    string getName() {
        return name;
    }

    Type getType() {
        return type;
    }

    int getSize() {
        return abs(up-low)+1;
    }

    int getLow() {
        return low;
    }
    
    void setAddress(int a) {
        adr = a;
    }

    void setInit() {
        init = true;
    }

    bool checkArg(){
        return arg;
    }

    bool checkInit() {
        return init;
    }
};

#endif  // VALUE_HPP
