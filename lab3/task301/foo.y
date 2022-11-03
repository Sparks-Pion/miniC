 //这个例子中有一些问题，请调试修改
%union
{
	int ival;
	char *sval;
} //**begin***//
%token <ival>NUM
%token <sval>STR
%nterm <ival>exp
%nterm <sval>useless
%left '+'
%%
exp:
	exp '+' exp
	|exp '-' exp
	|NUM
	;
useless: STR;
%% //**end**//