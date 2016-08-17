#include "BasicLexer.hpp"

const std::map<TokenType,std::regex>  BasicLexer::_token_regexs = {
        std::make_pair( TokenType::t_Ignore,   std::regex("[ \\t\\n\\r]+") ), 
        std::make_pair( TokenType::t_Int,      std::regex("-?\\d+" ) ),
        std::make_pair( TokenType::t_Float,    std::regex("-?(\\d+\\.|\\.\\d+|\\d+\\.\\d+)" ) ),
        std::make_pair( TokenType::t_Operator, std::regex("[\\+\\-\\>\\<\\?\\*\\@\\~\\#\\'\\/\\$\\%\\^\\&\\£\\!\\|\\\\]+" ) ),
        std::make_pair( TokenType::t_String,   std::regex("\\w+" ) )
    };