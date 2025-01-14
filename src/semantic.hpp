#ifndef SEMANTIC_HPP
#define SEMANTIC_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <stack>
#include "logger.hpp"

enum Type {
    Var, Arr
};

struct Variable {
    int id;
    std::string name;
    Type type;
    int lowerBound;
    int upperBound;
};

struct Procedure {
    int id;
    std::string name;
    std::vector<Type> argument_types;
};

struct Instruction {
    std::string instruction;
    std::string operand;
};

struct Registry {
    int address;
    std::string name;
    int value;
};

class SemanticAnalyzer {
private:
    Logger logger{"src/kompilator.log"};
    int next_procedure_id = 0;
    int next_addr = 1;
    int size = 1;
    std::map<int, std::vector<Variable>> procedure_variables;
    std::map<int, std::vector<Variable>> procedure_args;
    std::map<int, Procedure> procedures;
    std::map<int, Registry> registry;
    std::vector<Variable> temporary_variables;
    std::vector<std::string> proc_call_stack;
    std::vector<Instruction> instructions;

public:
    std::vector<Type> temporary_args;
    // Variables
    void addTemporaryVariable(const std::string& name, Type type, int lowerBound = 0, int upperBound = 0);
    void updateTemporaryVariables(int procedure_id);
    void addVariable(int procedure_id, const std::string& name, Type type, int lowerBound = 0, int upperBound = 0);
    const Variable& getVariable(int procedure_id, const std::string& name) const;
    void checkArrayIndex(int proc_id, std::string& name, int index) const;
    // Procedures
    int createProcedure();
    std::string setNameProcedure(const std::string& name);
    void addProcedure(int procedure_id, const std::string& name);
    const Procedure& getProcedure(int procedure_id) const;
    int getProcedureID(const std::string& name) const;
    void validateProcedureParameter(const std::string& name) const;
    void checkProcedureParameters(int proc_call_id, const std::string& proc_name, const std::vector<std::string>& args) const;
    void checkProcedureCall(const std::string& name);
    void endProcedureCall();
    void validateProcedureCall(int proc_call_id, const std::string& proc_name, const std::vector<std::string>& args);
    // Instructions
    void addInstruction(const std::string& instruction, std::string operand = "");
    const std::vector<Instruction>& getInstructions() const;
    std::string createLabel(const std::string& label);
    void setLabel(const std::string& label);
    void clearInstructions();
    // Registry
    int registryAdd(const std::string& name, int val);
    void registryAddArray(const std::string& name, int lower_bound, int upper_bound);
    // Utils
    void clear();
    void printDebugInfo();
};

#endif
