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
std::string current_label;


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
        semantic.addInstruction("HALT", "");
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
    commands command {
    }
    | command {
        $$ = $1;
    }
    ;

command:
    identifier ASSIGN expression SEMICOLON {
        //semantic.addInstruction("SET", std::get<std::string>($3));
        semantic.addInstruction("STORE", std::get<std::string>($1));
    }
    | IF condition THEN commands ELSE commands ENDIF {
        std::string if_label = semantic.createLabel("if");
        std::string else_label = semantic.createLabel("else");
        std::string end_label = semantic.createLabel("end_if");
        
        if (std::get<std::string>($2) == "=") {
            current_label = if_label;
            semantic.setLabel(if_label);
            // ELSE
            current_label = end_label;
            semantic.addInstruction("JUMP", end_label);
            // THEN
            semantic.setLabel(end_label);
        }
        else {
            current_label = else_label;
            semantic.setLabel(else_label);
            // THEN
            current_label = semantic.createLabel("end_if");
            semantic.addInstruction("JUMP", end_label);
            // ELSE
            semantic.setLabel(end_label);
        }
    }

    | IF condition THEN commands ENDIF {
        std::string end_label = semantic.createLabel("end_if");
        // THEN
        semantic.addInstruction("JUMP", end_label);
        semantic.setLabel(end_label);
    }
    | WHILE condition DO commands ENDWHILE {
        std::string start_label = semantic.createLabel("start_while");
        std::string end_label = semantic.createLabel("end_while");
        semantic.setLabel(start_label);
        // COND
        semantic.addInstruction("JZERO", end_label);
        // BODY
        semantic.addInstruction("JUMP", start_label);
        semantic.setLabel(end_label);
    }
    | REPEAT commands UNTIL condition SEMICOLON {
        std::string start_label = semantic.createLabel("start_repeat");
        std::string end_label = semantic.createLabel("end_repeat");
        current_label = start_label;
        semantic.setLabel(start_label);
        // COND
        // BODY
        semantic.addInstruction("JUMP", start_label);
        current_label = end_label;
        semantic.setLabel(end_label);
    }
    | FOR PIDENTIFIER FROM value TO value DO commands ENDFOR
    | FOR PIDENTIFIER FROM value DOWNTO value DO commands ENDFOR
    | proc_call SEMICOLON
    | READ identifier SEMICOLON {
        semantic.addInstruction("GET", std::get<std::string>($2));
    }
    | WRITE value SEMICOLON {
        semantic.addInstruction("SET", std::get<std::string>($2));
        semantic.addInstruction("PUT", "0");
    }
    ;

proc_head:
    PIDENTIFIER LBR args_decl RBR {
        current_procedure_name = semantic.setNameProcedure(std::get<std::string>($1));
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
        semantic.addTemporaryVariable(std::get<std::string>($3), Type::Var);
        $$ = $1;
    }
    | declarations COMMA PIDENTIFIER LSQBR NUM COLON NUM RSQBR { 
        semantic.addTemporaryVariable(std::get<std::string>($3), Type::Arr, std::get<int>($5), std::get<int>($7));
        semantic.registryAddArray(std::get<std::string>($3), std::get<int>($5), std::get<int>($7));
        $$ = $1;
    }
    | PIDENTIFIER {
        current_procedure_id = semantic.createProcedure();
        semantic.addTemporaryVariable(std::get<std::string>($1), Type::Var);
        $$ = current_procedure_id;
    }
    | PIDENTIFIER LSQBR NUM COLON NUM RSQBR {
        current_procedure_id = semantic.createProcedure();
        semantic.addTemporaryVariable(std::get<std::string>($1), Type::Arr, std::get<int>($3), std::get<int>($5));
        $$ = current_procedure_id;
    }
    ;

args_decl:
    args_decl COMMA PIDENTIFIER {
        semantic.addTemporaryVariable(std::get<std::string>($3), Type::Var);
        semantic.temporary_args.push_back(Type::Var);
    }
    | args_decl COMMA T PIDENTIFIER { 
        semantic.addTemporaryVariable(std::get<std::string>($4), Type::Arr);
        semantic.temporary_args.push_back(Type::Arr);
    }
    | PIDENTIFIER {
        semantic.addTemporaryVariable(std::get<std::string>($1), Type::Var);
        semantic.temporary_args.push_back(Type::Var);
    }
    | T PIDENTIFIER {
        semantic.addTemporaryVariable(std::get<std::string>($2), Type::Arr);
        semantic.temporary_args.push_back(Type::Arr);
    }
    ;

args:
    args COMMA PIDENTIFIER {
        arg_list.push_back(std::get<std::string>($3));
    }
    | PIDENTIFIER {
        arg_list.clear();
        arg_list.push_back(std::get<std::string>($1));
    }
    ;

expression:
    value {
        $$ = $1;
    }
    | value PLUS value {
        semantic.addInstruction("LOAD", std::get<std::string>($1));
        semantic.addInstruction("ADD", std::get<std::string>($3));
    }
    | value MINUS value {
        semantic.addInstruction("LOAD", std::get<std::string>($1));
        semantic.addInstruction("SUB", std::get<std::string>($3));
    }
    | value MULT value {
        semantic.addInstruction("LOAD", std::get<std::string>($1));
        semantic.addInstruction("MULT", std::get<std::string>($3));
    }
    | value DIV value {
        semantic.addInstruction("LOAD", std::get<std::string>($1));
        semantic.addInstruction("DIV", std::get<std::string>($3));
    }
    | value MOD value {
        semantic.addInstruction("LOAD", std::get<std::string>($1));
        semantic.addInstruction("MOD", std::get<std::string>($3));
    }
    ;

condition:
    value EQ value {
        semantic.addInstruction("LOAD", std::get<std::string>($1));
        semantic.addInstruction("SUB", std::get<std::string>($3));
        semantic.addInstruction("JZERO", current_label);
        //semantic.addInstruction("JUMP", "2");
        $$ = std::get<std::string>($2);
    }
    | value NEQ value {
        semantic.addInstruction("LOAD", std::get<std::string>($1));
        semantic.addInstruction("SUB", std::get<std::string>($3));
        semantic.addInstruction("JZERO", current_label);
        //semantic.addInstruction("JUMP", "2");
        $$ = std::get<std::string>($2);
    }
    | value LT value {
        semantic.addInstruction("LOAD", std::get<std::string>($3));
        semantic.addInstruction("SUB", std::get<std::string>($1));
        semantic.addInstruction("JPOS", current_label);
        $$ = std::get<std::string>($2);
    }
    | value GT value {
        semantic.addInstruction("LOAD", std::get<std::string>($1));
        semantic.addInstruction("SUB", std::get<std::string>($3));
        semantic.addInstruction("JPOS", current_label);
        $$ = std::get<std::string>($2);
    }
    | value LTE value {
        semantic.addInstruction("LOAD", std::get<std::string>($3));
        semantic.addInstruction("SUB", std::get<std::string>($1));
        semantic.addInstruction("JNEG", current_label);
        $$ = std::get<std::string>($2);
    }
    | value GTE value {
        semantic.addInstruction("LOAD", std::get<std::string>($1));
        semantic.addInstruction("SUB", std::get<std::string>($3));
        semantic.addInstruction("JNEG", current_label);
        $$ = std::get<std::string>($2);
    }
    ;

value:
    NUM {
        $$ = std::to_string(std::get<int>($1));
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
