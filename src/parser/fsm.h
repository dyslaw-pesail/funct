#ifndef PARSER_FSM_H
#define PARSER_FSM_H

#include "../lexer/token.h"
#include <algorithm>
#include <array>
#include <map>
#include <memory>
#include <list>

namespace parser {
        class fsm {
        public:
            struct parse_node {
                parse_node();
                parse_node(lexer::token Token);
                parse_node(unsigned int Type, std::string Text);
                std::list<std::shared_ptr<parse_node>> Children;
                unsigned int Type;
                std::string Text;
            };
            typedef void (*func)();
        private:
            struct _transition {
                _transition(func Func = nullptr, unsigned int NewState = 0);
                func Func;
                unsigned int NewState;
            };
            std::map<unsigned int, std::array<_transition, lexer::token::count>> _StateArray;
            unsigned int _CurrentState,
                         _StartState;
            std::list<std::shared_ptr<parse_node>> _Stack, _InputBuffer;
        public:
            void add_state(unsigned int State, unsigned int Destination = 0, func Func = nullptr);
            void add_transition(unsigned int From, unsigned To, unsigned int Input, func Func);
            void set_start_state(unsigned int StartState);
            void reset();
            void feed(lexer::token Token);
            std::shared_ptr<parse_node> reduce();
            void reduce(std::shared_ptr<parse_node> Node);
            void shift();
            void error();
        };
}

#endif
