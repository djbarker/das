#include <iostream>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "BasicLexer.hpp"
#include "Expression.hpp"
#include "Interpreter.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	Interpreter intrp;
	
	while(true)
	{
		Lexer lexer;
		
		char * line = readline("> ");
        if(!line) break;
        if(*line) add_history(line);
		std::string ln(line);
		
        if( ("exit"==ln) || ("quit"==ln) ) return 0;
        
		try
		{
			lexer << ln;
			TokenStream tok_stream(lexer);
			expr_t root = Expression::parse(tok_stream);
			cout << "Parsed expr: " << root->toString() << endl;
			cout << "Intrpd expr: " << intrp.interpret(root, intrp)->toString() << endl;
		}
		catch(runtime_error& e)
		{
			cout << e.what() << endl;
		}
		
		free(line);
	}
	
    return 0;
}