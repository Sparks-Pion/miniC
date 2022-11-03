/* PL词法分析器 */
/* 功能：能够识别出PL支持的所有单词符号并给出种别值 */
/* 说明：在下面的begin和end之间添加代码，已经实现了标识符和整常量的识别，你需要完成剩下的部分，加油吧！ */
/* 提示：因为是顺序匹配，即从上至下依次匹配规则，所以需要合理安排顺序～ */
%{
#include <stdio.h>
%} /*** begin ****/

INTCON			[\-]?[1-9][0-9]*|0
IDENT			[A-Za-z][A-Za-z0-9]*

 


%%  

{INTCON}		{printf("%s: INTCON\n", yytext);}
{IDENT}			{printf("%s: IDENT\n", yytext);}

 


\n				{}
.				{}
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
