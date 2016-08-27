#pragma once

#include <map>
#include <string>
#include "Expression.hpp"

class Interpreter
{
public:
	Interpreter();
	virtual ~Interpreter();
	
	expr_t interpret(expr_t exp, Interpreter& env);
	
	// generic atom interpret
	template<class T>
	expr_t interpret_atom(expr_t exp, Interpreter& env)
	{
		return cast_expr<T>(exp);
	}
	
	// specific methods for interpreting expressions
	expr_t interpret_ident(expr_t exp, Interpreter& env);
	expr_t interpret_infix(expr_t exp, Interpreter& env);
	expr_t interpret_juxta(expr_t exp, Interpreter& env);
	expr_t interpret_int(expr_t exp, Interpreter& env);
	expr_t interpret_float(expr_t exp, Interpreter& env);

protected:
	
	expr_t compute_infix(expr_t lhs, expr_t rhs, std::string op, Interpreter& env);

	std::map<std::string, expr_t> _g_exp;
};