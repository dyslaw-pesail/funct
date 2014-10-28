#ifndef LEXER_FSM_H
#define LEXER_FSM_H

#include <map>
#include <array>

namespace lexer {
    class fsm {
    public:
        typedef void (fsm::*func)(unsigned int);
    private:
        unsigned int _CurrentState;
        struct _pair {
            _pair(func Func, unsigned int Arg, unsigned int NextState);
            func Func;
            unsigned int Arg, NextState;
        };
        std::map<unsigned int, std::array<_pair, 256>> _StateArray;
    public: 
        enum transition_range {
            eof = 0,
            alpha,
            numeric,
            alphanumeric,
            upper_alpha,
            lower_alpha,
            whitespace
        };

        void add_state(unsigned int State);
        void add_transition(unsigned int From, unsigned int To, char Input, func Func, unsigned int Arg);
        void add_transition(unsigned int From, unsigned int To, char InputFrom, char InputTo, func Func, unsigned int Arg);
        void add_transition(unsigned int From, unsigned int To, transition_range Range, func Func, unsigned int Arg);

        void move_to(unsigned int NewState);
        void emit(unsigned int TokenId);    
    };
}

#endif
