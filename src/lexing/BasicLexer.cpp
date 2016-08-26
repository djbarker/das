#include "BasicLexer.hpp"

const std::map<TokenType,std::regex>  Lexer::_token_regexs = {
        { TokenType::t_Ignore,   std::regex("[ \\t\\n\\r]+") }, 
        { TokenType::t_Int,      std::regex("-?\\d+" ) },
        { TokenType::t_Float,    std::regex("-?(\\d+\\.|\\.\\d+|\\d+\\.\\d+)" ) },
		{ TokenType::t_Type,     std::regex("\\$\\w+")},
        { TokenType::t_Operator, std::regex("[\\+\\-\\>\\<\\?\\*\\@\\~\\#\\'\\/\\%\\^\\&\\£\\!\\|\\\\]+" ) },
		{ TokenType::t_OpenBrkt, std::regex("[\\[\\(\\{]") },
		{ TokenType::t_ClsBrkt,  std::regex("[\\]\\)\\}]") },
        { TokenType::t_String,   std::regex("\\w+" ) }
    };
	

BracketType char_to_bracket(char c)
{
	switch(c)
	{
	case '(': return Paren;
	case ')': return Paren;
	case '[': return Square;
	case ']': return Square;
	case '{': return Curly;
	case '}': return Curly;	
	default:
		throw std::runtime_error("Invalid bracket char");
	}
}