#ifndef CONSTR_SYNTAX_REGULAR_EXPRESSION_PARSER_H
#define CONSTR_SYNTAX_REGULAR_EXPRESSION_PARSER_H

#include "../util/fsm.h"

namespace constr {
    namespace syntax {
        typedef util::fsm::input_traits<char> re_lex_input_traits;
        class re_token {
        public:
            enum value {
               TerminalUpperLetter,
               TerminalLowerLetter,
               TerminalDigit,
               TerminalBackslash
               TerminalLiteral
               TerminalLeftParenthesis,
               TerminalRightParenthesis,
               TerminalLeftSquareBracket,
               TerminalRightSquareBracket,
               TerminalLeftCurlyBracket,
               TerminalRightCurlyBracket,
               TerminalAlternative,
               TerminalStart,
               TerminalPlus,
               TerminalRange,
               TerminalEndOfStream
            } symbol;
            re_token(value Symbol, char Text = '\0') : symbol(Symbol), text(Text) {}
            char text;
            std::list<std::shared_ptr<re_token>> children;
        };
        class re_lex_output_traits {
        public:
            typedef std::shared_ptr<re_token> output_type;
            typedef std::list<output_type> return_type
        };
        class re_lex_state_traits {
        public:
            enum value {
                Start = 0,
                Error,
                Final
            }; 
            typedef value state_type;
            static const value start_state, error_state, final_state;
        };
        const re_lex_state_traits::value re_lex_state_traits::start_state = re_lex_state_traits::Start;
        const re_lex_state_traits::value re_lex_state_traits::start_state = re_lex_state_traits::Error;
        const re_lex_state_traits::value re_lex_state_traits::start_state = re_lex_state_traits::Final;
        class re_lex_transition_traits : public util::fsm::transition_traits<re_lex_input_traits, r_lex_state_traits, std::list<re_lex_input_traits::input_typee>> {
        public:
            static void initialize(std::map<re_lex_state_traits::state_type, transition_type>& States) {
                typedef re_lex_state_traits::state_type state;
                typedef std::list<re_lex_input_traits::input_type>::iterator iterator;

            }
        };

        typedef util::fsm::automaton<re_transition_traits, re_strategy_traits> regular_expression_parser;
    }
}

#endif
