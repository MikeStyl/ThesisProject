CC=g++
CCFLAGS= -std=c++2a
FLEX=flex
YFLAGS= --output=grammar.tab.c --header=grammar.tab.h -t
YACC=bison

.PHONY: default
.DEFAULT_GOAL := default

default: compile evaluate

compile: lex.yy.o grammar.tab.o node.o rename.o eval.o codeGen.o compile.o
	$(CC) $(CCFLAGS) lex.yy.o grammar.tab.o node.o rename.o eval.o codeGen.o compile.o -o compile

evaluate: lex.yy.o grammar.tab.o node.o rename.o eval.o codeGen.o evaluate.o
	$(CC) $(CCFLAGS) lex.yy.o grammar.tab.o node.o rename.o eval.o codeGen.o evaluate.o -o evaluate

grammar.tab.o: grammar.tab.c
	$(CC) $(CCFLAGS) grammar.tab.c -c

grammar.tab.h grammar.tab.c: grammar.yacc node.h
	$(YACC) $(YFLAGS) grammar.yacc

lex.yy.o: lexer.l grammar.tab.h
	$(FLEX) lexer.l
	$(CC) $(CCFLAGS) lex.yy.c -c

node.o: node.h node.cpp
	$(CC) $(CCFLAGS) node.cpp -c

rename.o: node.h rename.cpp
	$(CC) $(CCFLAGS) rename.cpp -c

eval.o: node.h eval.cpp
	$(CC) $(CCFLAGS) eval.cpp -c

codeGen.o: node.h codeGen.cpp
	$(CC) $(CCFLAGS) codeGen.cpp -c

run: compile
	./compile $(ARGS)
	$(CC) $(CCFLAGS) -o compiledProgram output.cpp
	./compiledProgram

clean:
	/bin/rm -f lex.yy.* grammar.tab.* *.o output* test compiledProgram a.out compile evaluate