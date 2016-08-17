#include "Token.hpp"

using namespace std;

std::string TokenType_to_string(TokenType t)
{
    switch(t)
    {
        case t_Unknown:
            return "UNKNOWN";
            break;
        case t_Ignore:
            return "IGNORE";
            break;
        case t_Error:
            return "ERROR";
            break;
        case t_Int:
            return "INT";
            break;
        case t_Float:
            return "FLOAT";
            break;
        case t_String:
            return "STRING";
            break;
        case t_Operator:
            return "OPERATOR";
            break;
        default:
            return "VERY_UNKNOWN";
    }
}

namespace std
{
    std::string to_string(const std::string& s)
    {
        return s;
    }  
};
