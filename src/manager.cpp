#include "manager.hpp"

void Manager::initProc(const string& nameProc) {
    if (nameProc == "main") {
        yyerror("procedure name 'main' cannot be declared");
        return;
    }
    if (declProc.find(nameProc) != declProc.end()) {
        string errMsg = "procedure " + nameProc + " already declared";
        yyerror(errMsg.c_str());
        return;
    }
    int procAdr = memoryAdr++;
    Procedure proc(procAdr, nameProc);
    declProc.insert({nameProc, proc});
    currProc = nameProc;
    cout << "[Info] initialized procedure: " << nameProc << " (adr=" << procAdr << ")" << endl;
    for (Value &v : buff) {
        if (v.checkArg() && v.getAddress() == -1) {
            int argAdr = memoryAdr++;
            v.setAddress(argAdr);
            cout << "[Info] added argument " << (v.getType() == Type::val ? "variable" : "array") 
            << ": " << v.getName()
            << " (adr=" << argAdr << ")" << endl;
        }
    }
}

void Manager::addProc(const string& nameProc) {
    auto it = declProc.find(nameProc);
    if (it == declProc.end()) {
        if (declProc.find("main") == declProc.end()) {
            int mainAdr = 0;
            Procedure proc(mainAdr, "main");
            declProc.insert({"main", proc});
            it = declProc.find("main");
        } else {
            it = declProc.find("main");
        }
    }
    Procedure &proc = it->second;
    unordered_map<string, int> freq;
    for (Value &v : buff) {
        freq[v.getName()]++;
        if (freq[v.getName()] > 1 && v.getType() != Type::acc) {
            yyerror(("duplicate identifier usage: " + v.getName()).c_str());
            return;
        }
    }

    for (Value &v : buff) {
        if (v.checkArg())
            proc.addArg(v);
        else
            proc.addVar(v);
    }
    proc.setReturn(rtrnLine);
    currProc = "main";
    buff.clear();
    cout << "[Info] added procedure: " << nameProc 
         << " (proc adr=" << proc.getAddress() << ") in line:"
         << proc.getReturn() << " with " 
         << proc.getVar().size() << " vars and " 
         << proc.getArg().size() << " args" << endl;
}


void Manager::bufferVar(const string& name) {
    auto it = find_if(buff.begin(), buff.end(), [&name](Value &v) {
        return v.getName() == name;
    });
    if (it != buff.end()) {
        yyerror(("duplicate identifier usage: " + name).c_str());
        return;
    }
    int adr = memoryAdr++;
    Value var(adr, name, Type::val);
    buff.push_back(var);
    cout << "[Info] added variable: " << name << " (adr=" << adr << ")" << endl;
}

void Manager::bufferArr(const string& name, const string& low, const string& up) {
    int l, u;
    try {
        l = stoi(low);
        u = stoi(up);
    }
    catch (const std::exception& e) {
        yyerror("array bounds must be NUM");
        return;
    }
    if (l > u) {
        yyerror("invalid array bounds");
        return;
    }
    auto it = find_if(buff.begin(), buff.end(), [&name](Value &v) {
        return v.getName() == name;
    });
    if (it != buff.end()) {
        yyerror(("duplicate identifier usage: " + name).c_str());
        return;
    }
    int adr = memoryAdr++;
    Value arr(adr, name, l, u);
    buff.push_back(arr);
    size_t size = static_cast<size_t>(abs(u - l) + 1);
    int idxAdr = adr;
    for (size_t i = 0; i < size; ++i) {
        adr = memoryAdr++;
        Value arr(adr, name+"["+to_string(i+l)+"]", 0, 0, true);
        buff.push_back(arr);
    }
    cout << "[Info] added variable array: " << name 
    << " with bounds [" << l << ":" << u << "] (adr=("<< idxAdr<< ":" << adr <<"))" << endl;
}

void Manager::bufferArg(const string& name, Type type) {
    auto it = find_if(buff.begin(), buff.end(), [&name](Value &v) {
        return v.getName() == name;
    });
    if (it != buff.end()) {
        yyerror(("duplicate identifier usage: " + name).c_str());
        return;
    }
    int adr = -1;
    Value arg(adr, name, type, true);
    buff.push_back(arg);
}

void Manager::bufferProcCall(const string& name){
    buffCallArgs.push_back(name);
}

int Manager::getAdr(const string &name, const string &nameProc) {
    auto it = declProc.find(nameProc);
    if (it != declProc.end()) {
        Procedure &proc = it->second;
        for (Value &v : proc.getArg()) {
            if (v.getName() == name)
                return v.getAddress();
        }
        for (Value &v : proc.getVar()) {
            if (v.getName() == name)
                return v.getAddress();
        }
    }
    return -1;
}

int Manager::initIter(const string& iter){
    auto it = find_if(buff.begin(), buff.end(), [&iter](Value &v) {
        return v.getName() == iter;
    });
    if (it != buff.end()) {
        return it->getAddress();
    }
    else{
        int adr = memoryAdr++;
        Value iterator(adr, iter, Type::acc);
        buff.push_back(iterator);
        cout << "[Info] added iterator: " << iter << " (adr=" << adr << ")" << endl;
        memoryAdr++;
        Value counter(adr+1, iter+"'", Type::acc);
        buff.push_back(counter);
        cout << "[Info] added counter: " << iter+"'" << " (adr=" << adr+1 << ")" << endl;
        return adr;
    }
}

int Manager::addAcc(const string& name){
    auto it = find_if(buff.begin(), buff.end(), [&name](Value &v) {
        return v.getName() == name;
    });
    if (it != buff.end()) {
        return it->getAddress();
    }
    else{
        int adr = memoryAdr++;
        Value acc(adr, name, Type::acc);
        buff.push_back(acc);
        cout << "[Info] added help acc: " << name << " (adr=" << adr << ")" << endl;
        return adr;
    }
}

bool Manager::findArg(const string &name, const string &nameProc) {
    auto it = declProc.find(nameProc);
    if (it != declProc.end()) {
        Procedure &proc = it->second;
        for (Value &v : proc.getArg()) {
            if (v.getName() == name)
                return true;
        }
    }
    return false;
}


void Manager::initVar(const Types &ident) {
    string name = ident.name;
    auto it = find_if(buff.begin(), buff.end(), [&name](Value &v) {
         return v.getName() == name;
    });
    if (it != buff.end()) {
         it->setInit();
    } else {
         yyerror(("undeclared variable: " + name).c_str());
    }
}

void Manager::checkVarUsage(const string &name, bool isArrayUsage) {
    bool found = false;
    Type foundType;
    for (Value &v : buff) {
        if (v.getName() == name) {
            found = true;
            foundType = v.getType();
            break;
        }
    }
    if (!found) {
        yyerror(("undeclared variable: " + name).c_str());
        return;
    }
    if (isArrayUsage && foundType != Type::arr) {
        yyerror(("incorrect variable usage: " + name).c_str());
        return;
    }
    if (!isArrayUsage && foundType == Type::arr) {
        yyerror(("incorrect array usage: " + name).c_str());
        return;
    }
}

void Manager::checkProcCall(const string &procName) {
    if (procName == currProc) {
        yyerror(("recursion is not allowed: " + procName).c_str());
        return;
    }
    Procedure &proc = declProc[procName];
    size_t formalCount = proc.getArg().size();
    if (formalCount != buffCallArgs.size()) {
        yyerror(("incorrect parameters for procedure " + procName + ": expected " +
                 to_string(formalCount) + " parameters, got " +
                 to_string(buffCallArgs.size())).c_str());
        return;
    }
    for (size_t i = 0; i < buffCallArgs.size(); ++i) {
        string actualName = buffCallArgs[i];
        auto it = find_if(buff.begin(), buff.end(), [&actualName](Value &v) {
            return v.getName() == actualName;
        });
        if (it == buff.end()) {
            yyerror(("undeclared variable: " + actualName).c_str());
            continue;
        }
        Type actualType = it->getType();
        Type formalType = proc.getArg()[i].getType();
        if (actualType != formalType) {
            yyerror(("incorrect argument type for procedure " + procName + ": '" 
                     + actualName).c_str());
        }
        it->setInit();
    }
}

void Manager::checkInit() {
    for (auto &p : declProc) {
        Procedure &proc = p.second;
        for (Value &v : proc.getVar()) {
            if (!v.checkInit() && v.getType() != Type::acc) {
                cout << "[Warning] uninitialized variable: " << v.getName() 
                     << " in procedure " << p.first << "\n";
            }
        }
    }
}
