#ifndef LEXER_TOKEN_H
#define LEXER_TOKEN_H

namespace lexer {
    class token {
    public:
        enum type {
            identifier,
            constant,
            keyword,
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
    };
}

#endif
