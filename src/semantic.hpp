#ifndef SEMANTIC_HPP
#define SEMANTIC_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>

enum Type {
    Var, Arr
};

struct Variable {
    std::string name;
    Type type;
    int lowerBound;
    int upperBound;
};

struct Procedure {
    int id;
    std::string name;
};

class SemanticAnalyzer {
private:
    int next_procedure_id = 0;
    int size = 1;
    std::map<int, std::vector<Variable>> procedure_variables;
    std::map<int, Procedure> procedures;
    std::vector<Variable> temporary_variables;

public:
    void addTemporaryVariable(const std::string& name, Type type, int lowerBound = 0, int upperBound = 0);
    void updateTemporaryVariables(int procedure_id);
    void addVariable(int procedure_id, const std::string& name, Type type, int lowerBound = 0, int upperBound = 0);
    const Variable& getVariable(int procedure_id, const std::string& name) const;
    void addProcedure(const std::string& name);
    const Procedure& getProcedure(int procedure_id) const;
    int getProcedureID(const std::string& name) const;
    void clear();
    void printDebugInfo() const;
};

#endif
