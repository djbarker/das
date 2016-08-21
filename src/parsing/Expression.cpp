#include "Expression.hpp"

using namespace std;

std::shared_ptr<Expression> parse( TokenStream& tok_stream, int start )
{
	// just try them in order of precedence
	std::shared_ptr<Expression> out;
	
	cout << "> " << tok_stream.getp() << endl;
	cout << "> " << tok_stream.peek_curr()->toString() << endl;
	
	if( tok_stream.peek_curr()->getType() == t_ClsBrkt )
	{
		throw runtime_error("Expr cannot start with )");
	}
	
	if( start < 1)
	try
	{
		cout << "Try Juxta" << endl;
		out = std::make_shared<JuxtaposExpression>();
		out->parse(tok_stream);
		return out;
	}
	catch(runtime_error& e){ cout << "J: " <<  e.what() << endl; }
	
	if(start < 2)
	try
	{
		cout << "Try Paren" << endl;
		out = std::make_shared<ParenExpression>();
		out->parse(tok_stream);
		return out;
	}
	catch(runtime_error& e){ cout << "P: " << e.what() << endl; }
	
	if( start < 3)
	try
	{
		cout << "Try Int" << endl;
		out = std::make_shared<IntExpr>();
		out->parse(tok_stream);
		return out;
	}
	catch(runtime_error& e){ cout << "I: " << e.what() << endl; }
	
	
	throw runtime_error("No grammar match!");
}
