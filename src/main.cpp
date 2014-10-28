#include "lexer/lexer.h"
#include <iostream>

lexer::v1 Lexer;
char CBuffer[1024];

int main() {
    std::string Buffer;
    do {
        std::cin.getline(CBuffer, 1024);
        Buffer = CBuffer;;
        std::list<lexer::token>&& Tokens = Lexer.tokenize(Buffer);
        std::for_each(Tokens.begin(), Tokens.end(), 
            [](lexer::token T) {
                std::cout << T.repr() + " ";
            }
        );
        std::cout << std::endl;
    } while(!std::cin.eof());
    return 0;
}
