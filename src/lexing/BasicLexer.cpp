#include "BasicLexer.hpp"

using namespace std;

const vector<pair<TokenType,regex>> Lexer::_token_regexs = {
        { t_Ignore,   regex("[ \\t\\n\\r]+") }, 
        { t_Int,      regex("\\-?\\d+" ) },
        { t_Float,    regex("\\-?(\\d+\\.|\\.\\d+|\\d+\\.\\d+)" ) },
		{ t_Type,     regex("\\$\\w+")},
		{ t_Operator, regex("[=></:\\^\\\\\\!\\'\\@\\~\\*\\#\\?\\+\\-]+" ) },
		{ t_OpenBrkt, regex("[\\[\\(\\{]") },
		{ t_ClsBrkt,  regex("[\\]\\)\\}]") },
        { t_Ident,    regex("\\w+" ) }
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
		throw runtime_error("Invalid bracket char");
	}
}