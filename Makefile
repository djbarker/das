CXX=g++

CFLAGS=--std=c++11 

LIBS=-lm \
     -lreadline

LFLAGS=-L/usr/lib

SRCS=src/main.cpp \
	 src/util.cpp \
	 src/lexing/BasicLexer.cpp \
	 src/lexing/Token.cpp \
	 src/parsing/TokenStream.cpp \
	 src/parsing/Expression.cpp

IFLAGS=-I./include \
       -I./include/lexing \
	   -I./include/parsing

OBJS = $(SRCS:.c=.o)

MAIN = ddas

.PHONY: depend clean

all: $(MAIN)
	@echo  ddas has been compiled

$(MAIN): $(OBJS) 
	$(CXX) $(CFLAGS) $(IFLAGS) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CXX) $(CFLAGS) $(IFLAGS) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
