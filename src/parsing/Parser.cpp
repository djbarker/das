#include "Parser.hpp"

using namespace std;

Parser::Parser()
:root(nullptr)
{
}

// takes tokens and builds expressions
Parser& Parser::operator<< (std::unique_ptr<Token> tok)
{
	// start
	if(root) // == nullptr
	{
		root = tok;
		curr = tok;
	}
	
	if( curr->getType()==Constant )
	{
		if( tok->getType() & (t_Int|t_Float) )
		{
			cerr << "Error, cannot juxtapose two constants!" << endl;
		}
		else if( tok->getType()==t_Operator )
		{
			if ( "+" == tok->getValue() )
			{
				curr = AddExpr("+", curr, nullptr);
			}
			else if ( "-" == tok->getValue() )
			{
				curr = SubExpr("-", curr, nullptr);
			}
		}
	}
	else if( curr->getType()==)
}