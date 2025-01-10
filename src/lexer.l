%option noyywrap
%option yylineno
%{
#include <iostream>
#include <cstdio>
#include <string>
#include <variant>
#include <vector>
#include "parser.tab.hpp"

int yylex();
%}

DIGIT       [0-9]
LETTER      [a-z_]

NUM         {DIGIT}+
ID          {LETTER}+

%%
\#.*\n          ; // Komentarze
[ \t\r]+        ; // Ignorowanie białych znaków
\n              ; // Ignorowanie nowej linii

{NUM}           { yylval = std::stoi(yytext); return NUM; }
{ID}            { yylval = std::string(yytext); return PIDENTIFIER; }

"+"             { yylval = std::string(yytext); return PLUS; }
"-"             { yylval = std::string(yytext); return MINUS; }
"*"             { yylval = std::string(yytext); return MULT; }
"/"             { yylval = std::string(yytext); return DIV; }
"%"             { yylval = std::string(yytext); return MOD; }

":="            { yylval = std::string(yytext); return ASSIGN; }
"="             { yylval = std::string(yytext); return EQ; }
"!="            { yylval = std::string(yytext); return NEQ; }
"<"             { yylval = std::string(yytext); return LT; }
">"             { yylval = std::string(yytext); return GT; }
"<="            { yylval = std::string(yytext); return LTE; }
">="            { yylval = std::string(yytext); return GTE; }

"("             { yylval = std::string(yytext); return LBR; }
")"             { yylval = std::string(yytext); return RBR; }
"["             { yylval = std::string(yytext); return LSQBR; }
"]"             { yylval = std::string(yytext); return RSQBR; }
","             { yylval = std::string(yytext); return COMMA; }
":"             { yylval = std::string(yytext); return COLON; }
";"             { yylval = std::string(yytext); return SEMICOLON; }

"PROGRAM"           { yylval = std::string(yytext); return PROGRAM; }
"PROCEDURE"         { yylval = std::string(yytext); return PROCEDURE; }
"BEGIN"             { yylval = std::string(yytext); return P_BEGIN; }
"END"               { yylval = std::string(yytext); return END; }
"IS"                { yylval = std::string(yytext); return IS; }
"IF"                { yylval = std::string(yytext); return IF; }
"THEN"              { yylval = std::string(yytext); return THEN; }
"ELSE"              { yylval = std::string(yytext); return ELSE; }
"ENDIF"             { yylval = std::string(yytext); return ENDIF; }
"WHILE"             { yylval = std::string(yytext); return WHILE; }
"DO"                { yylval = std::string(yytext); return DO; }
"ENDWHILE"          { yylval = std::string(yytext); return ENDWHILE; }
"REPEAT"            { yylval = std::string(yytext); return REPEAT; }
"UNTIL"             { yylval = std::string(yytext); return UNTIL; }
"FOR"               { yylval = std::string(yytext); return FOR; }
"FROM"              { yylval = std::string(yytext); return FROM; }
"TO"                { yylval = std::string(yytext); return TO; }
"DOWNTO"            { yylval = std::string(yytext); return DOWNTO; }
"ENDFOR"            { yylval = std::string(yytext); return ENDFOR; }
"READ"              { yylval = std::string(yytext); return READ; }
"WRITE"             { yylval = std::string(yytext); return WRITE; }
"T"                 { yylval = std::string(yytext); return T; }

.                   { fprintf(stderr, "Nieznany symbol: %s w linii %d\n", yytext, yylineno); return ERROR; }
%%