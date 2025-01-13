#include "semantic.hpp"

// Variables
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
    std::cout << "Dodano var: " << name << " do procedury: " << procedure_id << std::endl;
}

const Variable& SemanticAnalyzer::getVariable(int procedure_id, const std::string& name) const {
    if (!procedure_variables.count(procedure_id)) {
        throw std::runtime_error("Error: Procedure ID not found.");
    }

    for (const auto& var : temporary_variables) {
        if (var.name == name) {
            std::cout << "Get variable: " << var.name << " " << var.type << " " << var.lowerBound << " " << var.upperBound << std::endl;
            return var;
        }
    }

    const auto& variables = procedure_variables.at(procedure_id);
    for (const auto& var : variables) {
        if (var.name == name) {
            std::cout << "Get variable: " << var.name << " " << var.type << " " << var.lowerBound << " " << var.upperBound << std::endl;
            return var;
        }
    }
    throw std::runtime_error("Error: Variable '" + name + "' not found in procedure ID: " + std::to_string(procedure_id));
}

void SemanticAnalyzer::addTemporaryVariable(const std::string& name, Type type, int lowerBound, int upperBound) {
    if (type == Type::Arr && (lowerBound > upperBound)) {
        throw std::runtime_error("Error: Invalid bounds for array '" + name + "'");
    }

    temporary_variables.push_back({name, type, lowerBound, upperBound});
    std::cout << "Dodano time var: " << name << std::endl;
}

void SemanticAnalyzer::updateTemporaryVariables(int procedure_id) {
    auto& args = procedure_args[procedure_id];
    for (const auto& var : temporary_variables) {
        args.push_back({var.name, var.type, var.lowerBound, var.upperBound});
        addVariable(procedure_id, var.name, var.type, var.lowerBound, var.upperBound);
    }
    std::cout << "Dodano all time var do procedury: " << procedure_id << std::endl;
    temporary_variables.clear();
}

void SemanticAnalyzer::checkArrayIndex(int proc_id, std::string& name, int index) const {
    const auto& array = getVariable(proc_id, name);
    if (index < array.lowerBound || index > array.upperBound) {
        throw std::runtime_error("Error: Array index out of bounds for '" + array.name + "'");
    }
}

// Procedures
int SemanticAnalyzer::createProcedure() {
    int procedure_id = next_procedure_id++;
    procedures[procedure_id] = {procedure_id, ""};
    std::cout << "Utworzono id procedury: " << procedure_id << " name: " << procedures[procedure_id].name << std::endl;
    return procedure_id;
}

std::string SemanticAnalyzer::setNameProcedure(const std::string& name) {
    proc_call_stack.push_back(name);
    return name;
}    
 
void SemanticAnalyzer::addProcedure(int procedure_id, const std::string& name) {
    for (const auto& proc : procedures) {
        if (proc.second.name == name) {
            throw std::runtime_error("Error: Procedure '" + name + "' already declared.");
        }
    }

    std::vector<Type> arg_types;
    for (const auto& var : temporary_variables) {
        arg_types.push_back(var.type);
    }
    for (const auto& var : arg_types) {
        std::cout << var << std::endl;
    }
    procedures[procedure_id] = {procedure_id, name, arg_types};
    std::cout << "Zaktualizowano name "<< procedures[procedure_id].name << " procedury: " << procedures[procedure_id].id << std::endl;
    updateTemporaryVariables(procedure_id);
    proc_call_stack.clear();
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

void SemanticAnalyzer::validateProcedureParameter(const std::string& name) const {
    int procedure_id = getProcedureID(name);
    
    for (const auto& var : temporary_variables) {
        if (var.name == name) {
            return;
        }
    }
    
    const auto& variables = procedure_variables.at(procedure_id);
    for (const auto& var : variables) {
        if (var.name == name) {
            return;
        }
    }
    throw std::runtime_error("Error: Undefined parameter '" + name + "'");
}

void SemanticAnalyzer::checkProcedureParameters(int proc_call_id, const std::string& proc_name, const std::vector<std::string>& args) const {
    int procedure_id = getProcedureID(proc_name);
    const auto& proc = procedures.at(procedure_id);

    if (proc.argument_types.size() != args.size()) {
        throw std::runtime_error("Error: Mismatch in the number of arguments for procedure '" + proc_name + "'");
    }
    
    for (size_t i = 0; i < args.size(); ++i) {
        const auto& arg_name = args[i];
        const auto& var = getVariable(proc_call_id, arg_name);

        if (proc.argument_types[i] != var.type) {
            throw std::runtime_error("Error: Argument type mismatch for procedure '" + proc_name + "'. Expected " +
                                     std::to_string(proc.argument_types[i]) + " but got " +
                                     std::to_string(var.type));
        }
    }
}

void SemanticAnalyzer::checkProcedureCall(const std::string& curr_name) {
    for (std::string name : proc_call_stack) {
        if (name == curr_name) {
            throw std::runtime_error("Error: Recursive call to procedure '" + name + "' is not allowed.");
        }
    }

    proc_call_stack.push_back(curr_name);
}

void SemanticAnalyzer::endProcedureCall() {
    if (!proc_call_stack.empty()) {
        proc_call_stack.pop_back();
    }
}

void SemanticAnalyzer::validateProcedureCall(int proc_call_id, const std::string& proc_name, const std::vector<std::string>& args) {
    checkProcedureParameters(proc_call_id, proc_name, args);
}

// Utils
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
