# pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include <regex>
#include <vector>
#include <stdexcept>

#include "util.hpp"
#include "Token.hpp"

// helper func
BracketType char_to_bracket(char c);

class Lexer
{
public:
   
    Lexer(){};
    virtual ~Lexer(){};

    Lexer& operator<< (std::string in)
    {
        _buffer << in;
        return *this;
    }
    
    token_t getTok() 
    {
        using namespace std;
        token_t out = shared_ptr<Token>(nullptr);
     
        //std::cout << _buffer.str() << endl;
        
        // just try the regexes until no matches, return the last which did match
        std::string buff = _buffer.str();
		TokenType prev = t_End;
        int i;
        for(i = 1; i<=(int)buff.size(); ++i )
        {
			TokenType curr = t_End;
            for( auto& kv : _token_regexs )
            {
				if( std::regex_match(buff.substr(0,i), kv.second) )
                {
					curr = kv.first;
					break;
				}
			}
			
			if(curr==t_End) break;
			prev = curr;
		}
		i--;
		
        switch(prev)
		{
			case t_Int:
			{
				int v = ::atoi(buff.substr(0,i).c_str());
				out = make_shared<IntToken>(v);
				break;
			}
			case t_Float:
			{
				double v = ::atof(buff.substr(0,i).c_str());
				out = shared_ptr<FloatToken>(new FloatToken(v));
				break;
			}
			case t_String:
			{
				out = shared_ptr<StringToken>(new StringToken(buff.substr(0,i)));
				break;
			}
			case t_Operator:
			{
				out = shared_ptr<OpToken>(new OpToken(buff.substr(0,i)));
				break;
			}
			case t_Type:
			{
				out = shared_ptr<TypeToken>(new TypeToken(buff.substr(1,i)));
				break;
			}
			case t_OpenBrkt:
			{
				char c = buff.substr(0,i).c_str()[0];
				out = shared_ptr<OpenToken>(new OpenToken(char_to_bracket(c)));
				break;
			}
			case t_ClsBrkt:
			{
				char c = buff.substr(0,i).c_str()[0];
				out = shared_ptr<CloseToken>(new CloseToken(char_to_bracket(c)));
				break;
			}			
			case t_Ignore:
			{
				 _buffer.clear();
				 _buffer.str("");
				 _buffer << buff.substr(i, buff.size()-i);
				 return getTok();
				 break;   
			}
        }
        
        if(nullptr!=out.get())
        {
            _buffer.clear();
            _buffer.str("");
            _buffer << buff.substr(i, buff.size()-i);
			_prev_tok = out;
            return out;
        }
              
		_prev_tok = std::shared_ptr<EndToken>(new EndToken(buff));
        return _prev_tok; 
    }
	
	token_t prevTok() const 
	{
		return _prev_tok;
	}
    
protected:

    std::stringstream _buffer;
	token_t _prev_tok;

    static const std::vector<std::pair<TokenType,std::regex>> _token_regexs;       
};
