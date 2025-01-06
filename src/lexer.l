%option noyywrap
%option yylineno
%option nounput
%{
#include "parser.hpp"
#include "utils.hpp"

int yylex();
%}

DIGIT       [0-9]
LETTER      [a-z_]

NUM         {DIGIT}+
ID          {LETTER}+

%%
\#.*\n          ; // Komentarze
[ \t\r]+        ; // Ignorowanie białych znaków
\n              ;

{NUM}           { yylval.intVal = atoi(yytext); return NUM; }
{ID}            { yylval.strVal = strdup(yytext); return PIDENTIFIER; }

"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MULT; }
"/"             { return DIV; }
"%"             { return MOD; }
":="            { return ASSIGN; }

"="             { return EQ; }
"!="            { return NEQ; }
"<"             { return LT; }
">"             { return GT; }
"<="            { return LTE; }
">="            { return GTE; }

"("             { return LBR; }
")"             { return RBR; }
"["             { return LSQBR; }
"]"             { return RSQBR; }
","             { return COMMA; }
":"             { return COLON; }
";"             { return SEMICOLON; }

"PROGRAM"           { return PROGRAM; }
"PROCEDURE"         { return PROCEDURE; }
"BEGIN"             { return P_BEGIN; }
"END"               { return END; }
"IS"                { return IS; }
"IF"                { return IF; }
"THEN"              { return THEN; }
"ELSE"              { return ELSE; }
"ENDIF"             { return ENDIF; }
"WHILE"             { return WHILE; }
"DO"                { return DO; }
"ENDWHILE"          { return ENDWHILE; }
"REPEAT"            { return REPEAT; }
"UNTIL"             { return UNTIL; }
"FOR"               { return FOR; }
"FROM"              { return FROM; }
"TO"                { return TO; }
"DOWNTO"            { return DOWNTO; }
"ENDFOR"            { return ENDFOR; }
"READ"              { return READ; }
"WRITE"             { return WRITE; }
"T"                 { return T; }

.                   { fprintf(stderr, "Nieznany symbol: %s w linii %d\n", yytext, yylineno); return ERROR; }
%%
