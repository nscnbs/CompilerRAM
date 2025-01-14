#include <iostream>
#include <cstdio>
#include <string>
#include <variant>
#include <vector>
#include "parser.tab.hpp"
#include "logger.hpp"

extern int yynerrs; // liczba błędów
extern int yylineno; // numer linii
extern FILE* yyin;
extern int yyparse();

Logger logger{"src/kompilator.log"};

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Użycie: " << argv[0] << " <plik_wejściowy>" << std::endl;
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        std::cerr << "Nie można otworzyć pliku: " << argv[1] << std::endl;
        return 1;
    }

    logger.log("File: " + std::string(argv[1]));
    yyin = file;
    yyparse();
    fclose(file);

    std::cout << "Analiza zakończona. Liczba błędów: " << yynerrs << std::endl;

    return 0;
}