#ifndef LEXER_FSM_H
#define LEXER_FSM_H

#include <map>
#include <array>
#include <algorithm>
#include <string>
#include <vector>
#include <memory>

namespace lexer {
    class fsm {
    public:
        typedef void (fsm::*func)(unsigned int, unsigned int);
        class receiver {
        public:
            virtual void receive_token(unsigned int TokenType, std::string TokenText)=0;
        };
    private:
        unsigned int _CurrentState;
        receiver *_CurrentReceiver;
        struct _pair {
            _pair(func Func, unsigned int Arg, unsigned int NextState);
            func Func;
            unsigned int Arg, NextState;
        };
        std::map<unsigned int, std::array<_pair, 256>> _StateArray;
        char _LastInput;
        unsigned int _StartState;
        std::vector<char> _InputBuffer;
        unsigned int _CurrentPosition;
        void _refeed();
    public: 
        enum transition_range {
            eof = 0,
            alpha,
            numeric,
            alphanumeric,
            upper_alpha,
            lower_alpha,
            whitespace,
            delimiter
        };

        void reset();
        inline void set_start_state(unsigned int StartState) { _StartState = StartState; }
        void add_state(unsigned int State);
        void add_transition(unsigned int From, unsigned int To, char Input, func Func, unsigned int Arg = 0);
        void add_transition(unsigned int From, unsigned int To, char InputFrom, char InputTo, func Func, unsigned int Arg = 0);
        void add_transition(unsigned int From, unsigned int To, transition_range Range, func Func, unsigned int Arg = 0);
        template<int N>
        void add_transition(unsigned int From, unsigned int To, std::array<char, N> Array, func Func, unsigned int Arg = 0) {
            std::for_each(Array.begin(), Array.end(), [&](char Char) { add_transition(From, To, Char, Func, Arg); });    
        }
        void set_receiver(receiver *Receiver);

        void feed(char Input);
        void move_to(unsigned int NewState, unsigned int);
        void move_to_repeat(unsigned int NewState, unsigned int);
        void emit(unsigned int, unsigned int TokenId);    
        void error(unsigned int, unsigned int ErrorId);
    };
}

#endif
