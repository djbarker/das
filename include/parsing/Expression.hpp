#include <iostream>
#include <memory>
#include "TokenStream.hpp"


enum ExpressionType
{
	e_Expr     = 1 << 0,
	e_Infix    = 1 << 1,
	e_Juxtapos = 1 << 2,
	e_Paren    = 1 << 3,
	e_Int	   = 1 << 4,
	e_Float    = 1 << 5,
	e_String   = 1 << 6
};

// some forward declarations
struct Expression;
struct ParenExpression;
template<ExpressionType E, class Tk> struct ValueExpression;
struct InfixExpression;
struct JuxtaposExpression;

using expr_t = std::shared_ptr<Expression>;

template<class T>
std::shared_ptr<T> cast_expr(expr_t e)
{
	return std::static_pointer_cast<T>(e);
}

struct P
{
	template<class T>
	static expr_t parse( TokenStream& ts ){	return T::parse(ts); }
};

struct Expression // Base
{
	Expression(){};
	virtual ~Expression(){};
	virtual ExpressionType getType() const = 0;
	virtual bool terminal() const = 0;
	virtual std::string toString() const = 0; 
	static expr_t parse ( TokenStream&, ExpressionType start=e_Expr );
};

template<ExpressionType E, class Tk>
struct ValueExpression : public Expression
{
	using value_t = typename Tk::value_t;
	using this_t  = ValueExpression<E,Tk>;
	
	ValueExpression(value_t v):value(v){}
	virtual ~ValueExpression() = default;
	virtual ExpressionType getType() const override { return E; }
	virtual bool terminal() const override { return true; }
	
	virtual std::string toString() const override
	{ 
		std::stringstream sstr;
		sstr << value;
		return sstr.str();
	}
	
	static expr_t parse(TokenStream& ts)
	{
		if( ts.peek_curr()->getType() == Tk::token_type )
		{
			const auto ptr = cast_tok<Tk>(ts.get());
			return std::make_shared<this_t>(ptr->value);
		}	
		else
		{
			throw std::runtime_error("Token not a value: " + ts.peek_curr()->toString());
		}
	}
	
	value_t value;
};

// typedefs
using StringExpr    = ValueExpression<e_String, StringToken >;
using IntExpr       = ValueExpression<e_Int,    IntToken >;
using FloatExpr     = ValueExpression<e_Float,  FloatToken >;


struct ParenExpression : public Expression // Non-terminal
{
	ParenExpression(expr_t e):exp(e){};
	virtual ~ParenExpression() = default;
	virtual ExpressionType getType() const override { return e_Expr; };
	virtual bool terminal() const override { return false; }
	
	virtual std::string toString() const override 
	{
		//return "{"+exp->toString()+"}";
		return exp->toString();
	}
	
	static expr_t parse( TokenStream& ts )
	{
		// TODO: account for bracket types
		
		if( ts.peek_curr()->getType() != t_OpenBrkt )
			throw std::runtime_error("Not open bracket " + ts.peek_curr()->toString());
			
		size_t pos = ts.getp();
		token_t tok = ts.get(); // consume open bracket
		auto exp = Expression::parse(ts);
		tok = ts.peek_curr();
				
		if( tok->getType()!=t_ClsBrkt)
		{
			ts.setp(pos);
			throw std::runtime_error("No close bracket");
		}
		
		ts.get(); // consume close bracket
		
		if(exp->terminal())
			return exp;
		else
			return std::make_shared<ParenExpression>(exp);
	}
	
	expr_t exp;
};

struct JuxtaposExpression : public Expression // Non-terminal
{
	JuxtaposExpression(expr_t l, expr_t r):lhs(l),rhs(r){};
	virtual ~JuxtaposExpression() = default;
	virtual ExpressionType getType() const { return e_Juxtapos; };
	virtual bool terminal() const override { return false; }
	
	virtual std::string toString() const 
	{
		return "("+lhs->toString() + " " + rhs->toString()+")";
	}
	
	static expr_t parse( TokenStream& ts )
	{
		size_t pos = ts.getp();
		expr_t lhs, rhs;
		try
		{
			lhs = Expression::parse(ts,e_Paren);
			rhs = Expression::parse(ts,e_Expr);
			
			return std::make_shared<JuxtaposExpression>(lhs,rhs);	
		}
		catch(std::runtime_error& e)
		{
			ts.setp(pos);
			throw e;
		}
	}
	
	expr_t lhs;
	expr_t rhs;
};

struct InfixExpression : public Expression // Non-terminal
{
	InfixExpression(expr_t l, expr_t r, std::string o)
		:lhs(l)
		,rhs(r)
		,op(o)
		{};
	virtual ~InfixExpression() = default;
	virtual ExpressionType getType() const { return e_Infix; }
	virtual bool terminal() const override { return false; }
	
	virtual std::string toString() const 
	{
		std::stringstream sstr;
		sstr << "(" << lhs->toString() << " " << op << " " << rhs->toString() << ")";
		return sstr.str();
	}
	
	static expr_t parse( TokenStream& ts )
	{
		size_t pos = ts.getp();
		expr_t lhs, rhs;
		try
		{
			lhs = Expression::parse(ts,e_Paren);
		}
		catch(std::runtime_error& e)
		{
			ts.setp(pos);
			throw e;
		}
		
		if( ts.peek_curr()->getType() != t_Operator )
		{
			ts.setp(pos);
			throw std::runtime_error("Tok not operator");
		}
		
		std::string op = cast_tok<OpToken>(ts.get())->value;
		
		try
		{
			rhs = Expression::parse(ts);
		}
		catch(std::runtime_error& e)
		{
			ts.setp(pos);
			throw std::runtime_error("Unable to parse rhs of infix");
		}
		
		// handle flipper
		if("\\"==op)
		{
			// flipping has higher precedence than juxtaposition & infix
			switch( rhs->getType() )
			{
				case e_Juxtapos:
				{
					expr_t rl = cast_expr<JuxtaposExpression>(rhs)->lhs;
					expr_t rr = cast_expr<JuxtaposExpression>(rhs)->rhs;
					expr_t ll = std::make_shared<JuxtaposExpression>(rl,lhs);
					expr_t lp = std::make_shared<ParenExpression>(ll);
					return std::make_shared<JuxtaposExpression>(lp,rr);
				}
				case e_Infix:
				{
					expr_t rl = cast_expr<InfixExpression>(rhs)->lhs;
					expr_t rr = cast_expr<InfixExpression>(rhs)->rhs;
					std::string op = cast_expr<InfixExpression>(rhs)->op;
					expr_t ll = std::make_shared<JuxtaposExpression>(rl,lhs);
					expr_t lp = std::make_shared<ParenExpression>(ll);
					return std::make_shared<InfixExpression>(lp,rr,op);
				}
			default:
				break;
			}
			
			expr_t flip = std::make_shared<JuxtaposExpression>(rhs,lhs);
			return std::make_shared<ParenExpression>(flip);
		}
		
		// handle constant arithmetic
		if( ("+"==op) || ("-"==op) || ("/"==op) || ("*"==op) )
		{
			std::function<double(double,double)> opf;
			if("+"==op) opf = [&](double x, double y) -> double { return x + y; };
			if("-"==op) opf = [&](double x, double y) -> double { return x - y; };
			if("/"==op) opf = [&](double x, double y) -> double { return x / y; };
			if("*"==op) opf = [&](double x, double y) -> double { return x * y; };
			
			if( e_Int == rhs->getType() && e_Int == lhs->getType() )
			{
				auto l = cast_expr<IntExpr>(lhs);
				auto r = cast_expr<IntExpr>(rhs);
				return std::make_shared<IntExpr>((int)opf(l->value,r->value));
			}
			else if( e_Float == rhs->getType() && e_Int == lhs->getType() )
			{
				auto l = cast_expr<IntExpr>(lhs);
				auto r = cast_expr<FloatExpr>(rhs);
				return std::make_shared<FloatExpr>(opf(l->value,r->value));
			}
			else if( e_Int == rhs->getType() && e_Float == lhs->getType() )
			{
				auto l = cast_expr<FloatExpr>(lhs);
				auto r = cast_expr<IntExpr>(rhs);
				return std::make_shared<FloatExpr>(opf(l->value,r->value));
			}
			else if( e_Float == rhs->getType() && e_Float == lhs->getType() )
			{
				auto l = cast_expr<FloatExpr>(lhs);
				auto r = cast_expr<FloatExpr>(rhs);
				return std::make_shared<FloatExpr>(opf(l->value,r->value));
			}
		}
		
		return std::make_shared<InfixExpression>(lhs,rhs,op);
	}
	
	std::string op;
	expr_t lhs;
	expr_t rhs;	
};