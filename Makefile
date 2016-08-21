CXX=g++

SRCS=src/main.cpp \
	 src/util.cpp \
	 src/lexing/BasicLexer.cpp \
	 src/lexing/Token.cpp \
	 src/parsing/TokenStream.cpp \
	 src/parsing/Expression.cpp

INCS=-I./include \
     -I./include/lexing \
	 -I./include/parsing

all: 
	$(CXX) $(SRCS) $(INCS) --std=c++11 -o ddas
