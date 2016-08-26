#include "Token.hpp"

using namespace std;

std::string TokenType_to_string(TokenType t)
{
    switch(t)
    {
        case t_Unknown:  return "UNKNOWN";
        case t_Ignore:   return "IGNORE";
        case t_End:      return "END";
        case t_Int:      return "INT";
        case t_Float:    return "FLOAT";
        case t_String:   return "STRING";
        case t_Operator: return "OPERATOR";
		case t_OpenBrkt: return "OPEN";
		case t_Type:     return "TYPE";
		case t_ClsBrkt:  return "CLOSE";
        default:         return "VERY_UNKNOWN";
    }
}

namespace std
{
    std::string to_string(const std::string& s)
    {
        return s;
    }  
};
