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

"int"                           {flexout("TYPE","int");}
"float"                         {flexout("TYPE","float");}
"char"                          {flexout("TYPE","char");}
"struct"                        {flexout("STRUCT",yytext);}
"return"                        {flexout("RETURN",yytext);}
"if"                            {flexout("IF",yytext);}
"else"                          {flexout("ELSE",yytext);}
"while"                         {flexout("WHILE",yytext);}

";"                             {flexout("SEMI",yytext);}
","                             {flexout("COMMA",yytext);}
"=="                            {flexout("RELOP",yytext);}
"="                             {flexout("ASSIGNOP",yytext);}
">="                            {flexout("RELOP",yytext);}
">"                             {flexout("RELOP",yytext);}
"<="                            {flexout("RELOP",yytext);}
"<"                             {flexout("RELOP",yytext);}   
"!="                            {flexout("RELOP",yytext);}
"+"                             {flexout("PLUS",yytext);}
"-"                             {flexout("MINUS",yytext);}
"*"                             {flexout("STAR",yytext);}
"/"                             {flexout("DIV",yytext);}
"&&"                            {flexout("AND",yytext);}
"||"                            {flexout("OR",yytext);}
"."                             {flexout("DOT",yytext);}
"!"                             {flexout("NOT",yytext);}

"("                             {flexout("LP",yytext);}
")"                             {flexout("RP",yytext);}
"["                             {flexout("LB",yytext);}
"]"                             {flexout("RB",yytext);}
"{"                             {flexout("LC",yytext);}
"}"                             {flexout("RC",yytext);}

-?[1-9][0-9]*|0                 {flexout("INT",yytext);/*十进制整数*/}
-?[0-7]+                        {flexout("INT",yytext);/*八进制整数*/}
0[xX][0-9a-fA-F]+               {flexout("INT",yytext);/*十六进制整数*/}

-?([1-9][0-9]*|0)\.[0-9]*       {flexout("FLOAT",yytext);}
-?([1-9][0-9]*|0)?\.[0-9]*[eE][+-]?[0-9]+ {flexout("FLOAT",yytext);}
[a-zA-Z][a-zA-Z0-9]*            {flexout("ID",yytext);}

[\n]		                    {yycolumn=1;}
[ \r\t]		                    {/*printf("过滤空格等字符\n");*/}

.                               {printf("Error type A at Line %d: Mysterious characters '%c'\n",yylineno,yytext[0]);}
-?0[0-9]*                       {printf("Error type A at Line %d: Illegal octal number '%s'\n",yylineno,yytext);}
-?0[xX][a-zA-Z0-9]*             {printf("Error type A at Line %d: Illegal hexadecimal number '%s'\n",yylineno,yytext);}

%%

int yywrap()
{
    return 1;
}