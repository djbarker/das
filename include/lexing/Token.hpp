#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include "util.hpp"

enum TokenType
{
    t_Unknown   = 1 << 0,
    t_Ignore    = 1 << 1,
    t_End       = 1 << 2,
    t_Int       = 1 << 3,
    t_Float     = 1 << 4,
    t_String    = 1 << 5,
    t_Operator  = 1 << 6,
	t_OpenBrkt  = 1 << 7,
	t_ClsBrkt   = 1 << 8,
	t_Type      = 1 << 9,
};

enum BracketType
{
	Paren,
	Curly,
	Square,
	Angle
};

std::string TokenType_to_string(TokenType t);

struct Token
{
    Token(){};
    virtual ~Token(){};
    virtual TokenType getType() const { return t_Unknown; }
    virtual std::string toString() const { return "BASE"; }
};

typedef std::shared_ptr<Token> token_t;

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
    
	static const TokenType token_type = T;
	using value_t = V;
    V value;
};

using IntToken    = GenericToken<t_Int, int>;
using FloatToken  = GenericToken<t_Float, double>;
using StringToken = GenericToken<t_String, std::string>;
using OpToken     = GenericToken<t_Operator, std::string>;
using TypeToken   = GenericToken<t_Type, std::string>;
using EndToken    = GenericToken<t_End, std::string>;
using OpenToken   = GenericToken<t_OpenBrkt, BracketType>;
using CloseToken  = GenericToken<t_ClsBrkt,  BracketType>;

template<class T>
std::shared_ptr<T> cast_tok(token_t t)
{
	return std::static_pointer_cast<T>(t);
}