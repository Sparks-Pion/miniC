%locations
//bison的.y文件中，加入％locations，这样bison加入－d编译后的.h符号表文件中会多出一个YYLTYPE结构体定义和一个该结构体类型的全局变量yylloc，这样，flex的.l文件include该.h文件后，该结构体类型就可以被flex知道，且flex可以向yylloc里面填入信息。

%{
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "def.h"

#include <bits/stdc++.h>
extern int yylineno;
extern char *yytext;
extern FILE *yyin;
char myerror[255];
extern NProgram *p;
extern int allerror;
void myyyerror();

extern "C"{
void yyerror(const char* fmt, ...);
extern int yylex(void);
}
%}

%union {
		NExtDefFunDec  *nExtDefFunDec;
        NExtDefList *nExtDefList;
        NExtDef *nExtDef;
        NSpecifier *nSpecifier;
        NExtDecList *nExtDecList;
        NDecList *nDecList;
        NVarDec *nVarDec;
        NFunDec *nFunDec;
        NCompSt *nCompSt;
        NVarList *nVarList;
        NParamDec *nParamDec;
        NDefList *nDefList;
        NDec *nDec;
        NDef *nDef;
        NStmtList *nStmtList;
        NStmt *nStmt;
        NExpression *nExpression;
        NArgs *nArgs;
        NIdentifier *nIdentifier;
        NStructSpecifier *nStructSpecifier;

        std::string *text;
		int		type_int;
        int		line;
		float	type_float;
		int		type_char;
		char	type_id[32];
}
//union的默认结构体类型为YYSTYPE，相当于自己把YYSTYPE重新定义为union类型。所以相应的yylval也变为union类型。
//这个union类型-d选项编译时会放在头文件中
//  %type 定义非终结符的语义值类型
%type  <nExtDefFunDec> program
%type  <nExtDefList> ExtDefList
%type  <nExtDef> ExtDef
%type  <nSpecifier> Specifier
%type  <nExtDecList> ExtDecList
%type  <nDecList> DecList
%type  <nVarDec> VarDec
%type  <nFunDec> FunDec
%type  <nCompSt> CompSt

%type  <nDefList> DefList
%type  <nVarList> VarList
%type  <nParamDec> ParamDec
%type  <nDec> Dec
%type  <nDef> Def
%type  <nStmtList> StmtList

%type  <nExpression> Exp
%type  <nStmt> Stmt
%type  <nArgs> Args
%type  <nIdentifier> OptTag  Tag
%type  <nStructSpecifier> StructSpecifier
//% token 定义终结符的语义值类型
%token <type_int> INT                   //指定INT的语义值是type_int，有词法分析得到的数值
%token <type_id> ID RELOP TYPE    //指定ID,RELOP 的语义值是type_id，有词法分析得到的标识符字符串mmecpy得到的
%token <type_float> FLOAT               //指定ID的语义值是type_float，有词法分析得到的float
%token <type_char> CHAR
%token <line> STRUCT LP RP LC RC LB RB SEMI COMMA     //用bison对该文件编译时，带参数-d，生成的exp.tab.h中给这些单词进行编码，可在lex.l中包含parser.tab.h使用这些单词种类码
%token <line> DOT PLUS MINUS STAR DIV MOD ASSIGNOP AND OR NOT IF BREAK ELSE WHILE RETURN PLUSASS MINUSASS STARASS DIVASS MODASS PLUSPLUS MINUSMINUS
//由低到高的定义优先级

%left COMMA
%left ASSIGNOP PLUSASS MINUSASS STARASS DIVASS MODASS
//先不支持？三目运算
%left OR
%left AND
%left RELOP//比较运算内部暂不定义优先级
%left PLUS MINUS
%left STAR DIV MOD
%right UMINUS NOT UPLUSPLUS UMINUSMINUS
%left PLUSPLUS MINUSMINUS 
%right DOT
%left LB

%nonassoc LOWER_THEN_ELSE //无结合性
%nonassoc ELSE

%%
/*High-level Definitions*/
program: ExtDefList {p=new NProgram($1);if($1) p->line=$1->line;} /*显示语法树*/
        ;
ExtDefList: {  $$=nullptr;}
        | ExtDef ExtDefList {$$=new NExtDefList(*$1,$2); $$->line=$1->line;}
        ;
ExtDef: Specifier ExtDecList SEMI {}
        | Specifier SEMI {} 
        | Specifier FunDec CompSt {}
        ;
ExtDecList: VarDec {}
        | VarDec COMMA ExtDecList {}
        ;

/*Specifiers*/
Specifier: TYPE {}   
        | StructSpecifier {}   
        ;

StructSpecifier: STRUCT OptTag LC DefList RC {}   
        | STRUCT Tag {}   
        ;

OptTag: ID {}   
        | {}   
        ;

Tag: ID {}   
        ;

/*Declarators*/
VarDec:  ID {}   //ID结点，标识符符号串存放结点的type_id
        | VarDec LB INT RB {}
        ;
FunDec: ID LP VarList RP {}
        | ID LP RP {}
        ;
VarList: ParamDec {}
        | ParamDec COMMA VarList {}
        ;
ParamDec: Specifier VarDec {}
        ;

/*Statements*/
CompSt:   LC DefList StmtList RC {}
        ;
StmtList: {}  
        | Stmt StmtList  {}
        ;
Stmt:     Exp SEMI {}
        | CompSt {}      //复合语句结点直接最为语句结点，不再生成新的结点
        | RETURN Exp SEMI {}
        | IF LP Exp RP Stmt %prec LOWER_THEN_ELSE   {}
        | IF LP Exp RP Stmt ELSE Stmt {}
        | WHILE LP Exp RP Stmt {}
        | BREAK SEMI {}
        ;

/*Local Definitions*/
DefList: {}
        | Def DefList {}
        ;
Def: Specifier DecList SEMI {}
        ;

DecList:  Dec  {}
        | Dec COMMA DecList  {}
        ;
Dec:      VarDec  {}
        | VarDec ASSIGNOP Exp  {}
        ;

/*Expressions*/
Exp:      Exp ASSIGNOP Exp {}//$$结点type_id空置未用，正好存放运算符
        
        | Exp PLUSASS Exp   {}//复合赋值运算
        | Exp MINUSASS Exp   {}
        | Exp STARASS Exp   {}
        | Exp DIVASS Exp   {}

        | PLUSPLUS Exp %prec UPLUSPLUS   {}//这里利用BISON %prec表示和UMINUS同优先级
        | MINUSMINUS Exp %prec UMINUSMINUS   {}//这里利用BISON %prec表示和UMINUS同优先级
        | Exp PLUSPLUS   {}//这里利用BISON %prec表示和UMINUS同优先级
        | Exp MINUSMINUS  {}//这里利用BISON %prec表示和UMINUS同优先级

        | Exp AND Exp   {}
        | Exp OR Exp    {}
        | Exp RELOP Exp {}  //词法分析关系运算符号自身值保存在$2中
        | Exp PLUS Exp  {}
        | Exp MINUS Exp {}
        | Exp STAR Exp  {}
        | Exp DIV Exp   {}
        
        | Exp MOD Exp   {}
        
        | LP Exp RP     {}
        | MINUS Exp %prec UMINUS   {}//这里利用BISON %prec表示和UMINUS同优先级 相当于虚拟出一个运算符
        | NOT Exp       {}
        | ID LP Args RP {}
        | ID LP RP      {}
        | Exp LB Exp RB {}
        | Exp DOT ID    {}
        | ID            {}
        | INT           {$$=new NInteger($1);$$->line=yylineno;}
        | FLOAT         {}
        | CHAR          {}
        ;
Args:   Exp COMMA Args {}
        | Exp {}
        ;

%%
// int main(int argc, char *argv[]){
// 	yyin=fopen(argv[1],"r");
// 	if (!yyin) return 1;
// 	yylineno=1;
//         allptr=NULL;
//         allerror=0;
// 	yyparse();
//         if(p) p->parse();
//         if(!allerror)
//         semanticanalysis(allptr,0);
//         if(!allerror){
//         //std::string temp=argv[1];
//         //temp[temp.size()-1]='l';
//         //temp+="l";
//         //FILE *tt=freopen(temp.data(),"w",stdout);
//         //codeGen(allptr,0);
//         //fclose(tt);
//         //toy_as(temp);
//         //char argvv[2][10];
//         //strcpy(argvv[0],"llc");
//         //temp[temp.size()-2]='b';
//         //temp[temp.size()-1]='c';
//         //strcpy(argvv[1],temp.data());
//         //toy_llc(2,(char **)argvv);
//         }
// 	return 0;
// }

#include<stdarg.h>
void yyerror(const char* fmt, ...)
{
    va_list ap;//取参数对应的函数
    va_start(ap, fmt);
    fprintf(stderr, "Grammar Error at Line %d Column %d: ", yylloc.first_line,yylloc.first_column);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr,"%s",myerror);
    fprintf(stderr,"%s", ".\n");
    memset(myerror,0,sizeof(myerror));
}	
void myyyerror()
{
    fprintf(stderr, "Grammar Error at Line %d Column %d: ", yylloc.first_line,yylloc.first_column);
    fprintf(stderr,"%s",myerror);
    fprintf(stderr, "%s",".\n");
    memset(myerror,0,sizeof(myerror));
}