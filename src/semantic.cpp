#include "semantic.hpp"

void SemanticAnalyzer::addVariable(int procedure_id, const std::string& name, Type type , int lowerBound, int upperBound) {
    auto& variables = procedure_variables[procedure_id];

    for (const auto& var : variables) {
        if (var.name == name) {
            throw std::runtime_error("Error: Variable '" + name + "' already declared in procedure ID: " + std::to_string(procedure_id));
        }
    }

    if (Type::Arr && lowerBound > upperBound) {
        throw std::runtime_error("Error: Invalid array bounds for '" + name + "'");
    }

    variables.push_back({name, type, lowerBound, upperBound});
}

const Variable& SemanticAnalyzer::getVariable(int procedure_id, const std::string& name) const {
    const auto& variables = procedure_variables.at(procedure_id);
    for (const auto& var : variables) {
        if (var.name == name) {
            return var;
        }
    }
    throw std::runtime_error("Error: Variable '" + name + "' not found in procedure ID: " + std::to_string(procedure_id));
}

void SemanticAnalyzer::addTemporaryVariable(const std::string& name, Type type, int lowerBound, int upperBound) {
    temporary_variables.push_back({name, type, lowerBound, upperBound});
}

void SemanticAnalyzer::updateTemporaryVariables(int procedure_id) {
    for (const auto& var : temporary_variables) {
        addVariable(procedure_id, var.name, var.type, var.lowerBound, var.upperBound);
    }
    temporary_variables.clear();
}

void SemanticAnalyzer::addProcedure(const std::string& name) {
    for (const auto& proc : procedures) {
        if (proc.second.name == name) {
            throw std::runtime_error("Error: Procedure '" + name + "' already declared.");
        }
    }

    int procedure_id = next_procedure_id++;
    procedures[procedure_id] = {procedure_id, name};
    updateTemporaryVariables(procedure_id);
}

const Procedure& SemanticAnalyzer::getProcedure(int procedure_id) const {
    if (!procedures.count(procedure_id)) {
        throw std::runtime_error("Error: Procedure ID " + std::to_string(procedure_id) + " not found.");
    }
    return procedures.at(procedure_id);
}

int SemanticAnalyzer::getProcedureID(const std::string& name) const {
    for (const auto& [id, proc] : procedures) {
        if (proc.name == name) {
            return id;
        }
    }
    throw std::runtime_error("Error: Procedure '" + name + "' not found.");
}

void SemanticAnalyzer::clear() {
    procedure_variables.clear();
    procedures.clear();
    next_procedure_id = 0;
}

void SemanticAnalyzer::printDebugInfo() const {
    std::cout << "Procedures:\n";
    for (const auto& [id, proc] : procedures) {
        std::cout << "  ID: " << id << ", Name: " << proc.name << "\n";
    }

    std::cout << "Variables:\n";
    for (const auto& [proc_id, vars] : procedure_variables) {
        std::cout << "  Procedure ID: " << proc_id << "\n";
        for (const auto& var : vars) {
            std::cout << "    Name: " << var.name
                      << ", Type: " << var.type
                      << ", Bounds: [" << var.lowerBound << ", " << var.upperBound << "]\n";
        }
    }
}
