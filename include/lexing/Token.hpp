#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include "util.hpp"

enum TokenType
{
    t_Unknown  = 1 << 0,
    t_Ignore   = 1 << 1,
    t_Error    = 1 << 2,
    t_Int      = 1 << 3,
    t_Float    = 1 << 4,
    t_String   = 1 << 5,
    t_Operator = 1 << 6,
};

std::string TokenType_to_string(TokenType t);

struct Token
{
    Token(){};
    virtual ~Token(){};
    virtual TokenType getType() const { return t_Unknown; }
    virtual std::string toString() const { return "BASE"; }
};

template<TokenType T, class V>
struct GenericToken : public Token
{
    GenericToken(V val):value(val){};
    virtual ~GenericToken(){};
    virtual TokenType getType() const override { return T; }
    virtual std::string toString() const override 
    { 
        std::stringstream sstr;
        sstr << TokenType_to_string(T) << "[" << value << "]";
        return sstr.str();
    }
    
    V value;
};

using IntToken    = GenericToken<t_Int, int>;
using FloatToken  = GenericToken<t_Float, double>;
using StringToken = GenericToken<t_String, std::string>;
using OpToken     = GenericToken<t_Operator, std::string>;
using ErrToken    = GenericToken<t_Error, std::string>;