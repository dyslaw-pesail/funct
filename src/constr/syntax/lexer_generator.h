#ifndef CONSTR_SYNTAX_LEXER_GENERATOR_H
#define CONSTR_SYNTAX_LEXER_GENERATOR_H

#include "../../util/fsm.h"

namespace constr {
    namespace syntax {
    
        typedef util::fsm::input_traits<char> re_input_traits;
        
        class re_output_symbol {
        public:
            enum value {
                TerminalLeftParenthesis,
                TerminalRightParenthesis,
                TerminalLowerAlpha,
                TerminalUpperAlpha,
                TerminalDigit,
                TerminalMinus,
                TerminalPlus,
                TerminalStar,
                TerminalAlternative,
                TerminalLiteral,
                SymbolAlpha,
                SymbolAlphaNumeric,
                SymbolAlphaRange,
                SymbolNumericRange,
                SymbolExpression
            };
            std::string text;
            std::list<std::shared_ptr<re_output_symbol>> children;
        };
        class re_output_traits {
        public:
            typedef std::shared_pointer<re_output_symbol> output_type;
            typedef std::list<output_type> return_type;
        };
        class re_state_traits {
        public:
            enum value {
                Start = 0,
                Final
            };
            static const value start_state = Start,
                               final_state = Final;
            typedef value state_type;
        };

        class re_transition_traits : public util::fsm::transition_traits<re_input_traits, re_state_traits, std::list<re_input_traits::input_type>> {
        public:
            static void initialize(std::map<re_state_traits::state_type, transition_type>&);
        }

        class re_strategy {};
        class re_strategy_traits : public util::fsm::strategy_traits<re_input_traits, re_output_traits, re_state_traits, std::list<char>, re_strategy, 1> {};

        template<re_transition_traits, re_strategy_trais>
        class regular_expression_automaton {
        };

        class lexer_generator {
        };
    }
}

#endif
