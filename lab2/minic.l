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


[\n]		{yycolumn=1;}
[ \r\t]		{/*printf("过滤空格等字符\n");*/}

%%

int yywrap()
{
    return 1;
}