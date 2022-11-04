%{
#include "string.h"
#include "def.h"

extern int yycolumn;

#define YYSTYPE yylval
extern "C"
{	
	int yywrap(void);
	int yylex(void);
}

void flexout(std::string name,std::string value){
  std::cout<<name<<"\t:"<<value<<std::endl;
}
//flex为bison提供行号信息
%}
%option yylineno 

%%

"int"        {flexout("TYPE","int");}
"float"      {flexout("TYPE","float");}
"char"      {flexout("TYPE","char");}

\-?[1-9][0-9]*|0              {flexout("INT",yytext);}
\-?([1-9][0-9]*|0)\.[0-9]*    {flexout("FLOAT",yytext);}
[a-zA-Z][a-zA-Z0-9]*          {flexout("ID",yytext);}
";"                           {flexout("SEMI",yytext);}
","                           {flexout("COMMA",yytext);}
"="                           {flexout("ASSIGNOP",yytext);}
> | < | >= | <= | == | !=     {flexout("RELOP",yytext);}
"+"                           {flexout("PLUS",yytext);}
"-"                           {flexout("MINUS",yytext);}
"*"                           {flexout("STAR",yytext);}
"/"                           {flexout("DIV",yytext);}
"&&"                          {flexout("AND",yytext);}
"||"                          {flexout("OR",yytext);}
"."                           {flexout("DOT",yytext);}
"!"                           {flexout("NOT",yytext);}

"("                           {flexout("LP",yytext);}
")"                           {flexout("RP",yytext);}
"["                           {flexout("LB",yytext);}
"]"                           {flexout("RB",yytext);}
"{"                           {flexout("LC",yytext);}
"}"                           {flexout("RC",yytext);}
"struct"                      {flexout("STRUCT",yytext);}
"return"                      {flexout("RETURN",yytext);}
"if"                          {flexout("IF",yytext);}
"else"                        {flexout("ELSE",yytext);}
"while"                       {flexout("WHILE",yytext);}

[\n]		{yycolumn=1;}
[ \r\t]		{/*printf("过滤空格等字符\n");*/}

.		{printf("Error type A at Line %d: Mysterious characters '%c'\n",yylineno,yytext[0]);}
%%

int yywrap()
{
    return 1;
}