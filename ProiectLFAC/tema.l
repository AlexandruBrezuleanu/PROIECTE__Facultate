%{
#include <stdio.h>
#include "y.tab.h"
%}
%option noyywrap
%%
"int"|"float"|"char"|"string"|"bool" {yylval.strval=strdup(yytext);return TIP;}
"begin_progr" {return PROGSTART;}
"end_progr" {return PROGEND;}
"begin_variables" {return VARSTART;}
"end_variables" {return VAREND;}
"begin_functions" {return FCTSTART;}
"end_functions" {return FCTEND;}
"<=" {return LE;}
">=" {return GE;}
"==" {return EQ;}
"!=" {return NE;}
"if" {return IF;}
"else" {return ELSE;}
"while" {return WHILE;}
"for" {return FOR;}
"do" {return DO;}
"then" {return THEN;}
"Print" {return PRINT;}
"&&" {return AND;}
"||" {return OR;}
"struct" {return STRUCT;}
"const" {return CONST;}
"'"[a-zA-Z0-9 :=%!@+-\/]+"'" {yylval.strval=strdup(yytext); return outstring;}
"true" {yylval.boolval=1; return TRUE;}
"false" {yylval.boolval=0; return FALSE;}
[0-9]+ {yylval.intval=atoi(yytext); return NR;}
[0-9]+"."[0-9]+ {yylval.floatval=atof(yytext); return FLOAT;}
[a-zA-Z]+ {yylval.strval=strdup(yytext); return string;}
@[_a-zA-Z][_a-zA-Z0-9]* {yylval.strval=strdup(yytext);return ID;}
#[_a-zA-Z][_a-zA-Z0-9]* {yylval.strval=strdup(yytext);return IDS;}
"=" {return ASSIGN;}
[ \t] ;
\n {yylineno++;}
. {return yytext[0];}

