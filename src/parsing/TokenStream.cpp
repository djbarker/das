#include "TokenStream.hpp"

TokenStream::TokenStream(Lexer& l)
:_lexer(l)
,_idx(0)
{
	token_t tok;
	do
	{
		tok = l.getTok();
		_tokens.push_back(tok);
	}
	while( !(tok->getType() & (t_Unknown|t_Error)) );
	std::cout << "TokStream read " << _tokens.size() << std::endl;
}

TokenStream::~TokenStream()
{
}

token_t TokenStream::peek_prev() const
{
	return _tokens[_idx-1];
}

token_t TokenStream::peek_curr() const
{
	return _tokens[_idx];
}

token_t TokenStream::peek_next() const
{
	return _tokens[_idx+1];
}

token_t TokenStream::get()
{
	return _tokens[++_idx];
}

size_t TokenStream::getp() const
{
	return _idx;
}

void TokenStream::setp(size_t i)
{
	_idx = i;
}