# pragma once

#include <iostream>
#include <memory>
#include "Token.hpp"
#include "Expression.hpp"

class Parser
{
public:
	Parser();
	virtual ~Parser();
	
	Parser& operator<< (std::unique_ptr<Token> tok);
	
protected:
	std::shared_ptr<Expression> root;
	std::shared_ptr<Expression> curr;
};