#ifndef LEXER_TOKEN_H
#define LEXER_TOKEN_H

#include <string>

namespace lexer {
    class token {
    public:
        enum {
            identifier,
            constant,
            variable,
            keyword_new,
            bracket_parent_open,
            bracket_parent_close,
            bracket_square_open,
            bracket_square_close,
            bracket_angle_open,
            bracket_angle_close,
            comma,
            arrow,
            match
        };
    private:
        unsigned int _Type;
        std::string _Text; 
    public:
        token(unsigned Type, std::string Text = "");
        unsigned get_type();
        std::string get_text();
        std::string repr();
    };
}

#endif
