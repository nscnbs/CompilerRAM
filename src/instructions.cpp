#include "instructions.hpp"

vector<Instruction> GenerateInstruction::connectRead(const Types &ident) {
    vector<Instruction> code;
    switch (ident.type) {
        case val:
            code.push_back(Instruction("GET", ident.name));
            break;
        case arr:
            switch (ident.indexType) {
                case num:
                    code.push_back(Instruction("SET", ident.index));
                    code.push_back(Instruction("ADD", ident.name));
                    code.push_back(Instruction("STORE", "_pointer_"));
                    code.push_back(Instruction("GET", "0"));
                    code.push_back(Instruction("STOREI", "_pointer_"));
                    break;
                case val:
                    code.push_back(Instruction("LOAD", ident.name));
                    code.push_back(Instruction("ADD", ident.index));
                    code.push_back(Instruction("STORE", "_pointer_"));
                    code.push_back(Instruction("GET", "0"));
                    code.push_back(Instruction("STOREI", "_pointer_"));
                    break;
            }
            break;
    }
    return code;
}

vector<Instruction> GenerateInstruction::connectWrite(const Types &value) {
    vector<Instruction> code;
    switch (value.type) {
        case num:
            code.push_back(Instruction("SET", value.name));
            code.push_back(Instruction("PUT", "0"));
            break;
        case val:
            code.push_back(Instruction("PUT", value.name));
            break;
        case arr:
            switch (value.indexType) {
                case num:
                    code.push_back(Instruction("SET", value.index));
                    code.push_back(Instruction("ADD", value.name));
                    code.push_back(Instruction("LOADI", "0"));
                    code.push_back(Instruction("PUT", "0"));
                    break;
                case val:
                    code.push_back(Instruction("LOAD", value.name));
                    code.push_back(Instruction("ADD", value.index));
                    code.push_back(Instruction("LOADI", "0"));
                    code.push_back(Instruction("PUT", "0"));
                    break;
            }
            break;
    }
    return code;
}


vector<Instruction> GenerateInstruction::connectAssign(const Types &ident, const vector<Instruction> &exprs) {
    vector<Instruction> code;
    switch (ident.type) {
        case val:
            code = exprs;
            code.push_back(Instruction("STORE", ident.name));
            break;
        case arr:
            switch (ident.indexType) {
                case num:
                    code.push_back(Instruction("SET", ident.index));
                    code.push_back(Instruction("ADD", ident.name));
                    code.push_back(Instruction("STORE", "_pointer_"));
                    for (const auto &ins : exprs)
                        code.push_back(ins);
                    code.push_back(Instruction("STOREI", "_pointer_"));
                    break;
                case val:
                    code.push_back(Instruction("LOAD", ident.name));
                    code.push_back(Instruction("ADD", ident.index));
                    code.push_back(Instruction("STORE", "_pointer_"));
                    for (const auto &ins : exprs)
                        code.push_back(ins);
                    code.push_back(Instruction("STOREI", "_pointer_"));
                    break;
            }
            break;
    }
    return code;
}

vector<Instruction> GenerateInstruction::connectIf(const vector<Instruction> &cond, const vector<Instruction> &then) {
    int signCode = condition.extractSignCode(cond);
    int jumpIndex = 0;
    for (size_t i = 0; i < cond.size(); ++i) {
        if (cond[i].op == "JPOS" || cond[i].op == "JNEG" || cond[i].op == "JZERO") {
            jumpIndex = i;
            break;
        }
    }
    int jumpLine = 2;
    int endLine = then.size()+1;
    vector<Instruction> code = cond;

    switch(signCode) {
        case SIG_EQ:
            code[jumpIndex].arg = to_string(jumpLine);
            code.push_back(Instruction("JUMP", to_string(endLine)));
            break;
        case SIG_NE:
            code[jumpIndex].arg = to_string(endLine);
            break;
        case SIG_GT:
            code[jumpIndex].arg = to_string(jumpLine);
            code.push_back(Instruction("JUMP", to_string(endLine)));
            break;
        case SIG_LT:
            code[jumpIndex].arg = to_string(jumpLine);
            code.push_back(Instruction("JUMP", to_string(endLine)));
            break;
        case SIG_GE:
            code[jumpIndex].arg = to_string(endLine);
            break;
        case SIG_LE:
            code[jumpIndex].arg = to_string(endLine);
            break;
    }
    for (const auto &ins : then) {
        code.push_back(ins);
    }
    return code;
}


vector<Instruction> GenerateInstruction::connectIfElse(const vector<Instruction> &cond,
                                                         const vector<Instruction> &then,
                                                         const vector<Instruction> &elsee) {
    int signCode = condition.extractSignCode(cond);
    int jumpIndex = 0;
    for (size_t i = 0; i < cond.size(); ++i) {
        if (cond[i].op == "JPOS" || cond[i].op == "JNEG" || cond[i].op == "JZERO") {
            jumpIndex = i;
            break;
        }
    }
    int jumpLine = 0;
    int endLine = 0;
    vector<Instruction> code = cond;

    switch(signCode) {
        case SIG_EQ:
            jumpLine = elsee.size()+2;
            endLine = then.size()+1;
            code[jumpIndex].arg = to_string(jumpLine);
            for (const auto &ins : elsee) {
                code.push_back(ins);
            }
            code.push_back(Instruction("JUMP", to_string(endLine)));
            for (const auto &ins : then) {
                code.push_back(ins);
            }
            break;
        case SIG_NE:
            jumpLine = then.size()+2;
            endLine = elsee.size()+1;
            code[jumpIndex].arg = to_string(jumpLine);
            for (const auto &ins : then) {
                code.push_back(ins);
            }
            code.push_back(Instruction("JUMP", to_string(endLine)));
            for (const auto &ins : elsee) {
                code.push_back(ins);
            }
            break;
        case SIG_GT:
            jumpLine = elsee.size()+2;
            endLine = then.size()+1;
            code[jumpIndex].arg = to_string(jumpLine);
            for (const auto &ins : elsee) {
                code.push_back(ins);
            }
            code.push_back(Instruction("JUMP", to_string(endLine)));
            for (const auto &ins : then) {
                code.push_back(ins);
            }
            break;
        case SIG_LT:
            jumpLine = elsee.size()+2;
            endLine = then.size()+1;
            code[jumpIndex].arg = to_string(jumpLine);
            for (const auto &ins : elsee) {
                code.push_back(ins);
            }
            code.push_back(Instruction("JUMP", to_string(endLine)));
            for (const auto &ins : then) {
                code.push_back(ins);
            }
            break;
        case SIG_GE:
            jumpLine = then.size()+2;
            endLine = elsee.size()+1;
            code[jumpIndex].arg = to_string(jumpLine);
            for (const auto &ins : then) {
                code.push_back(ins);
            }
            code.push_back(Instruction("JUMP", to_string(endLine)));
            for (const auto &ins : elsee) {
                code.push_back(ins);
            }
            break;
        case SIG_LE:
            jumpLine = then.size()+2;
            endLine = elsee.size()+1;
            code[jumpIndex].arg = to_string(jumpLine);
            for (const auto &ins : then) {
                code.push_back(ins);
            }
            code.push_back(Instruction("JUMP", to_string(endLine)));
            for (const auto &ins : elsee) {
                code.push_back(ins);
            }
            break;
    }
    return code;
}


vector<Instruction> GenerateInstruction::connectWhile(const vector<Instruction> &cond, const vector<Instruction> &com) {
    int signCode = condition.extractSignCode(cond);
    Types type = condition.types;
    int jumpIndex = 0;
    for (size_t i = 0; i < cond.size(); ++i) {
        if (cond[i].op == "JPOS" || cond[i].op == "JNEG" || cond[i].op == "JZERO") {
            jumpIndex = i;
            break;
        }
    }
    int startLine = 0;
    int endLine = com.size()+2;
    vector<Instruction> code = cond;

    switch(signCode) {
        case SIG_EQ:
            code[jumpIndex].op = "JPOS";
            code[jumpIndex].arg = to_string(endLine+1);
            code.push_back(Instruction("JNEG", to_string(endLine)));
            break;
        case SIG_NE:
            code[jumpIndex].arg = to_string(endLine);
            break;
        case SIG_GT:
            if((type.type == Type::val || type.type == Type::arr) && (type.indexType == Type::num)){
                if(type.index == "0"){
                    code[jumpIndex].op = "JNEG";
                }
                else{
                    code[jumpIndex].op = "JPOS";
                } 
            }
            else if((type.type == num) && (type.indexType == Type::val || type.indexType == Type::arr)){
                if(type.name == "0"){
                    code[jumpIndex].op = "JPOS";
                }
                else{
                    code[jumpIndex].op = "JNEG";
                } 
            }
            else{
                code[jumpIndex].op = "JNEG";
            }
            code[jumpIndex].arg = to_string(endLine+1);
            code.push_back(Instruction("JZERO", to_string(endLine)));
            break;
        case SIG_LT:
            if((type.type == Type::val || type.type == Type::arr) && (type.indexType == Type::num)){
                if(type.index == "0"){
                    code[jumpIndex].op = "JPOS";
                }
                else{
                    code[jumpIndex].op = "JNEG";
                } 
            }
            else if((type.type == num) && (type.indexType == Type::val || type.indexType == Type::arr)){
                if(type.name == "0"){
                    code[jumpIndex].op = "JNEG";
                }
                else{
                    code[jumpIndex].op = "JPOS";
                } 
            }
            else{
                code[jumpIndex].op = "JPOS";
            }
            code[jumpIndex].arg = to_string(endLine+1);
            code.push_back(Instruction("JZERO", to_string(endLine)));
            break;
        case SIG_GE:
            code[jumpIndex].arg = to_string(endLine);
            break;
        case SIG_LE:
            code[jumpIndex].arg = to_string(endLine);
            break;
    }
    startLine = -(code.size()+com.size());
    for (const auto &ins : com) {
        code.push_back(ins);
    }
    code.push_back(Instruction("JUMP", to_string(startLine)));
    return code;
}


vector<Instruction> GenerateInstruction::connectRepeat(const vector<Instruction> &com, const vector<Instruction> &cond) {
    int signCode = condition.extractSignCode(cond);
    Types type = condition.types;
    int jumpIndex = 0;
    for (size_t i = 0; i < cond.size(); ++i) {
        if (cond[i].op == "JPOS" || cond[i].op == "JNEG" || cond[i].op == "JZERO") {
            jumpIndex = i;
            break;
        }
    }
    int startLine = -(cond.size()+com.size()-1);
    vector<Instruction> code = com;
    vector<Instruction> condit = cond;
    switch(signCode) {
        case SIG_EQ:
            condit[jumpIndex].op = "JNEG";
            condit[jumpIndex].arg = to_string(startLine);
            condit.push_back(Instruction("JPOS", to_string(startLine-1)));
            break;
        case SIG_NE:
            condit[jumpIndex].arg = to_string(startLine);
            break;
        case SIG_GT:
            if((type.type == Type::val || type.type == Type::arr) && (type.indexType == Type::num)){
                if(type.index == "0"){
                    condit[jumpIndex].op = "JNEG";
                }
                else{
                    condit[jumpIndex].op = "JPOS";
                } 
            }
            else if((type.type == num) && (type.indexType == Type::val || type.indexType == Type::arr)){
                if(type.name == "0"){
                    condit[jumpIndex].op = "JPOS";
                }
                else{
                    condit[jumpIndex].op = "JNEG";
                } 
            }
            else{
                condit[jumpIndex].op = "JNEG";
            }
            condit[jumpIndex].arg = to_string(startLine);
            condit.push_back(Instruction("JZERO", to_string(startLine-1)));
            break;
        case SIG_LT:
            if((type.type == Type::val || type.type == Type::arr) && (type.indexType == Type::num)){
                if(type.index == "0"){
                    condit[jumpIndex].op = "JPOS";
                }
                else{
                    condit[jumpIndex].op = "JNEG";
                } 
            }
            else if((type.type == num) && (type.indexType == Type::val || type.indexType == Type::arr)){
                if(type.name == "0"){
                    condit[jumpIndex].op = "JNEG";
                }
                else{
                    condit[jumpIndex].op = "JPOS";
                } 
            }
            else{
                condit[jumpIndex].op = "JPOS";
            }
            condit[jumpIndex].arg = to_string(startLine);
            condit.push_back(Instruction("JZERO", to_string(startLine-1)));
            break;
        case SIG_GE:
            condit[jumpIndex].arg = to_string(startLine);
            break;
        case SIG_LE:
            condit[jumpIndex].arg = to_string(startLine);
            break;
    }
    for (const auto &ins : condit) {
        code.push_back(ins);
    }
    return code;
}


vector<Instruction> GenerateInstruction::connectFor(const string &iter, const Types &low, const Types &up, const vector<Instruction> &com) {
    vector<Instruction> code;
    int iterAdr = manager.initIter(iter);
    string iterator = to_string(iterAdr);
    string counter = to_string(iterAdr+1);
    switch (low.type) {
        case num:
            code.push_back(Instruction("SET", low.name));
            break;
        case val:
            code.push_back(Instruction("LOAD", low.name));
            break;
        case arr:
            code.push_back(Instruction("LOADI", low.name));
            break;
    }
    code.push_back(Instruction("STORE", iterator));
    switch (up.type) {
        case num:
            code.push_back(Instruction("SET", up.name));
            code.push_back(Instruction("SUB", iterator));
            break;
        case val:
            code.push_back(Instruction("LOAD", up.name));
            code.push_back(Instruction("SUB", iterator));
            break;
        case arr:
            code.push_back(Instruction("LOADI", up.name));
            break;
    }
    int startLine = -(com.size()+7);
    int endLine = com.size()+8;
    code.push_back(Instruction("STORE", counter));
    code.push_back(Instruction("JNEG", to_string(endLine)));
    for (const auto &ins : com)
        code.push_back(ins);
    code.push_back(Instruction("SET", "1"));
    code.push_back(Instruction("ADD", iterator));
    code.push_back(Instruction("STORE", iterator));
    code.push_back(Instruction("SET", "-1"));
    code.push_back(Instruction("ADD", counter));
    code.push_back(Instruction("STORE", counter));
    code.push_back(Instruction("JUMP", to_string(startLine)));
    return code;
}


vector<Instruction> GenerateInstruction::connectForDown(const string &iter, const Types &low, const Types &up, const vector<Instruction> &com) {
    vector<Instruction> code;
    int iterAdr = manager.initIter(iter);
    string iterator = to_string(iterAdr);
    string counter = to_string(iterAdr+1);
    switch (low.type) {
        case num:
            code.push_back(Instruction("SET", low.name));
            break;
        case val:
            code.push_back(Instruction("LOAD", low.name));
            break;
        case arr:
            code.push_back(Instruction("LOADI", low.name));
            break;
    }
    code.push_back(Instruction("STORE", iterator));
    switch (up.type) {
        case num:
            code.push_back(Instruction("SET", up.name));
            code.push_back(Instruction("STORE", counter));
            code.push_back(Instruction("LOAD", iterator));
            code.push_back(Instruction("SUB", counter));
            break;
        case val:
            code.push_back(Instruction("LOAD", iterator));
            code.push_back(Instruction("SUB", up.name));
            break;
        case arr:
            code.push_back(Instruction("LOADI", up.name));
            break;
    }
    int startLine = -(com.size()+7);
    int endLine = com.size()+8;
    code.push_back(Instruction("STORE", counter));
    code.push_back(Instruction("JNEG", to_string(endLine)));
    for (const auto &ins : com)
        code.push_back(ins);
    code.push_back(Instruction("SET", "-1"));
    code.push_back(Instruction("ADD", iterator));
    code.push_back(Instruction("STORE", iterator));
    code.push_back(Instruction("SET", "-1"));
    code.push_back(Instruction("ADD", counter));
    code.push_back(Instruction("STORE", counter));
    code.push_back(Instruction("JUMP", to_string(startLine)));
    return code;
}

vector<Instruction> GenerateInstruction::connectCall(const string &nameProc) {
    vector<Instruction> code;
    vector<string> args = manager.buffCallArgs;
    vector<Value> procArgs;
    auto it = manager.declProc.find(nameProc);
    if (it != manager.declProc.end()) {
        procArgs = it->second.getArg();
    }
    for (size_t i = 0; i < args.size(); ++i) {
        code.push_back({"SET", args[i]});
        code.push_back({"STORE", to_string(procArgs[i].getAddress())});
    }
    manager.buffCallArgs.clear();
    code.push_back({"SET", "rtrnLine"});
    code.push_back({"STORE", nameProc});
    code.push_back({"JUMP", nameProc});
    return code;
}

vector<Instruction> GenerateInstruction::connectProc(const string &nameProc, const vector<Instruction> &com) {
    vector<Instruction> code = declarateArr();
    for (const auto &ins : com)
        code.push_back(ins);
    code.push_back(Instruction("RTRN", nameProc));
    code = replaceNamesAdrs(nameProc, code);
    for (const auto &ins : code) {
        procCode.push_back(ins);
        manager.rtrnLine++;
    }
    return procCode;
}

vector<Instruction> GenerateInstruction::connectMain(const vector<Instruction> &com) {
    vector<Instruction> code = declarateArr();
    for (const auto &ins : com)
        code.push_back(ins); 
    code = replaceNamesAdrs("main", code);
    return code;
}


void GenerateInstruction::generateCode(const vector<Instruction> &proc, const vector<Instruction> &main) {
    if(!proc.empty()){
        mergedCode.push_back(Instruction("JUMP", to_string(proc.size()+1)));
    }
    for (const auto &ins : proc)
        mergedCode.push_back(ins);
    for (const auto &ins : main)
        mergedCode.push_back(ins); 
    mergedCode.push_back(Instruction("HALT", ""));
    mergedCode = replaceCall(mergedCode);
}

vector<Instruction> GenerateInstruction::replaceNamesAdrs(const string &nameProc, vector<Instruction>& code) {
    string point = "_pointer_", acc = "_acc_", cca = "_cca_";
    string x = "_x_", y = "_y_", result = "_rslt_";
    string sign = "_sign_", temp = "_pwr_", counter = "_cnt_";
    for (auto& ins : code) {
        if (!ins.arg.empty()) {
            bool checkNumeric = all_of(ins.arg.begin(), ins.arg.end(), ::isdigit);
            if (!checkNumeric) {
                if(ins.arg == point){
                    int pointAdr = manager.addAcc(point);
                    ins.arg = to_string(pointAdr);
                }
                else if(ins.arg == acc){
                    int accAdr = manager.addAcc(acc);
                    ins.arg = to_string(accAdr);
                }
                else if(ins.arg == cca){
                    int ccaAdr = manager.addAcc(cca);
                    ins.arg = to_string(ccaAdr);
                }
                else if(ins.arg == x){
                    int xAdr = manager.addAcc(x);
                    ins.arg = to_string(xAdr);
                }
                else if(ins.arg == y){
                    int yAdr = manager.addAcc(y);
                    ins.arg = to_string(yAdr);
                }
                else if(ins.arg == result){
                    int resultAdr = manager.addAcc(result);
                    ins.arg = to_string(resultAdr);
                }
                else if(ins.arg == sign){
                    int signAdr = manager.addAcc(sign);
                    ins.arg = to_string(signAdr);
                }
                else if(ins.arg == temp){
                    int tempAdr = manager.addAcc(temp);
                    ins.arg = to_string(tempAdr);
                }
                else if(ins.arg == counter){
                    int counterAdr = manager.addAcc(counter);
                    ins.arg = to_string(counterAdr);
                }
                else{
                    bool forArg = manager.findArg(ins.arg, nameProc);
                    int adr = manager.getAdr(ins.arg, nameProc);
                    if (adr != -1) {
                        ins.arg = to_string(adr);
                        if (forArg && (ins.op == "LOAD" || ins.op == "STORE" ||
                                    ins.op == "ADD"  || ins.op == "SUB")) {
                            ins.op = ins.op + "I";
                        }
                    }
                }
            }
        }
    }
    return code;
}

vector<Instruction> GenerateInstruction::replaceCall(vector<Instruction>& code) {
    for (auto& ins : code) {
        for (auto &p : manager.declProc) {
            Procedure &proc = p.second;
            if (ins.op == "RTRN" && ins.arg == proc.getName())
            {
                ins.arg = to_string(proc.getAddress());
            }
            if (ins.op == "SET" && ins.arg == "rtrnLine")
            {
                ins.arg = to_string(codeLine + 3);
            }
            if (ins.op == "STORE" && ins.arg == proc.getName())
            {
                ins.arg = to_string(proc.getAddress());
            }
            if (ins.op == "JUMP" && ins.arg == proc.getName())
            {
                ins.arg = to_string(-codeLine + proc.getReturn());
            }
        }
        codeLine++;
    }
    return code;
}

vector<Instruction> GenerateInstruction::declarateArr() {
    vector<Instruction> code;
    for (auto &p : manager.declProc) {
        Procedure &proc = p.second;
        for (Value &v : proc.getVar()) {
            if (v.getType() == Type::arr && v.getSize()>1) {
                code.push_back(Instruction("SET", to_string(v.getAddress()+1-v.getLow())));
                code.push_back(Instruction("STORE", to_string(v.getAddress())));
            }
        }
    }
    return code;
}

const vector<Instruction>& GenerateInstruction::getCode() const {
    return mergedCode;
}

vector<Instruction> GenerateInstruction::connectEq(const Types &x, const Types &y) {
    return condition.equal(x, y);
}
vector<Instruction> GenerateInstruction::connectNeq(const Types &x, const Types &y) {
    return condition.nequal(x, y);
}
vector<Instruction> GenerateInstruction::connectGt(const Types &x, const Types &y) {
    return condition.grater(x, y);
}
vector<Instruction> GenerateInstruction::connectLt(const Types &x, const Types &y) {
    return condition.lower(x, y);
}
vector<Instruction> GenerateInstruction::connectGte(const Types &x, const Types &y) {
    return condition.graterEq(x, y);
}
vector<Instruction> GenerateInstruction::connectLte(const Types &x, const Types &y) {
    return condition.lowerEq(x, y);
}
vector<Instruction> GenerateInstruction::connectVal(const Types &value) {
    return expression.valAssign(value);
}
vector<Instruction> GenerateInstruction::connectPlus(const Types &x, const Types &y) {
    return expression.plus(x, y);
}
vector<Instruction> GenerateInstruction::connectMinus(const Types &x, const Types &y) {
    return expression.minus(x, y);
}
vector<Instruction> GenerateInstruction::connectMult(const Types &x, const Types &y) {
    return expression.mult(x, y);
}
vector<Instruction> GenerateInstruction::connectDiv(const Types &x, const Types &y) {
    return expression.div(x, y);
}
vector<Instruction> GenerateInstruction::connectMod(const Types &x, const Types &y) {
    return expression.mod(x, y);
}
