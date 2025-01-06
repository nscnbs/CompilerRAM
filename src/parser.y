%{
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "utils.hpp"

using namespace std;

extern int yylineno;
int yylex( void );
void yyerror(const char *s);
%}

%union {
    int intVal;
    char* strVal;
}

%token <intVal> NUM
%token <strVal> PIDENTIFIER

%token PLUS MINUS MULT DIV MOD ASSIGN
%token EQ NEQ LT GT LTE GTE
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
    procedures main
    ;

procedures:
    procedures PROCEDURE proc_head IS declarations P_BEGIN commands END
    | procedures PROCEDURE proc_head IS P_BEGIN commands END
    | /* empty */
    ;

main:
    PROGRAM IS declarations P_BEGIN commands END
    | PROGRAM IS P_BEGIN commands END
    ;

commands:
    commands command
    | command
    ;

command:
    identifier ASSIGN expression SEMICOLON
    | IF condition THEN commands ELSE commands ENDIF
    | IF condition THEN commands ENDIF
    | WHILE condition DO commands ENDWHILE
    | REPEAT commands UNTIL condition SEMICOLON
    | FOR PIDENTIFIER FROM value TO value DO commands ENDFOR
    | FOR PIDENTIFIER FROM value DOWNTO value DO commands ENDFOR
    | proc_call SEMICOLON
    | READ identifier SEMICOLON
    | WRITE value SEMICOLON
    ;

proc_head:
    PIDENTIFIER LBR args_decl RBR
    ;

proc_call:
    PIDENTIFIER LBR args RBR
    ;

declarations:
    declarations COMMA PIDENTIFIER
    | declarations COMMA PIDENTIFIER LSQBR NUM COLON NUM RSQBR
    | PIDENTIFIER
    | PIDENTIFIER LSQBR NUM COLON NUM RSQBR
    ;

args_decl:
    args_decl COMMA PIDENTIFIER
    | args_decl COMMA T PIDENTIFIER
    | PIDENTIFIER
    | T PIDENTIFIER
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
    NUM
    | identifier
    ;

identifier:
    PIDENTIFIER
    | PIDENTIFIER LSQBR PIDENTIFIER RSQBR
    | PIDENTIFIER LSQBR NUM RSQBR

%%
void yyerror(const char* s) {
    fprintf(stderr, "Błąd: %s w linii %d\n", s, yylineno);
}
