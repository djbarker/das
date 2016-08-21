#include <iostream>
#include "TokenStream.hpp"

enum ExpressionType
{
	e_Expression,
	e_StringConst,
	e_IntConst,
	e_FloatConst,
	e_Add,
	e_Sub,
	e_Mult,
	e_Div,
	e_Juxtapos,
	e_Paren,
};

struct Expression;

struct Expression // Base
{
	Expression(){};
	virtual ~Expression(){};
	virtual ExpressionType getType() const { return e_Expression; }
	virtual std::string toString() const = 0;
	virtual void parse ( TokenStream& ) = 0;
};


using expr_t = std::shared_ptr<Expression>;
std::shared_ptr<Expression> parse( TokenStream& tok_stream, int start = 0 );

template<ExpressionType E, class Tk>
struct ValueExpression : public Expression // Terminal
{
	using value_t = typename Tk::value_t;
	
	virtual ExpressionType getType() const override { return E; }
	
	virtual std::string toString() const override
	{ 
		std::stringstream sstr;
		sstr << value;
		return sstr.str();
	}
	
	virtual void parse(TokenStream& ts) override
	{
		if( ts.peek_curr()->getType() == Tk::token_type )
		{
			const auto ptr = cast_tok<Tk>(ts.peek_curr());
			value = ptr->value;
			ts.get(); // consume token 
		}	
		else
		{
			throw std::runtime_error("token not an int");
		}
	}
	
	value_t value;
};

struct ParenExpression : public Expression // Non-terminal
{
	ParenExpression() = default;
	virtual ~ParenExpression() = default;
	virtual ExpressionType getType() const override { return e_Expression; };
	
	virtual std::string toString() const override 
	{
		return "(" + exp->toString() + ")";
	}
	
	virtual void parse( TokenStream& ts ) override
	{
		// TODO: account for bracket types
		
		if( ts.peek_curr()->getType() != t_OpenBrkt )
			throw std::runtime_error("Not open bracket " + ts.peek_curr()->toString());
			
		size_t pos = ts.getp();
		token_t tok = ts.get(); // consume open bracket
		exp = ::parse(ts);
		tok = ts.peek_curr();
		std::cout << tok->toString() << std::endl;
		
		if( tok->getType()!=t_ClsBrkt)
		{
			ts.setp(pos);
			throw std::runtime_error("No close bracket");
		}
		
		ts.get(); // consume close bracket
	}
	
	expr_t exp;
};

struct JuxtaposExpression : public Expression // Non-terminal
{
	JuxtaposExpression() = default;
	virtual ~JuxtaposExpression() = default;
	virtual ExpressionType getType() const { return e_Juxtapos; };
	
	virtual std::string toString() const 
	{
		return "("+lhs->toString() + " " + rhs->toString()+")";
	}
	
	virtual void parse( TokenStream& ts )
	{
		size_t pos = ts.getp();
		try
		{
			std::cout << "Parsing lhs" << std::endl;
			lhs = ::parse(ts,1);
			std::cout << "Parsing rhs" << std::endl;
			rhs = ::parse(ts,0);	
		}
		catch(std::runtime_error& e)
		{
			std::cout << e.what() << std::endl;
			ts.setp(pos);
			throw e;
		}
	}
	
	expr_t lhs;
	expr_t rhs;
};

template<ExpressionType E>
struct BinOpExpression : public Expression // Non-terminal
{
	BinOpExpression() = default;
	virtual ~BinOpExpression() = default;
	
	virtual ExpressionType getType() const { return E; }
	virtual std::string toString() const 
	{
		std::stringstream sstr;
		sstr << "(" << lhs->toString() << " " << symbol << " " << rhs->toString() << ")";
		return sstr.str();
	}
	
	virtual void parse( TokenStream& ts )
	{
		size_t pos = ts.getp();
		try
		{
			lhs = ::parse(ts);
		}
		catch(std::exception& e)
		{
			ts.setp(pos);
			throw e;
		}
		
		if( ts.peek_curr()->getType() != t_Operator )
		{
			ts.setp(pos);
			throw std::runtime_error("");
		}
		
		symbol = cast_tok<OpToken>(ts.get())->value;
		
		try
		{
			rhs = ::parse(ts);
		}
		catch(std::exception& e)
		{
			ts.setp(pos);
			throw std::runtime_error("");
		}
	}
	
	std::string symbol;
	expr_t lhs;
	expr_t rhs;	
};

// typedefs
using StringExpr    = ValueExpression<e_StringConst, StringToken >;
using IntExpr       = ValueExpression<e_IntConst,    IntToken >;
using FloatExpr     = ValueExpression<e_FloatConst,  FloatToken >;
using AddExpr       = BinOpExpression<e_Add>;
using SubExpr       = BinOpExpression<e_Sub>;
using MultExpr      = BinOpExpression<e_Mult>;
using DivExpr       = BinOpExpression<e_Div>;