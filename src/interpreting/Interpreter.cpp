#include "Interpreter.hpp"

using namespace std;

Interpreter::Interpreter()
{
}

Interpreter::~Interpreter()
{
}

expr_t Interpreter::interpret(expr_t root, Interpreter& env)
{
	switch(root->getType())
	{
	case e_Infix:    return interpret_infix(root, env);
	case e_Ident:    return interpret_ident(root, env);
	case e_Int:      return interpret_int(root, env);
	case e_Float:    return interpret_float(root, env);
	case e_Juxtapos: return interpret_juxta(root, env);
	
	default:
		cout << root->toString() << endl;
		return root;
	}
}

expr_t Interpreter::interpret_ident(expr_t exp, Interpreter& env)
{
	auto s = cast_expr<IdentExpr>(exp);
	try
	{
		return interpret( _g_exp.at(s->value), env );
	}
	catch(std::out_of_range& e)
	{
		throw runtime_error("Variable " + s->value + " does not exist!");
	}
}

expr_t Interpreter::interpret_int(expr_t exp, Interpreter& env)
{
	return interpret_atom<IntExpr>(exp, env);
}

expr_t Interpreter::interpret_float(expr_t exp, Interpreter& env)
{
	return interpret_atom<FloatExpr>(exp, env);
}

expr_t Interpreter::interpret_infix(expr_t exp, Interpreter& env)
{
	auto e = cast_expr<InfixExpression>(exp);
	auto lhs = e->lhs;
	auto rhs = e->rhs;
	auto op  = e->op;
	
	if(":"==op)
	{
		if( lhs->getType() != e_Ident )
			throw runtime_error("Can only assign to identifier!");
		
		string name = cast_expr<IdentExpr>(lhs)->value;
		_g_exp[name] = rhs;
		return lhs;
	}

	return compute_infix(lhs,rhs,op,env);
}

expr_t Interpreter::compute_infix(expr_t lhs, expr_t rhs, string op, Interpreter& env )
{
	if( ("+"==op) || ("-"==op) || ("/"==op) || ("*"==op) || ("^"==op) )
	{
		std::function<double(double,double)> opf;
		if("+"==op) opf = [&](double x, double y) -> double { return x + y; };
		if("-"==op) opf = [&](double x, double y) -> double { return x - y; };
		if("/"==op) opf = [&](double x, double y) -> double { return x / y; };
		if("*"==op) opf = [&](double x, double y) -> double { return x * y; };
		if("^"==op) opf = [&](double x, double y) -> double { return pow(x,y); };
		
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
		else
		{
			return compute_infix(interpret(lhs,env), interpret(rhs,env), op, env);	
		}
	}
	else
	{
		// TODO: handle user defined infix ops here
		throw runtime_error("User defined infix not implemented!");
	}
}

expr_t Interpreter::interpret_juxta(expr_t exp, Interpreter& env)
{
	return exp;
}