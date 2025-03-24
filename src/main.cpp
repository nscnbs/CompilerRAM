#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <memory>
#include <variant>
#include <vector>
#include "parse.hpp"
#include "instructions.hpp"
#include "types.hpp"

extern int err;
extern int yylineno;
extern FILE* yyin;
extern int yyparse();

extern GenerateInstruction generator;
void writeInstructions(const std::string &outputFile);

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "[Error] usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    auto fileDeleter = [](FILE* f) { if (f) fclose(f); };
    std::unique_ptr<FILE, decltype(fileDeleter)> inputFile(fopen(argv[1], "r"), fileDeleter);
    if (!inputFile) {
        std::cerr << "[Error] cannot read the file: " << argv[1] << std::endl;
        return 1;
    }
    std::cout << "[Running] processing file: '" << argv[1] << "'" << std::endl;

    yyin = inputFile.get();
    yyparse();

    writeInstructions(argv[2]);

    if(err){
        std::cout << "[Done] exited with code=-1" << std::endl;
        return 0;
    }
    else{
        std::cout << "[Done] exited with code=0" << std::endl;
        return 0;
    }
}

void writeInstructions(const std::string &outputFile) {
    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cerr << "[Error] cannot open the output file: " << outputFile << std::endl;
        return;
    }
    const std::vector<Instruction>& code = generator.getCode();
    for (const auto& ins : code) {
        out << ins.op << " " << ins.arg << std::endl;
    }
    std::cout << "[Success] written to file: '" << outputFile << "'" << std::endl;
}
