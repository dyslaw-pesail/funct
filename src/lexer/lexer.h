#ifndef LEXER_H
#define LEXER_H

#include "fsm.h"
#include <string>
#include <list>

namespace lexer {
    class v1 {
        fsm _FSM;
        enum {
            StateStart,
            State_O,
            State_OK,
            StateIdentifier
        };
    public:
        v1();
        std::list<> tokenize(std::string InputString);
    };
}

#endif
