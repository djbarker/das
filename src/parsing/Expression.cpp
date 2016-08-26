#include "Expression.hpp"

using namespace std;

int rec_level = 0;

std::map<std::string, std::pair<int,InfixExpression::OpDir> >
InfixExpression::Precedence = {
		{"-",  {1,  Left } },
		{"+",  {2,  Left } },
		{"/",  {3,  Right} },
		{"*",  {4,  Left } },
		{"^",  {5,  Right} },
		{"\\", {6,  Left } }
	};

std::shared_ptr<Expression> Expression::parse( TokenStream& tok_stream, ExpressionType start )
{
	// just try them in order of precedence
	std::shared_ptr<Expression> out;
	
	const bool debug = false;
	rec_level++;
	if(debug) cout << rec_level << " > " << tok_stream.getp() << ", " << tok_stream.peek_curr()->toString() << endl;
	
	if( start <= e_Infix)
	try
	{
		if(debug) cout << rec_level << " > " << "Try Infix" << endl;
		out = InfixExpression::parse(tok_stream);
		if(debug) cout << rec_level << " > " << out->toString() << endl;
		rec_level--;
		return out;
	}
	catch(runtime_error& e){ if(debug) cout << rec_level << " > " << "B: " << e.what() << endl; }
	
	if( start <= e_Juxtapos)
	try
	{
		if(debug) cout << rec_level << " > " << "Try Juxta" << endl;
		out = JuxtaposExpression::parse(tok_stream);
		if(debug) cout << rec_level << " > " << out->toString() << endl;
		rec_level--;
		return out;
	}
	catch(runtime_error& e){ if(debug) cout << rec_level << " > " << "J: " <<  e.what() << endl; }
	
	if(start <= e_Paren)
	try
	{
		if(debug) cout << rec_level << " > " << "Try Paren" << endl;
		out = ParenExpression::parse(tok_stream);
		if(debug) cout << rec_level << " > " << out->toString() << endl;
		rec_level--;
		return out;
	}
	catch(runtime_error& e){ if(debug) cout << rec_level << " > " << "P: " << e.what() << endl; }
	
	if( start <= e_Int)
	try
	{
		if(debug) cout << rec_level << " > " << "Try Int" << endl;
		out = IntExpr::parse(tok_stream);
		rec_level--;
		return out;
	}
	catch(runtime_error& e){ if(debug) cout << rec_level << " > " << "I: " << e.what() << endl; }
	
	if( start <= e_Float)
	try
	{
		if(debug) cout << rec_level << " > " << "Try Float" << endl;
		out = FloatExpr::parse(tok_stream);
		rec_level--;
		return out;
	}
	catch(runtime_error& e){ if(debug) cout << rec_level << " > " << "I: " << e.what() << endl; }
	
	
	if( start <= e_String)
	try
	{
		if(debug) cout << rec_level << " > " << "Try String" << endl;
		out = StringExpr::parse(tok_stream);
		rec_level--;
		return out;
	}
	catch(runtime_error& e){ if(debug) cout << rec_level << " > " << "S: " << e.what() << endl; }
	
	
	rec_level--;
	throw runtime_error("No grammar match!");
}
