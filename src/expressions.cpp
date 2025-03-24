#include "expressions.hpp"

vector<Instruction> Expressions::valAssign(const Types &value){
    vector<Instruction> code;
    switch (value.type) {
        case num:
            code.push_back(Instruction("SET", value.name));
            break;
        case val:
            code.push_back(Instruction("LOAD", value.name));
            break;
        case arr:
            switch (value.indexType) {
                case num:
                    code.push_back(Instruction("SET", value.index));
                    code.push_back(Instruction("ADD", value.name));
                    code.push_back(Instruction("LOADI", "0"));
                    break;
                case val:
                    code.push_back(Instruction("LOAD", value.name));
                    code.push_back(Instruction("ADD", value.index));
                    code.push_back(Instruction("LOADI", "0"));
                    break;
            }
            break;
    }
    return code;
}
vector<Instruction> Expressions::plus(const Types &x, const Types &y){
    vector<Instruction> code;
    switch (x.type) {
        case num:
            switch (y.type) {
                case num:
                    code.push_back(Instruction("SET", y.name));
                    code.push_back(Instruction("STORE", "_acc_"));
                    code.push_back(Instruction("SET", x.name));
                    code.push_back(Instruction("ADD", "_acc_"));
                    break;
                case val:
                    code.push_back(Instruction("SET", x.name));
                    code.push_back(Instruction("ADD", y.name));
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_acc_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_acc_"));
                            break;
                    }
                    code.push_back(Instruction("SET", x.name));
                    code.push_back(Instruction("ADDI", "_acc_"));
                    break;
            }
            break;
        case val:
            switch (y.type) {
                case num:
                    code.push_back(Instruction("SET", y.name));
                    code.push_back(Instruction("ADD", x.name));
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("ADD", y.name));
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_acc_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_acc_"));
                            break;
                    }
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("ADDI", "_acc_"));
                    break;
            }
            break;
        case arr:
            switch (x.indexType) {
                case num:
                    code.push_back(Instruction("SET", x.index));
                    code.push_back(Instruction("ADD", x.name));
                    code.push_back(Instruction("STORE", "_acc_"));
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("ADD", x.index));
                    code.push_back(Instruction("STORE", "_acc_"));
                    break;
            }
            switch (y.type) {
                case num:
                    code.push_back(Instruction("SET", y.name));
                    code.push_back(Instruction("ADDI", "_acc_"));
                    break;
                case val:
                    code.pop_back();
                    code.push_back(Instruction("LOADI", "0"));
                    code.push_back(Instruction("ADD", y.name));
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_cca_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_cca_"));
                            break;
                    }
                    code.push_back(Instruction("LOADI", "_acc_"));
                    code.push_back(Instruction("ADDI", "_cca_"));
                    break;
            }
            break;
    }
    return code;
}
vector<Instruction> Expressions::minus(const Types &x, const Types &y){
    vector<Instruction> code;
    switch (x.type) {
        case num:
            switch (y.type) {
                case num:
                    code.push_back(Instruction("SET", y.name));
                    code.push_back(Instruction("STORE", "_acc_"));
                    code.push_back(Instruction("SET", x.name));
                    code.push_back(Instruction("SUB", "_acc_"));
                    break;
                case val:
                    code.push_back(Instruction("SET", x.name));
                    code.push_back(Instruction("SUB", y.name));
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_acc_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_acc_"));
                            break;
                    }
                    code.push_back(Instruction("SET", x.name));
                    code.push_back(Instruction("SUBI", "_acc_"));
                    break;
            }
            break;
        case val:
            switch (y.type) {
                case num:
                    code.push_back(Instruction("SET", y.name));
                    code.push_back(Instruction("STORE", "_acc_"));
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("SUB", "_acc_"));
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("SUB", y.name));
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_acc_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_acc_"));
                            break;
                    }
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("SUBI", "_acc_"));
                    break;
            }
            break;
        case arr:
            switch (x.indexType) {
                case num:
                    code.push_back(Instruction("SET", x.index));
                    code.push_back(Instruction("ADD", x.name));
                    code.push_back(Instruction("STORE", "_cca_"));
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("ADD", x.index));
                    code.push_back(Instruction("STORE", "_cca_"));
                    break;
            }
            switch (y.type) {
                case num:
                    code.push_back(Instruction("SET", y.name));
                    code.push_back(Instruction("STORE", "_acc_"));
                    code.push_back(Instruction("LOADI", "_cca_"));
                    code.push_back(Instruction("SUB", "_acc_"));
                    break;
                case val:
                    code.pop_back();
                    code.push_back(Instruction("LOADI", "0"));
                    code.push_back(Instruction("SUB", y.name));
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_cca_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_cca_"));
                            break;
                    }
                    code.push_back(Instruction("LOADI", "_acc_"));
                    code.push_back(Instruction("SUBI", "_cca_"));
                    break;
            }
            break;
    }
    return code;
}
vector<Instruction> Expressions::mult(const Types &x, const Types &y){
    vector<Instruction> code;
    vector<Instruction> mlty = getMult();
    switch (x.type) {
        case num:
            switch (y.type) {
                case num:
                    if(x.name == "2"){
                        code.push_back(Instruction("SET", y.name));
                        code.push_back(Instruction("STORE", "_acc_"));
                        code.push_back(Instruction("ADD", "_acc_"));
                    }
                    else if (y.name == "2"){
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("STORE", "_acc_"));
                        code.push_back(Instruction("ADD", "_acc_"));
                    }
                    else if(x.name == "0" || y.name == "0"){
                        code.push_back(Instruction("SET", "0"));
                    }
                    else{
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("STORE", "_x_"));
                        code.push_back(Instruction("SET", y.name));
                        code.push_back(Instruction("STORE", "_y_"));
                        for (const auto &ins : mlty)
                            code.push_back(ins); 
                    }
                    break;
                case val:
                    if(x.name == "2"){
                        code.push_back(Instruction("LOAD", y.name));
                        code.push_back(Instruction("ADD", y.name));
                    }
                    else if(x.name == "0"){
                        code.push_back(Instruction("SET", "0"));
                    }
                    else{
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("STORE", "_x_"));
                        code.push_back(Instruction("LOAD", y.name));
                        code.push_back(Instruction("STORE", "_y_"));
                        for (const auto &ins : mlty)
                            code.push_back(ins); 
                    }
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_acc_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_acc_"));
                            break;
                    }
                    if(x.name == "2"){
                        code.push_back(Instruction("LOAD", y.name));
                        code.push_back(Instruction("ADD", y.name));
                    }
                    else if(x.name == "0"){
                        code.push_back(Instruction("SET", "0"));
                    }
                    else{
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("STORE", "_x_"));
                        code.push_back(Instruction("LOADI", "_acc_"));
                        code.push_back(Instruction("STORE", "_y_"));
                        for (const auto &ins : mlty)
                            code.push_back(ins); 
                    }
                    break;
            }
            break;
        case val:
            switch (y.type) {
                case num:
                    if(y.name == "2"){
                        code.push_back(Instruction("LOAD", x.name));
                        code.push_back(Instruction("ADD", x.name));
                    }
                    else if(y.name == "0"){
                        code.push_back(Instruction("SET", "0"));
                    }
                    else{                        
                        code.push_back(Instruction("LOAD", x.name));
                        code.push_back(Instruction("STORE", "_x_"));
                        code.push_back(Instruction("SET", y.name));
                        code.push_back(Instruction("STORE", "_y_"));
                        for (const auto &ins : mlty)
                            code.push_back(ins); 
                    }
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("STORE", "_x_"));
                    code.push_back(Instruction("LOAD", y.name));
                    code.push_back(Instruction("STORE", "_y_"));
                    for (const auto &ins : mlty)
                        code.push_back(ins); 
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_acc_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_acc_"));
                            break;
                    }
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("STORE", "_x_"));
                    code.push_back(Instruction("LOADI", "_acc_"));
                    code.push_back(Instruction("STORE", "_y_"));
                    for (const auto &ins : mlty)
                        code.push_back(ins); 
                    break;
            }
            break;
        case arr:
            switch (x.indexType) {
                case num:
                    code.push_back(Instruction("SET", x.index));
                    code.push_back(Instruction("ADD", x.name));
                    code.push_back(Instruction("STORE", "_acc_"));
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("ADD", x.index));
                    code.push_back(Instruction("STORE", "_acc_"));
                    break;
            }
            switch (y.type) {
                case num:
                    code.pop_back();
                    code.push_back(Instruction("LOADI", "0"));
                    code.push_back(Instruction("STORE", "_x_"));
                    code.push_back(Instruction("SET", y.name));
                    code.push_back(Instruction("STORE", "_y_"));
                    for (const auto &ins : mlty)
                        code.push_back(ins); 
                    break;
                case val:
                    code.pop_back();
                    code.push_back(Instruction("LOADI", "0"));
                    code.push_back(Instruction("STORE", "_x_"));
                    code.push_back(Instruction("SET", y.name));
                    code.push_back(Instruction("STORE", "_y_"));
                    for (const auto &ins : mlty)
                        code.push_back(ins); 
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_cca_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_cca_"));
                            break;
                    }
                    code.push_back(Instruction("LOADI", "_acc_"));
                    code.push_back(Instruction("STORE", "_x_"));
                    code.push_back(Instruction("LOADI", "_cca_"));
                    code.push_back(Instruction("STORE", "_y_"));
                    for (const auto &ins : mlty)
                        code.push_back(ins); 
                    break;
            }
            break;
    }
    return code;
}
vector<Instruction> Expressions::div(const Types &x, const Types &y){
    vector<Instruction> code;
    vector<Instruction> divide = getDiv();
    switch (x.type) {
        case num:
            switch (y.type) {
                case num:
                    if(abs(stoi(x.name)) < abs(stoi(y.name))){
                        code.push_back(Instruction("SET", "0"));
                    }
                    else if(x.name == y.name){
                        code.push_back(Instruction("SET", "1"));
                    }
                    else if (y.name == "2"){
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("HALF", ""));
                    }
                    else if(x.name == "0" || y.name == "0"){
                        code.push_back(Instruction("SET", "0"));
                    }
                    else{
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("STORE", "_x_"));
                        code.push_back(Instruction("SET", y.name));
                        code.push_back(Instruction("STORE", "_y_"));
                        for (const auto &ins : divide)
                            code.push_back(ins); 
                    }
                    break;
                case val:
                    if(x.name == "0"){
                        code.push_back(Instruction("SET", "0"));
                    }
                    else{
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("STORE", "_x_"));
                        code.push_back(Instruction("LOAD", y.name));
                        code.push_back(Instruction("STORE", "_y_"));
                        for (const auto &ins : divide)
                            code.push_back(ins); 
                    }
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_acc_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_acc_"));
                            break;
                    }
                    if(x.name == "0"){
                        code.push_back(Instruction("SET", "0"));
                    }
                    else{
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("STORE", "_x_"));
                        code.push_back(Instruction("LOADI", "_acc_"));
                        code.push_back(Instruction("STORE", "_y_"));
                        for (const auto &ins : divide)
                            code.push_back(ins); 
                    }
                    break;
            }
            break;
        case val:
            switch (y.type) {
                case num:
                    if(y.name == "2"){
                        code.push_back(Instruction("LOAD", x.name));
                        code.push_back(Instruction("HALF", ""));
                    }
                    else if(y.name == "0"){
                        code.push_back(Instruction("SET", "0"));
                    }
                    else{
                        code.push_back(Instruction("LOAD", x.name));
                        code.push_back(Instruction("STORE", "_x_"));
                        code.push_back(Instruction("SET", y.name));
                        code.push_back(Instruction("STORE", "_y_"));
                        for (const auto &ins : divide)
                            code.push_back(ins); 
                    }
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("STORE", "_x_"));
                    code.push_back(Instruction("LOAD", y.name));
                    code.push_back(Instruction("STORE", "_y_"));
                    for (const auto &ins : divide)
                        code.push_back(ins); 
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_acc_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_acc_"));
                            break;
                    }
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("STORE", "_x_"));
                    code.push_back(Instruction("LOADI", "_acc_"));
                    code.push_back(Instruction("STORE", "_y_"));
                    for (const auto &ins : divide)
                        code.push_back(ins); 
                    break;
            }
            break;
        case arr:
            switch (x.indexType) {
                case num:
                    code.push_back(Instruction("SET", x.index));
                    code.push_back(Instruction("ADD", x.name));
                    code.push_back(Instruction("STORE", "_acc_"));
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("ADD", x.index));
                    code.push_back(Instruction("STORE", "_acc_"));
                    break;
            }
            switch (y.type) {
                case num:
                    code.pop_back();
                    code.push_back(Instruction("LOADI", "0"));
                    code.push_back(Instruction("STORE", "_x_"));
                    code.push_back(Instruction("SET", y.name));
                    code.push_back(Instruction("STORE", "_y_"));
                    for (const auto &ins : divide)
                        code.push_back(ins); 
                    break;
                case val:
                    code.pop_back();
                    code.push_back(Instruction("LOADI", "0"));
                    code.push_back(Instruction("STORE", "_x_"));
                    code.push_back(Instruction("SET", y.name));
                    code.push_back(Instruction("STORE", "_y_"));
                    for (const auto &ins : divide)
                        code.push_back(ins); 
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_cca_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_cca_"));
                            break;
                    }
                    code.push_back(Instruction("LOADI", "_acc_"));
                    code.push_back(Instruction("STORE", "_x_"));
                    code.push_back(Instruction("LOADI", "_cca_"));
                    code.push_back(Instruction("STORE", "_y_"));
                    for (const auto &ins : divide)
                        code.push_back(ins); 
                    break;
            }
            break;
    }
    return code;
}
vector<Instruction> Expressions::mod(const Types &x, const Types &y){
    vector<Instruction> code;
    vector<Instruction> modulo = getMod();
    switch (x.type) {
        case num:
            switch (y.type) {
                case num:
                    if(abs(stoi(x.name)) < abs(stoi(y.name))){
                        code.push_back(Instruction("SET", x.name));
                    }
                    else if(x.name == y.name){
                        code.push_back(Instruction("SET", "0"));
                    }
                    else if(x.name == "0" || y.name == "0"){
                        code.push_back(Instruction("SET", "0"));
                    }
                    else{
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("STORE", "_x_"));
                        code.push_back(Instruction("SET", y.name));
                        code.push_back(Instruction("STORE", "_y_"));
                        for (const auto &ins : modulo)
                            code.push_back(ins); 
                    }
                    break;
                case val:
                    if(x.name == "0"){
                        code.push_back(Instruction("SET", "0"));
                    }
                    else{
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("STORE", "_x_"));
                        code.push_back(Instruction("LOAD", y.name));
                        code.push_back(Instruction("STORE", "_y_"));
                        for (const auto &ins : modulo)
                            code.push_back(ins); 
                    }
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_acc_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_acc_"));
                            break;
                    }
                    if(x.name == "0"){
                        code.push_back(Instruction("SET", "0"));
                    }
                    else{
                        code.push_back(Instruction("SET", x.name));
                        code.push_back(Instruction("STORE", "_x_"));
                        code.push_back(Instruction("LOADI", "_acc_"));
                        code.push_back(Instruction("STORE", "_y_"));
                        for (const auto &ins : modulo)
                            code.push_back(ins); 
                    }
                    break;
            }
            break;
        case val:
            switch (y.type) {
                case num:
                    if(y.name == "0"){
                        code.push_back(Instruction("SET", "0"));
                    }
                    else{
                        code.push_back(Instruction("LOAD", x.name));
                        code.push_back(Instruction("STORE", "_x_"));
                        code.push_back(Instruction("SET", y.name));
                        code.push_back(Instruction("STORE", "_y_"));
                        for (const auto &ins : modulo)
                            code.push_back(ins); 
                    }
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("STORE", "_x_"));
                    code.push_back(Instruction("LOAD", y.name));
                    code.push_back(Instruction("STORE", "_y_"));
                    for (const auto &ins : modulo)
                        code.push_back(ins); 
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_acc_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_acc_"));
                            break;
                    }
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("STORE", "_x_"));
                    code.push_back(Instruction("LOADI", "_acc_"));
                    code.push_back(Instruction("STORE", "_y_"));
                    for (const auto &ins : modulo)
                        code.push_back(ins); 
                    break;
            }
            break;
        case arr:
            switch (x.indexType) {
                case num:
                    code.push_back(Instruction("SET", x.index));
                    code.push_back(Instruction("ADD", x.name));
                    code.push_back(Instruction("STORE", "_acc_"));
                    break;
                case val:
                    code.push_back(Instruction("LOAD", x.name));
                    code.push_back(Instruction("ADD", x.index));
                    code.push_back(Instruction("STORE", "_acc_"));
                    break;
            }
            switch (y.type) {
                case num:
                    code.pop_back();
                    code.push_back(Instruction("LOADI", "0"));
                    code.push_back(Instruction("STORE", "_x_"));
                    code.push_back(Instruction("SET", y.name));
                    code.push_back(Instruction("STORE", "_y_"));
                    for (const auto &ins : modulo)
                        code.push_back(ins); 
                    break;
                case val:
                    code.pop_back();
                    code.push_back(Instruction("LOADI", "0"));
                    code.push_back(Instruction("STORE", "_x_"));
                    code.push_back(Instruction("SET", y.name));
                    code.push_back(Instruction("STORE", "_y_"));
                    for (const auto &ins : modulo)
                        code.push_back(ins); 
                    break;
                case arr:
                    switch (y.indexType) {
                        case num:
                            code.push_back(Instruction("SET", y.index));
                            code.push_back(Instruction("ADD", y.name));
                            code.push_back(Instruction("STORE", "_cca_"));
                            break;
                        case val:
                            code.push_back(Instruction("LOAD", y.name));
                            code.push_back(Instruction("ADD", y.index));
                            code.push_back(Instruction("STORE", "_cca_"));
                            break;
                    }
                    code.push_back(Instruction("LOADI", "_acc_"));
                    code.push_back(Instruction("STORE", "_x_"));
                    code.push_back(Instruction("LOADI", "_cca_"));
                    code.push_back(Instruction("STORE", "_y_"));
                    for (const auto &ins : modulo)
                        code.push_back(ins); 
                    break;
            }
            break;

    }
    return code;
}
