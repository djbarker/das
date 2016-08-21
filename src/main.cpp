#include <iostream>
#include "BasicLexer.hpp"
#include "Expression.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	{ // show lexer output
    Lexer lexer;
    lexer << argv[1];
    std::shared_ptr<Token> tok;
    do
    {
        tok = lexer.getTok();
        cout << tok->toString() << ", ";
    }
    while( !(tok->getType() & (t_Unknown|t_Error)) );
	cout << endl;
	}
	
	cout << "Building parse tree" << endl;
	
	{ // parse
	Lexer lexer;
	lexer << argv[1];
	TokenStream tok_stream(lexer);
	std::shared_ptr<Expression> root = ::parse(tok_stream);
	cout << "Parsed expression: "  << endl;
	cout << "  " << root->toString() << endl;
	}
	
    return 0;
}