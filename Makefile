CXX=g++

all: src/main.cpp src/util.cpp src/lexing/BasicLexer.cpp src/lexing/Token.cpp
	$(CXX) src/main.cpp src/util.cpp src/lexing/BasicLexer.cpp src/lexing/Token.cpp -I./include -I./include/lexing --std=c++11 -o ddas
