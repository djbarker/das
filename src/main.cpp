#include <iostream>
#include "BasicLexer.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    BasicLexer lexer;
    
    lexer << argv[1];
    
    std::shared_ptr<Token> tok;
    do
    {
        tok = lexer.getTok();
        cout << tok->getType() << ": " << tok->toString() << endl;
    }
    while( !(tok->getType() & (Unknown|Error)) );
    
    return 0;
}