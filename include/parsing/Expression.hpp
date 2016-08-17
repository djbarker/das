#include <iostream>
#include "Token.hpp"

enum ExpressionType
{
	e_NaE; // Not an Expression
	e_StringConst,
	e_IntConst,
	e_FloatConst,
	e_Add,
	e_Sub,
	e_Mult,
	e_Div,	
};

struct Expression 
{
	Expression(){};
	virtual ~Expression(){};
	virtual ExpressionType getType() const { return NaE; }
	virtual std::string toString() const { return "?"; }
	virtual bool parse(const Token& tok) {};	
};

template<ExpressionType E, class V>
struct ValueExpression : public Expression
{
	ValueExpression(V val):value(val){};
	virtual ~ValueExpression(){};
	virtual ExpressionType getType() const { return E; }
	
	virtual std::string toString() const 
	{ 
		std::stringstream sstr;
		sstr << value;
		return sstr.str();	
	}
	
	virtual bool parse(const Token& tok) 
	{
		
	}
	
	V value;
};

template<ExpressionType E>
struct BinOpExpression : public BinOpExpression
{
	BinOpExpression(std::string symbol, Expression* lhs, Expression* rhs)
	:symbol(symbol)
	,lhs(lhs)
	,rhs(rhs)
	{
	}
	
	virtual ~BinOpExpression(
	{
		delete lhs;
		delete rhs;	
	}
	
	virtual ExpressionType getType() const { return E; }
	virtual std::std toString() const 
	{
		std::string sstr;
		sstr << "(" << lhr.toString() << " " << symbol << " " << rhs.toString() << ")";
		return sstr.str();
	}
	
	std::string symbol;
	Expression* lhs;
	Expression* rhs;	
};

// typedefs
using StringExpr    = ValueExpression<e_String, std::string>;
using IntExpr       = ValueExpression<e_Int, int>;
using FloatExpr     = ValueExpression<e_Float, double>;
using AddExpr       = BinOpExpression<e_Add>;
using SubExpr       = BinOpExpression<e_Sub>;
using MultExpr      = BinOpExpression<e_Mult>;
using DivExpr       = BinOpExpression<e_Div>;