/* PL词法分析器 */
/* 功能：能够识别出PL支持的所有单词符号并给出种别值 */
/* 说明：在下面的begin和end之间添加代码，已经实现了标识符和整常量的识别，你需要完成剩下的部分，加油吧！ */
/* 提示：因为是顺序匹配，即从上至下依次匹配规则，所以需要合理安排顺序～ */
%{
#include <stdio.h>
%} /*** begin ****/

INTCON			[\-]?[1-9][0-9]*|0
IDENT			[A-Za-z][A-Za-z0-9]*

CHARCON         '[^\']*'


%%  
"of"            {printf("%s: OFSYM\n", yytext);}
"array"         {printf("%s: ARRAYSYM\n", yytext);}
"program"       {printf("%s: PROGRAMSYM\n", yytext);}
"mod"           {printf("%s: MODSYM\n", yytext);}
"and"           {printf("%s: ANDSYM\n", yytext);}
"or"            {printf("%s: ORSYM\n", yytext);}
"not"           {printf("%s: NOTSYM\n", yytext);}
"begin"         {printf("%s: BEGINSYM\n", yytext);}
"end"           {printf("%s: ENDSYM\n", yytext);}
"if"            {printf("%s: IFSYM\n", yytext);}
"then"          {printf("%s: THENSYM\n", yytext);}
"else"          {printf("%s: ELSESYM\n", yytext);}
"while"         {printf("%s: WHILESYM\n", yytext);}
"do"            {printf("%s: DOSYM\n", yytext);}
"call"          {printf("%s: CALLSYM\n", yytext);}
"const"         {printf("%s: CONSTSYM\n", yytext);}
"type"          {printf("%s: TYPESYM\n", yytext);}
"var"           {printf("%s: VARSYM\n", yytext);}
"procedure"     {printf("%s: PROCSYM\n", yytext);}



{IDENT}			{printf("%s: IDENT\n", yytext);}
{INTCON}		{printf("%s: INTCON\n", yytext);}
{CHARCON}       {printf("%s: CHARCON\n", yytext);}
"+"             {printf("%s: PLUS\n", yytext);}
"-"             {printf("%s: MINUS\n", yytext);}
"*"             {printf("%s: TIMES\n", yytext);}
"/"             {printf("%s: DIVSYM\n", yytext);}
"="             {printf("%s: EQL\n", yytext);}
"<>"            {printf("%s: NEQ\n", yytext);}
"<"             {printf("%s: LSS\n", yytext);}
"<="            {printf("%s: LEQ\n", yytext);}
">"             {printf("%s: GTR\n", yytext);}
">="            {printf("%s: GEQ\n", yytext);}
"["             {printf("%s: LBRACK\n", yytext);}
"]"             {printf("%s: RBRACK\n", yytext);}
"("             {printf("%s: LPAREN\n", yytext);}
")"             {printf("%s: RPAREN\n", yytext);}
","             {printf("%s: COMMA\n", yytext);}
";"             {printf("%s: SEMICOLON\n", yytext);}
"."             {printf("%s: PERIOD\n", yytext);}           
":="            {printf("%s: BECOME\n", yytext);}
":"             {printf("%s: COLON\n", yytext);}

"\n"			{}
"\t"            {}
"\r"            {}
" "			    {}
.	            {printf("%s: ERROR\n", yytext);}
%% /*** end ***/
int yywrap() { return 1; }
int main(int argc, char **argv)
{
	if (argc > 1) {
		if (!(yyin = fopen(argv[1], "r"))) {
			perror(argv[1]);
			return 1;
		}
	}
	while (yylex());
	return 0;
}
