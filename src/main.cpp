#include "lexer/lexer.h"
#include <iostream>

lexer::v1 Lexer;

int main() {
    std::string Buffer;
    do {
        std::cin >> Buffer;
        Lexer.tokenize(Buffer);
    } while(!std::cin.eof());
    return 0;
}
