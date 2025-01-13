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

int current_procedure_id;
std::string current_procedure_name;

std::vector<std::string> arg_list;

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
        semantic.addProcedure(std::get<int>($5), std::get<std::string>($3));        
    }
    | procedures PROCEDURE proc_head IS P_BEGIN commands END {
        current_procedure_id = semantic.createProcedure();
        semantic.addProcedure(current_procedure_id , std::get<std::string>($3));
    }
    | /* epsilon */
    ;

main:
    PROGRAM IS declarations P_BEGIN commands END {
        semantic.addProcedure(std::get<int>($3), "main");
    }
    | PROGRAM IS P_BEGIN commands END {
        current_procedure_id = semantic.createProcedure();
        semantic.addProcedure(current_procedure_id, "main");
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
        current_procedure_name = semantic.setNameProcedure(std::get<std::string>($1))
        $$ = $1;
    }
    ;

proc_call:
    PIDENTIFIER LBR args RBR {
        semantic.checkProcedureCall(current_procedure_name);
        semantic.validateProcedureCall(current_procedure_id, std::get<std::string>($1), arg_list);
        semantic.endProcedureCall();
        $$ = $1;
    }
    ;

declarations:
    declarations COMMA PIDENTIFIER {
        semantic.addVariable(std::get<int>($1), std::get<std::string>($3), Type::Var);
        $$ = $1;
    }
    | declarations COMMA PIDENTIFIER LSQBR NUM COLON NUM RSQBR { 
        semantic.addVariable(std::get<int>($1), std::get<std::string>($3), Type::Arr, std::get<int>($5), std::get<int>($7));
        $$ = $1;
    }
    | PIDENTIFIER {
        current_procedure_id = semantic.createProcedure();
        semantic.addVariable(current_procedure_id, std::get<std::string>($1), Type::Var);
        $$ = current_procedure_id;
    }
    | PIDENTIFIER LSQBR NUM COLON NUM RSQBR {
        current_procedure_id = semantic.createProcedure();
        semantic.addVariable(current_procedure_id, std::get<std::string>($1), Type::Arr, std::get<int>($3), std::get<int>($5));
        $$ = current_procedure_id;
    }
    ;

args_decl:
    args_decl COMMA PIDENTIFIER {
        semantic.addTemporaryVariable(std::get<std::string>($3), Type::Var);
    }
    | args_decl COMMA T PIDENTIFIER { 
        semantic.addTemporaryVariable(std::get<std::string>($4), Type::Arr);
    }
    | PIDENTIFIER {
        semantic.addTemporaryVariable(std::get<std::string>($1), Type::Var);
    }
    | T PIDENTIFIER {
        semantic.addTemporaryVariable(std::get<std::string>($2), Type::Arr);
    }
    ;

args:
    args COMMA PIDENTIFIER {
        std::cout << " args COMMA PIDENTIFIER " << std::get<std::string>($3) << std::endl;
        arg_list.push_back(std::get<std::string>($3));
    }
    | PIDENTIFIER {
        std::cout << " PIDENTIFIER " << std::get<std::string>($1) << std::endl;
        arg_list.clear();
        arg_list.push_back(std::get<std::string>($1));
    }
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
    | PIDENTIFIER LSQBR PIDENTIFIER RSQBR {
        $$ = $1;
    }
    | PIDENTIFIER LSQBR NUM RSQBR {
        semantic.checkArrayIndex(current_procedure_id, std::get<std::string>($1), std::get<int>($3));
        $$ = $1;
    }

%%
void yyerror(const char* s) {
    std::cerr << "Syntax error: " << s << " at line " << yylineno << std::endl;
    exit(EXIT_FAILURE);
}
