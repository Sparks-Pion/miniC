%{
#include "string.h"
#include "def.h"
#include "parser.hpp"
extern int yycolumn;

#define YYSTYPE yylval
extern "C"
{	
	int yywrap(void);
	int yylex(void);
}

%}
%option yylineno 

%%

int    [1-9][0-9]*
char \'[^'\\]\'

%%
{int}        {yylval.ival=atoi(yytext); return NUM;}
{char}      {yylval.sval=yytext[1]; return STR;/*这种情况下，正则需要简便处理*/}
%%

int yywrap()
{
return 1;
}
