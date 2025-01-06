#include <iostream>
#include <cstdio>
#include "parser.hpp"

using namespace std;

extern int yynerrs; // liczba błędów
extern int yylineno; // numer linii
extern FILE* yyin;
extern int yyparse();

int main(int argc, char **argv) {
    if (argc < 2) {
        cerr << "Użycie: " << argv[0] << " <plik_wejściowy>" << endl;
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        cerr << "Nie można otworzyć pliku: " << argv[1] << endl;
        return 1;
    }

    yyin = file;
    yyparse();
    fclose(file);

    cout << "Analiza zakończona. Liczba błędów: " << yynerrs << endl;

    return 0;
}
