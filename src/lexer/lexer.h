#ifndef LEXER_H
#define LEXER_H

#include "fsm.h"
#include "token.h"
#include <string>
#include <list>


namespace lexer {
    class v1 : public fsm::receiver {
        fsm _FSM;
        enum {
            state_start,
            state_identifier,
            state_n,
            state_ne,
            state_new,
            state_bracket_parent_open,
            state_bracket_parent_close,
            state_bracket_square_open,
            state_bracket_square_close,
            state_bracket_angle_open,
            state_bracket_angle_close,
            state_comma,
            state_minus,
            state_arrow,
            state_match,
            state_variable,
            state_constant_0,
            state_constant
        };
        std::list<token> _CurrentTokenList;
    public:
        v1();
        std::list<token> tokenize(std::string InputString);
        virtual void receive_token(unsigned int TokenType, std::string TokenText);
    };
}

#endif
