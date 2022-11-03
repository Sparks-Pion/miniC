scanner:lab107.y
	bison -d lab107.y
	flex lab107.l
	gcc -o scanner lab107.tab.c lex.yy.c -lm -lfl
.PHONY:clean
clean:
	rm scanner lab107.tab.c lex.yy.c lab107.tab.h