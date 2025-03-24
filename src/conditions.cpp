#include "conditions.hpp"

int Conditions::extractSignCode(const vector<Instruction>& cond) {
    if (cond.empty()) {
        cout << "[Warning] empty condition vector" << endl; 
    }
    string sign = cond.back().arg;
    if (sign == "=")
        return SIG_EQ;
    else if (sign == "!=")
        return SIG_NE;
    else if (sign == ">")
        return SIG_GT;
    else if (sign == "<")
        return SIG_LT;
    else if (sign == ">=")
        return SIG_GE;
    else if (sign == "<=")
        return SIG_LE;
    else{
        cout << "[Warning] operand is not returned" << endl; 
        return -1;
    }
}

vector<Instruction> Conditions::equal(const Types &x, const Types &y){
    vector<Instruction> code;
    switch (x.type) {
        case num:
            switch (y.type) {
                case val:
                    if(x.name == "0"){
                        code.push_back(Instruction("LOAD", y.name));
                        code.push_back(Instruction("JZERO", "="));                        
                    }
                    else{
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("SUB", y.name));
                        code.push_back(Instruction("JZERO", "="));
                    }
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                    }
                    if(x.name == "0"){
                        code.pop_back();
                        code.push_back(Instruction("LOADI", "0"));
                        code.push_back(Instruction("JZERO", "="));                        
                        }
                    else{
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("SUBI", "_pointer_"));
                        code.push_back(Instruction("JZERO", "="));
                        }
                    break;
            }
            break;
        case val:
            switch (y.type) {
                case num:
                    if(y.name == "0"){
                        code.push_back(Instruction("LOAD", x.name));
                        code.push_back(Instruction("JZERO", "="));                        
                    }
                    else{
                        code.push_back(Instruction("SET", y.name));
                        code.push_back(Instruction("SUB", x.name));
                        code.push_back(Instruction("JZERO", "="));
                    }
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("SUB", y.name));
                    code.push_back(Instruction("JZERO", "="));
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                    }
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("SUBI", "_pointer_"));
                    code.push_back(Instruction("JZERO", "="));
                    break;
            }
            break;
        case arr:
            switch (x.indexType) {
                case num:
                    code.push_back(Instruction("SET", x.index));
                    code.push_back(Instruction("ADD", x.name));
                    code.push_back(Instruction("STORE", "_pointer_"));
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("ADD", x.index));
                    code.push_back(Instruction("STORE", "_pointer_"));
                    break;
            }
            switch (y.type) {
                case num:
                    if(y.name == "0"){
                        code.pop_back();
                        code.push_back(Instruction("LOADI", "0"));
                        code.push_back(Instruction("JZERO", "="));                        
                    }
                    else{
                        code.push_back(Instruction("SET", y.name));
                        code.push_back(Instruction("SUBI", "_pointer_"));
                        code.push_back(Instruction("JZERO", "="));
                    }
                    break;
                case val:
                    code.pop_back();
                    code.push_back(Instruction("LOADI", "0"));
                    code.push_back(Instruction("SUB", y.name));
                    code.push_back(Instruction("JZERO", "="));
                    break;
                case arr:
                  
                    break;
            }
            break;
    }
    return code;
}
vector<Instruction> Conditions::nequal(const Types &x, const Types &y){
    vector<Instruction> code;
    switch (x.type) {
        case num:
            switch (y.type) {
                case val:
                    if(x.name == "0"){
                        code.push_back(Instruction("LOAD", y.name));
                        code.push_back(Instruction("JZERO", "!="));                        
                    }
                    else{
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("SUB", y.name));
                        code.push_back(Instruction("JZERO", "!="));
                    }
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                    }
                    if(x.name == "0"){
                        code.pop_back();
                        code.push_back(Instruction("LOADI", "0"));
                        code.push_back(Instruction("JZERO", "!="));                        
                        }
                    else{
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("SUBI", "_pointer_"));
                        code.push_back(Instruction("JZERO", "!="));
                        }
                    break;
            }
            break;
        case val:
            switch (y.type) {
                case num:
                    if(y.name == "0"){
                        code.push_back(Instruction("LOAD", x.name));
                        code.push_back(Instruction("JZERO", "!="));                        
                    }
                    else{
                        code.push_back(Instruction("SET", y.name));
                        code.push_back(Instruction("SUB", x.name));
                        code.push_back(Instruction("JZERO", "!="));
                    }
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("SUB", y.name));
                    code.push_back(Instruction("JZERO", "!="));
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                    }
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("SUBI", "_pointer_"));
                    code.push_back(Instruction("JZERO", "!="));
                    break;
            }
            break;
        case arr:
            switch (x.indexType) {
                case num:
                    code.push_back(Instruction("SET", x.index));
                    code.push_back(Instruction("ADD", x.name));
                    code.push_back(Instruction("STORE", "_pointer_"));
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("ADD", x.index));
                    code.push_back(Instruction("STORE", "_pointer_"));
                    break;
            }
            switch (y.type) {
                case num:
                    if(y.name == "0"){
                        code.pop_back();
                        code.push_back(Instruction("LOADI", "0"));
                        code.push_back(Instruction("JZERO", "!="));                        
                    }
                    else{
                        code.push_back(Instruction("SET", y.name));
                        code.push_back(Instruction("SUBI", "_pointer_"));
                        code.push_back(Instruction("JZERO", "!="));
                    }
                    break;
                case val:
                    code.pop_back();
                    code.push_back(Instruction("LOADI", "0"));
                    code.push_back(Instruction("SUB", y.name));
                    code.push_back(Instruction("JZERO", "!="));
                    break;
                case arr:
                    
                    break;
            }
            break;
    }
    return code;
}
vector<Instruction> Conditions::grater(const Types &x, const Types &y){
    vector<Instruction> code;
    switch (x.type) {
        case num:
            switch (y.type) {
                case val:
                    if(x.name == "0"){
                        code.push_back(Instruction("LOAD", y.name));
                        code.push_back(Instruction("JPOS", ">"));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;                    
                    }
                    else{
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("SUB", y.name));
                        code.push_back(Instruction("JNEG", ">"));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;   
                    }
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                    }
                    if(x.name == "0"){
                        code.pop_back();
                        code.push_back(Instruction("LOADI", "0"));
                        code.push_back(Instruction("JPOS", ">"));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;                         
                    }
                    else{
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("SUBI", "_pointer_"));
                        code.push_back(Instruction("JNEG", ">"));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;   
                    }
                    break;
            }
            break;
        case val:
            switch (y.type) {
                case num:
                    if(y.name == "0"){
                        code.push_back(Instruction("LOAD", x.name));
                        code.push_back(Instruction("JPOS", ">"));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;                        
                    }
                    else{
                        code.push_back(Instruction("SET", y.name));
                        code.push_back(Instruction("SUB", x.name));
                        code.push_back(Instruction("JNEG", ">"));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;   
                    }
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("SUB", y.name));
                    code.push_back(Instruction("JPOS", ">"));
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                    }
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("SUBI", "_pointer_"));
                    code.push_back(Instruction("JPOS", ">"));
                    break;
            }
            break;
        case arr:
            switch (x.indexType) {
                case num:
                    code.push_back(Instruction("SET", x.index));
                    code.push_back(Instruction("ADD", x.name));
                    code.push_back(Instruction("STORE", "_pointer_"));
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("ADD", x.index));
                    code.push_back(Instruction("STORE", "_pointer_"));
                    break;
            }
            switch (y.type) {
                case num:
                    if(y.name == "0"){
                        code.pop_back();
                        code.push_back(Instruction("LOADI", "0"));
                        code.push_back(Instruction("JPOS", ">"));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;                         
                    }
                    else{
                        code.push_back(Instruction("SET", y.name));
                        code.push_back(Instruction("SUBI", "_pointer_"));
                        code.push_back(Instruction("JNEG", ">"));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;   
                    }
                    break;
                case val:
                    code.pop_back();
                    code.push_back(Instruction("LOADI", "0"));
                    code.push_back(Instruction("SUB", y.name));
                    code.push_back(Instruction("JNEG", ">"));
                    break;
                case arr:
                    
                    break;
            }
            break;
    }
    return code;
}
vector<Instruction> Conditions::lower(const Types &x, const Types &y){
    vector<Instruction> code;
    switch (x.type) {
        case num:
            switch (y.type) {
                case val:
                    if(x.name == "0"){
                        code.push_back(Instruction("LOAD", y.name));
                        code.push_back(Instruction("JPOS", "<"));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;                           
                    }
                    else{
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("SUB", y.name));
                        code.push_back(Instruction("JNEG", "<"));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;   
                    }
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                    }
                    if(x.name == "0"){
                        code.pop_back();
                        code.push_back(Instruction("LOADI", "0"));
                        code.push_back(Instruction("JPOS", "<")); 
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;                        
                    }
                    else{
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("SUBI", "_pointer_"));
                        code.push_back(Instruction("JNEG", "<"));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;   
                    }
                    break;
            }
            break;
        case val:
            switch (y.type) {
                case num:
                    if(y.name == "0"){
                        code.push_back(Instruction("LOAD", x.name));
                        code.push_back(Instruction("JNEG", "<")); 
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;                     
                    }
                    else{
                        code.push_back(Instruction("SET", y.name));
                        code.push_back(Instruction("SUB", x.name));
                        code.push_back(Instruction("JPOS", "<"));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;   
                    }
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("SUB", y.name));
                    code.push_back(Instruction("JNEG", "<"));
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                    }
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("SUBI", "_pointer_"));
                    code.push_back(Instruction("JNEG", "<"));
                    break;
            }
            break;
        case arr:
            switch (x.indexType) {
                case num:
                    code.push_back(Instruction("SET", x.index));
                    code.push_back(Instruction("ADD", x.name));
                    code.push_back(Instruction("STORE", "_pointer_"));
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("ADD", x.index));
                    code.push_back(Instruction("STORE", "_pointer_"));
                    break;
            }
            switch (y.type) {
                case num:
                    if(y.name == "0"){
                        code.pop_back();
                        code.push_back(Instruction("LOADI", "0"));
                        code.push_back(Instruction("JNEG", "<"));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;                           
                    }
                    else{
                        code.push_back(Instruction("SET", y.name));
                        code.push_back(Instruction("SUBI", "_pointer_"));
                        code.push_back(Instruction("JPOS", "<"));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;   
                    }
                    break;
                case val:
                    code.pop_back();
                    code.push_back(Instruction("LOADI", "0"));
                    code.push_back(Instruction("SUB", y.name));
                    code.push_back(Instruction("JPOS", "<"));
                    break;
                case arr:
                    
                    break;
            }
            break;
    }
    return code;
}
vector<Instruction> Conditions::graterEq(const Types &x, const Types &y){
    vector<Instruction> code;
    switch (x.type) {
        case num:
            switch (y.type) {
                case val:
                    if(x.name == "0"){
                        code.push_back(Instruction("LOAD", y.name));
                        code.push_back(Instruction("JPOS", ">="));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;                        
                    }
                    else{
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("SUB", y.name));
                        code.push_back(Instruction("JNEG", ">="));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;   
                    }
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                    }
                    if(x.name == "0"){
                        code.pop_back();
                        code.push_back(Instruction("LOADI", "0"));
                        code.push_back(Instruction("JPOS", ">="));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;                          
                    }
                    else{
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("SUBI", "_pointer_"));
                        code.push_back(Instruction("JNEG", ">="));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;   
                    }
                    break;
            }
            break;
        case val:
            switch (y.type) {
                case num:
                    if(y.name == "0"){
                        code.push_back(Instruction("LOAD", x.name));
                        code.push_back(Instruction("JNEG", ">="));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;                           
                    }
                    else{
                        code.push_back(Instruction("SET", y.name));
                        code.push_back(Instruction("SUB", x.name));
                        code.push_back(Instruction("JPOS", ">="));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;   
                    }
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("SUB", y.name));
                    code.push_back(Instruction("JNEG", ">="));
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                    }
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("SUBI", "_pointer_"));
                    code.push_back(Instruction("JNEG", ">="));
                    break;
            }
            break;
        case arr:
            switch (x.indexType) {
                case num:
                    code.push_back(Instruction("SET", x.index));
                    code.push_back(Instruction("ADD", x.name));
                    code.push_back(Instruction("STORE", "_pointer_"));
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("ADD", x.index));
                    code.push_back(Instruction("STORE", "_pointer_"));
                    break;
            }
            switch (y.type) {
                case num:
                    if(y.name == "0"){
                        code.pop_back();
                        code.push_back(Instruction("LOADI", "0"));
                        code.push_back(Instruction("JNEG", ">="));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;                         
                    }
                    else{
                        code.push_back(Instruction("SET", y.name));
                        code.push_back(Instruction("SUBI", "_pointer_"));
                        code.push_back(Instruction("JPOS", ">="));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;   
                    }
                    break;
                case val:
                    code.pop_back();
                    code.push_back(Instruction("LOADI", "0"));
                    code.push_back(Instruction("SUB", y.name));
                    code.push_back(Instruction("JNEG", ">="));
                    break;
                case arr:
                    
                    break;
            }
            break;
    }
    return code;
}
vector<Instruction> Conditions::lowerEq(const Types &x, const Types &y){
    vector<Instruction> code;
    switch (x.type) {
        case num:
            switch (y.type) {
                case val:
                    if(x.name == "0"){
                        code.push_back(Instruction("LOAD", y.name));
                        code.push_back(Instruction("JNEG", "<="));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;                          
                    }
                    else{
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("SUB", y.name));
                        code.push_back(Instruction("JPOS", "<="));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;    
                    }
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                    }
                    if(x.name == "0"){
                        code.pop_back();
                        code.push_back(Instruction("LOADI", "0"));
                        code.push_back(Instruction("JNEG", "<="));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;                        
                    }
                    else{
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("SUBI", "_pointer_"));
                        code.push_back(Instruction("JPOS", "<="));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;   
                    }
                    break;
            }
            break;
        case val:
            switch (y.type) {
                case num:
                    if(y.name == "0"){
                        code.push_back(Instruction("LOAD", x.name));
                        code.push_back(Instruction("JPOS", "<="));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;                          
                    }
                    else{
                        code.push_back(Instruction("SET", y.name));
                        code.push_back(Instruction("SUB", x.name));
                        code.push_back(Instruction("JNEG", "<="));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;   
                    }
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("SUB", y.name));
                    code.push_back(Instruction("JPOS", "<="));
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_pointer_"));
                            break;
                    }
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("SUBI", "_pointer_"));
                    code.push_back(Instruction("JPOS", "<="));
                    break;
            }
            break;
        case arr:
            switch (x.indexType) {
                case num:
                    code.push_back(Instruction("SET", x.index));
                    code.push_back(Instruction("ADD", x.name));
                    code.push_back(Instruction("STORE", "_pointer_"));
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("ADD", x.index));
                    code.push_back(Instruction("STORE", "_pointer_"));
                    break;
            }
            switch (y.type) {
                case num:
                    if(y.name == "0"){
                        code.pop_back();
                        code.push_back(Instruction("LOADI", "0"));
                        code.push_back(Instruction("JPOS", "<=")); 
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;                       
                    }
                    else{
                        code.push_back(Instruction("SET", y.name));
                        code.push_back(Instruction("SUBI", "_pointer_"));
                        code.push_back(Instruction("JNEG", "<="));
                        types.name = x.name;
                        types.type = x.type;
                        types.index = y.name;
                        types.indexType = y.type;   
                    }
                    break;
                case val:
                    code.pop_back();
                    code.push_back(Instruction("LOADI", "0"));
                    code.push_back(Instruction("SUB", y.name));
                    code.push_back(Instruction("JPOS", "<="));
                    break;
                case arr:
                    
                    break;
            }
            break;
    }
    return code;
}
