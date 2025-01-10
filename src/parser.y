%{
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <variant>
#include <iostream>
#include <vector>
#include "semantic.hpp"

extern int yylineno;
int yylex();
void yyerror(const char *s);

SemanticAnalyzer semantic;

using YYSTYPE = std::variant<int, std::string>;
%}

%define api.value.type {std::variant<int, std::string>}

%token NUM
%token PIDENTIFIER
%token PLUS MINUS MULT DIV MOD
%token ASSIGN EQ NEQ LT GT LTE GTE
%token LBR RBR LSQBR RSQBR COMMA SEMICOLON COLON
%token PROGRAM PROCEDURE P_BEGIN END IS
%token IF THEN ELSE ENDIF
%token WHILE DO ENDWHILE
%token REPEAT UNTIL
%token FOR FROM TO DOWNTO ENDFOR
%token READ WRITE
%token T
%token ERROR

%start program

%%
program:
    procedures main {
        semantic.printDebugInfo();
        semantic.clear();
    }
    ;

procedures:
    procedures PROCEDURE proc_head IS declarations P_BEGIN commands END {
        semantic.addProcedure(std::get<std::string>($3));
    }
    | procedures PROCEDURE proc_head IS P_BEGIN commands END {
        semantic.addProcedure(std::get<std::string>($3));
    }
    | /* epsilon */
    ;

main:
    PROGRAM IS declarations P_BEGIN commands END {
        semantic.addProcedure("main");
    }
    | PROGRAM IS P_BEGIN commands END {
        semantic.addProcedure("main");
    }
    ;

commands:
    commands command
    | command {
        $$ = $1;
    }
    ;

command:
    identifier ASSIGN expression SEMICOLON
    | IF condition THEN commands ELSE commands ENDIF {
    }

    | IF condition THEN commands ENDIF {
    }
    | WHILE condition DO commands ENDWHILE
    | REPEAT commands UNTIL condition SEMICOLON
    | FOR PIDENTIFIER FROM value TO value DO commands ENDFOR
    | FOR PIDENTIFIER FROM value DOWNTO value DO commands ENDFOR
    | proc_call SEMICOLON
    | READ identifier SEMICOLON
    | WRITE value SEMICOLON {
    }

    ;

proc_head:
    PIDENTIFIER LBR args_decl RBR {
        $$ = $1;
    }
    ;

proc_call:
    PIDENTIFIER LBR args RBR {
    }
    ;

declarations:
    declarations COMMA PIDENTIFIER {
        semantic.addTemporaryVariable(std::get<std::string>($3), Type::Var);
        $$ = $1;
    }
    | declarations COMMA PIDENTIFIER LSQBR NUM COLON NUM RSQBR {
        semantic.addTemporaryVariable(std::get<std::string>($3), Type::Arr, std::get<int>($5), std::get<int>($7));
        $$ = $1;
    }
    | PIDENTIFIER {
        semantic.addTemporaryVariable(std::get<std::string>($1), Type::Var);
    }
    | PIDENTIFIER LSQBR NUM COLON NUM RSQBR {
        semantic.addTemporaryVariable(std::get<std::string>($1), Type::Arr);}
    ;

args_decl:
    args_decl COMMA PIDENTIFIER {
        semantic.addTemporaryVariable(std::get<std::string>($3), Type::Var);
        $$ = $1;
    }
    | args_decl COMMA T PIDENTIFIER { 
        semantic.addTemporaryVariable(std::get<std::string>($4), Type::Arr);
        $$ = $1;
    }
    | PIDENTIFIER {
        semantic.addTemporaryVariable(std::get<std::string>($1), Type::Var);
    }
    | T PIDENTIFIER {
        semantic.addTemporaryVariable(std::get<std::string>($2), Type::Arr);
    }
    ;

args:
    args COMMA PIDENTIFIER
    | PIDENTIFIER
    ;

expression:
    value
    | value PLUS value
    | value MINUS value
    | value MULT value
    | value DIV value
    | value MOD value
    ;

condition:
    value EQ value
    | value NEQ value
    | value LT value
    | value GT value
    | value LTE value
    | value GTE value
    ;

value:
    NUM {
        $$ = $1;
    }
    | identifier {
        $$ = $1;
    }
    ;

identifier:
    PIDENTIFIER {
        $$ = $1;
    }
    | PIDENTIFIER LSQBR PIDENTIFIER RSQBR
    | PIDENTIFIER LSQBR NUM RSQBR

%%
void yyerror(const char* s) {
    fprintf(stderr, "Błąd: %s w linii %d\n", s, yylineno);
}
