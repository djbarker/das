#pragma once

#include "Token.hpp"
#include "BasicLexer.hpp"

/*
   Provides a more useful interface to the lexer
*/

class TokenStream
{
public:

	TokenStream(Lexer& l);
	virtual ~TokenStream();

	token_t peek_prev() const;
	token_t peek_next() const;
	token_t peek_curr() const;
	token_t get();
	size_t getp() const;
	void setp(size_t);
	
protected:

	Lexer& _lexer;
	size_t _idx;
	std::deque<token_t> _tokens;	

};