# pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include <regex>
#include <map>

#include "util.hpp"
#include "Token.hpp"

class BasicLexer
{
public:
   
    BasicLexer(){};
    virtual ~BasicLexer(){};

    BasicLexer& operator<< (std::string in)
    {
        _buffer << in;
        return *this;
    }
    
    std::shared_ptr<Token> getTok() 
    {
        using namespace std;
        shared_ptr<Token> out = shared_ptr<Token>(nullptr);
     
        //std::cout << _buffer.str() << endl;
        
        // just try the regexes until one matches
        std::string buff = _buffer.str();
        size_t i;
        for(i = buff.size(); i>0; --i )
        {
            //std::cout << buff.substr(0,i) << std::endl;
            for( auto& kv : _token_regexs )
            {
                //std::cout << TokenType_to_string(kv.first) << std::endl;
                if( std::regex_match(buff.substr(0,i), kv.second) )
                {
                    switch(kv.first)
                    {
                        case Int:
                        {
                            int v = ::atoi(buff.substr(0,i).c_str());
                            out = make_shared<IntToken>(v);
                            break;
                        }
                        case Float:
                        {
                            double v = ::atof(buff.substr(0,i).c_str());
                            out = shared_ptr<FloatToken>(new FloatToken(v));
                            break;
                        }
                        case String:
                        {
                            out = shared_ptr<StringToken>(new StringToken(buff.substr(0,i)));
                            break;
                        }
                        case Operator:
                        {
                            out = shared_ptr<OpToken>(new OpToken(buff.substr(0,i)));
                            break;
                        }
                        case Ignore:
                        {
                            _buffer.clear();
                            _buffer.str("");
                            _buffer << buff.substr(i, buff.size()-i);
                            return getTok();
                            break;   
                        }
                    }
                    break;
                }
            }
            if(nullptr!=out.get()) break;
        }
        
        if(nullptr!=out.get())
        {
            _buffer.clear();
            _buffer.str("");
            _buffer << buff.substr(i, buff.size()-i);
            return out;
        }
              
        return std::shared_ptr<ErrToken>(new ErrToken(buff)); 
    }
    
protected:

    std::stringstream _buffer;

    static const std::map<TokenType,std::regex> _token_regexs;       
};