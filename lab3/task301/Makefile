all: calc

OBJS =  parser.o \
		token.o  \
		main.o \

parser.cpp: foo.y
	bison -d -v foo.y -o $@

token.cpp: calc.l foo.hpp 
	flex -o $@ calc.l

%.o: %.cpp
	g++  -o $@ $<  -lpthread -ldl -c 

calc: $(OBJS)
	g++ -o $@ $(OBJS)  -lfl -lstdc++  -lpthread -ldl

clean:
	$(RM) -rf $(OBJS) calc