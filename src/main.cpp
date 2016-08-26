#include <iostream>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "BasicLexer.hpp"
#include "Expression.hpp"


using namespace std;

int main(int argc, char* argv[])
{
	while(true)
	{
		Lexer lexer;
		
		char * line = readline("> ");
        if(!line) break;
        if(*line) add_history(line);
		std::string ln(line);
		
        if( ("exit"==line) || ("quit"==line) ) return 0;
        
		lexer << ln;
		TokenStream tok_stream(lexer);
		expr_t root = Expression::parse(tok_stream);
		cout << "Parsed expression: " << root->toString() << endl;
		
		free(line);
	}
	
    return 0;
}