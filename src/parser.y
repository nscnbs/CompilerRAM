%code requires {
    #include "types.hpp"
}
%{
#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <variant>
#include <vector>
#include "manager.hpp"
#include "instructions.hpp"

int yylex();
extern int yylineno;
bool err = false;

void yyerror(const char* s) {
    fprintf(stderr, "[Error] %s at line %d\n", s, yylineno);
    err = true;
}

Manager manager;
GenerateInstruction generator;
Types ident;
Types value;
Types value2;
Types lower;
Types upper;
std::string nameProc;
std::string nameArr;
std::string nameVar;
std::string number;
std::string arg;
std::string decl;
std::string iter;
std::string low;
std::string up;
std::string idx;
std::vector<Instruction> procCode;
std::vector<Instruction> mainCode;
std::vector<Instruction> com;
std::vector<Instruction> com2;
std::vector<Instruction> cond;
std::vector<Instruction> exprs;
std::vector<Instruction> rtrn;
using YYSTYPE = std::variant<std::string, std::vector<Instruction>, Types>;
%}

%define api.value.type {std::variant<std::string, std::vector<Instruction>, Types>}

%token PIDENTIFIER
%token NUM
%token PROGRAM
%token PROCEDURE
%token BGN
%token END
%token IS
%token IF
%token THEN
%token ELSE
%token ENDIF
%token WHILE
%token DO
%token ENDWHILE
%token REPEAT
%token UNTIL
%token FOR
%token FROM
%token TO
%token DOWNTO
%token ENDFOR
%token READ
%token WRITE
%token T
%token PLUS
%token MINUS
%token MLTY
%token DIV
%token MOD
%token ASSIGN
%token EQ
%token NEQ
%token LT
%token GT
%token LE
%token GE
%token LBR
%token RBR
%token LSQBR
%token RSQBR
%token COMMA
%token SEMICOLON
%token COLON
%token ERROR

%start program

%%
program:
    procedures main {
        procCode = std::get<std::vector<Instruction>>($1);
        mainCode = std::get<std::vector<Instruction>>($2);
        manager.checkInit();
        generator.generateCode(procCode, mainCode);
    }
    ;

procedures:
    procedures PROCEDURE proc_head IS declarations BGN commands END {
        com = std::get<std::vector<Instruction>>($7);
        nameProc = std::get<std::string>($3);
        manager.addProc(nameProc);
        rtrn = generator.connectProc(nameProc, com);        
        $$ = rtrn;
    }
    | procedures PROCEDURE proc_head IS BGN commands END {
        com = std::get<std::vector<Instruction>>($6);
        nameProc = std::get<std::string>($3);
        manager.addProc(nameProc);
        rtrn = generator.connectProc(nameProc, com);
        $$ = rtrn;
    }
    | { $$ = std::vector<Instruction>(); }
    ;

main:
    PROGRAM IS declarations BGN commands END {
        com = std::get<std::vector<Instruction>>($5);
        nameProc = "main";
        manager.addProc(nameProc);
        rtrn = generator.connectMain(com);
        $$ = rtrn;
    }
    | PROGRAM IS BGN commands END {
        com = std::get<std::vector<Instruction>>($4);
        nameProc = "main";
        manager.addProc(nameProc);
        rtrn = generator.connectMain(com);
        $$ = rtrn;
    }
    ;

commands:
    commands command {
        com = std::get<std::vector<Instruction>>($1);
        com2 = std::get<std::vector<Instruction>>($2);
        com.insert(com.end(), com2.begin(), com2.end());
        $$ = com;
    }
    | command { $$ = $1; }
    ;

command:
    identifier ASSIGN expression SEMICOLON {
        ident = std::get<Types>($1);
        exprs = std::get<std::vector<Instruction>>($3);
        manager.initVar(ident);
        rtrn = generator.connectAssign(ident, exprs);
        $$ = rtrn;
    }
    | IF condition THEN commands ELSE commands ENDIF {
        cond = std::get<std::vector<Instruction>>($2);
        com = std::get<std::vector<Instruction>>($4);
        com2 = std::get<std::vector<Instruction>>($6);
        rtrn = generator.connectIfElse(cond, com, com2);
        $$ = rtrn;
    }
    | IF condition THEN commands ENDIF {
        cond = std::get<std::vector<Instruction>>($2);
        com = std::get<std::vector<Instruction>>($4);
        rtrn = generator.connectIf(cond, com);
        $$ = rtrn;
    }
    | WHILE condition DO commands ENDWHILE {
        cond = std::get<std::vector<Instruction>>($2);
        com = std::get<std::vector<Instruction>>($4);
        rtrn = generator.connectWhile(cond, com);
        $$ = rtrn;
    }
    | REPEAT commands UNTIL condition SEMICOLON {
        cond = std::get<std::vector<Instruction>>($4);
        com = std::get<std::vector<Instruction>>($2);
        rtrn = generator.connectRepeat(com, cond);
        $$ = rtrn;
    }
    | FOR PIDENTIFIER FROM value TO value DO commands ENDFOR {
        iter = std::get<std::string>($2);
        lower = std::get<Types>($4);
        upper = std::get<Types>($6);
        com = std::get<std::vector<Instruction>>($8);
        rtrn = generator.connectFor(iter, lower, upper, com);
        $$ = rtrn;
    }
    | FOR PIDENTIFIER FROM value DOWNTO value DO commands ENDFOR {
        iter = std::get<std::string>($2);
        lower = std::get<Types>($4);
        upper = std::get<Types>($6);
        com = std::get<std::vector<Instruction>>($8);
        rtrn = generator.connectForDown(iter, lower, upper, com);
        $$ = rtrn;
    }
    | proc_call SEMICOLON {
        nameProc = std::get<std::string>($1);
        rtrn = generator.connectCall(nameProc);
        $$ = rtrn;
    }
    | READ identifier SEMICOLON {
        ident = std::get<Types>($2);
        rtrn = generator.connectRead(ident);
        $$ = rtrn;
    }
    | WRITE value SEMICOLON {
        value = std::get<Types>($2);
        rtrn = generator.connectWrite(value);
        $$ = rtrn;
    }
    ;

proc_head:
    PIDENTIFIER LBR args_decl RBR {
        nameProc = std::get<std::string>($1);
        manager.initProc(nameProc);
        $$ = $1;
    }
    ;

proc_call:
    PIDENTIFIER LBR args RBR {
        nameProc = std::get<std::string>($1);
        manager.checkProcCall(nameProc);
        $$ = $1;
    }
    ;

declarations:
    declarations COMMA PIDENTIFIER {
        decl = std::get<std::string>($3);
        manager.bufferVar(decl);
    }
    | declarations COMMA PIDENTIFIER LSQBR NUM COLON NUM RSQBR {
        decl = std::get<std::string>($3);
        low = std::get<std::string>($5);
        up = std::get<std::string>($7);
        manager.bufferArr(decl, low, up);
    }
    | PIDENTIFIER {
        decl = std::get<std::string>($1);
        manager.bufferVar(decl);
    }
    | PIDENTIFIER LSQBR NUM COLON NUM RSQBR {
        decl = std::get<std::string>($1);
        low = std::get<std::string>($3);
        up = std::get<std::string>($5);
        manager.bufferArr(decl, low, up);
    }
    ;

args_decl:
    args_decl COMMA PIDENTIFIER {
        arg = std::get<std::string>($3);
        manager.bufferArg(arg, Type::val);
    }
    | args_decl COMMA T PIDENTIFIER {
        arg = std::get<std::string>($4);
        manager.bufferArg(arg, Type::arr);
    }
    | PIDENTIFIER {
        arg = std::get<std::string>($1);
        manager.bufferArg(arg, Type::val);
    }
    | T PIDENTIFIER {
        arg = std::get<std::string>($2);
        manager.bufferArg(arg, Type::arr);
    }
    ;

args:
    args COMMA PIDENTIFIER {
        arg = std::get<std::string>($3);
        manager.bufferProcCall(arg);
    }
    | PIDENTIFIER {
        arg = std::get<std::string>($1);
        manager.bufferProcCall(arg);
    }
    ;

expression:
    value {
        value = std::get<Types>($1);
        rtrn = generator.connectVal(value);
        $$ = rtrn;
    }
    | value PLUS value {
        value = std::get<Types>($1);
        value2 = std::get<Types>($3);
        rtrn = generator.connectPlus(value, value2);
        $$ = rtrn;
    }
    | value MINUS value {
        value = std::get<Types>($1);
        value2 = std::get<Types>($3);
        rtrn = generator.connectMinus(value, value2);
        $$ = rtrn;
    }
    | value MLTY value {
        value = std::get<Types>($1);
        value2 = std::get<Types>($3);
        rtrn = generator.connectMult(value, value2);
        $$ = rtrn;
    }
    | value DIV value {
        value = std::get<Types>($1);
        value2 = std::get<Types>($3);
        rtrn = generator.connectDiv(value, value2);
        $$ = rtrn;
    }
    | value MOD value {
        value = std::get<Types>($1);
        value2 = std::get<Types>($3);
        rtrn = generator.connectMod(value, value2);
        $$ = rtrn;
    }
    ;

condition:
    value EQ value {
        value = std::get<Types>($1);
        value2 = std::get<Types>($3);
        rtrn = generator.connectEq(value, value2);
        $$ = rtrn;
    }
    | value NEQ value {
        value = std::get<Types>($1);
        value2 = std::get<Types>($3);
        rtrn = generator.connectNeq(value, value2);
        $$ = rtrn; 
    }
    | value GT value {
        value = std::get<Types>($1);
        value2 = std::get<Types>($3);
        rtrn = generator.connectGt(value, value2);
        $$ = rtrn; 
    }
    | value LT value {
        value = std::get<Types>($1);
        value2 = std::get<Types>($3);
        rtrn = generator.connectLt(value, value2);
        $$ = rtrn;   
    }
    | value GE value {
        value = std::get<Types>($1);
        value2 = std::get<Types>($3);
        rtrn = generator.connectGte(value, value2);
        $$ = rtrn;    
    }
    | value LE value {
        value = std::get<Types>($1);
        value2 = std::get<Types>($3);
        rtrn = generator.connectLte(value, value2);
        $$ = rtrn;
    }
    ;

value:
    NUM {
        number = std::get<std::string>($1);
        $$ = Types{number, Type::num};
    }
    | identifier { $$ = $1; }
    ;

identifier:
    PIDENTIFIER {
        nameVar = std::get<std::string>($1);
        manager.checkVarUsage(nameVar, false);
        $$ = Types{nameVar, Type::val};
    }
    | PIDENTIFIER LSQBR PIDENTIFIER RSQBR {
        nameArr = std::get<std::string>($1);
        idx = std::get<std::string>($3);
        manager.checkVarUsage(nameArr, true);
        $$ = Types{nameArr, Type::arr, idx, Type::val};
    }
    | PIDENTIFIER LSQBR NUM RSQBR {
        nameArr = std::get<std::string>($1);
        idx = std::get<std::string>($3);
        manager.checkVarUsage(nameArr, true);
        $$ = Types{nameArr, Type::arr, idx, Type::num};
    }
    ;

%%
