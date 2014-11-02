#ifndef CONSTR_SYNTAX_LEXER_GENERATOR_H
#define CONSTR_SYNTAX_LEXER_GENERATOR_H

#include "../util/fsm.h"
#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>

namespace constr {
    namespace syntax {
        typedef util::fsm::input_traits<char> gen_input_traits;
        class gen_token {
        public:
            enum value {
                TerminalWhitespace,
                TerminalTerminalIdentifier,
                TerminalSymbolIdentifier,
                TerminalAssign,
                TerminalAlternative,
                TerminalRegularExpression
            } symbol;
            gen_token(value Symbol, std::string Text = "") : symbol(Symbol), text(Text) {}
            std::string text;
            std::list<std::shared_ptr<gen_token>> children;
            operator std::string() {
                switch(symbol) {
                case TerminalWhitespace:
                    return "Whitespace";
                case TerminalAssign:
                    return ":=";
                case TerminalAlternative:
                    return "OR";
                case TerminalTerminalIdentifier:
                    return "TermId(" + text + ")";
                case TerminalSymbolIdentifier:
                    return "SymbId(" + text + ")";
                case TerminalRegularExpression:
                    return "RegExp("+ text + ")";
                }
            }
        };
        class gen_output_traits {
        public:
            typedef std::shared_ptr<gen_token> output_type;
            typedef std::list<output_type> return_type;
        };
        class gen_state_traits {
        public:
            enum value {
                Start = 0,
                Whitespace,
                Colon,
                Assign,
                Alternative,
                BeginTerminalIdentifier,
                TerminalIdentifier,
                BeginSymbolIdentifier,
                SymbolIdentifier,
                BeginRegularExpression,
                RegularExpression,
                Error,
                Final
            };
            static const value start_state, final_state, error_state;
            typedef value state_type;
        };
        const gen_state_traits::value gen_state_traits::start_state = gen_state_traits::Start;
        const gen_state_traits::value gen_state_traits::final_state = gen_state_traits::Final;
        const gen_state_traits::value gen_state_traits::error_state = gen_state_traits::Error;

        class gen_transition_traits : public util::fsm::transition_traits<gen_input_traits, gen_state_traits, std::list<gen_input_traits::input_type>> {
        public:
            static void initialize(std::map<gen_state_traits::state_type, transition_type>& States) {
                typedef gen_state_traits::state_type state;
                typedef std::list<gen_input_traits::input_type>::iterator iterator;
                States[gen_state_traits::Start] = [](const state& CurrentState, iterator Begin, iterator End) -> state {
                    const char Char = *Begin;
                    switch(Char) {
                    case gen_input_traits::end_of_stream:
                        return gen_state_traits::final_state;
                    case ' ':
                    case '\n':
                    case '\r':
                    case '\t':
                        return gen_state_traits::Whitespace;
                    case ':':
                        return gen_state_traits::Colon;
                    case '|':
                        return gen_state_traits::Alternative;
                    case '~':
                        return gen_state_traits::BeginRegularExpression;
                    default:
                        if(Char >= 'A' && Char <= 'Z') return gen_state_traits::BeginSymbolIdentifier;
                        else if(Char >= 'a' && Char <= 'z') return gen_state_traits::BeginTerminalIdentifier;
                        else return gen_state_traits::Error;
                    }
                };
                States[gen_state_traits::Colon] = [](const state& CurrentState, iterator Begin, iterator End) -> state {
                    switch(*Begin) {
                    case '=':
                        return gen_state_traits::Assign;
                    default:
                        return gen_state_traits::error_state;
                    }
                };
                States[gen_state_traits::BeginTerminalIdentifier] = [](const state& CurrentState, iterator Begin, iterator End) -> state {
                    const char Char = *Begin;
                    switch(Char) {
                    case ' ':
                    case '\n':
                    case '\r':
                    case '\t':
                    case gen_input_traits::end_of_stream:
                        return gen_state_traits::TerminalIdentifier;
                    default:
                        if((Char >= 'A' && Char <= 'Z') || (Char >= 'a' && Char <= 'z') || (Char >= '0' && Char <= '9') || Char == '_') return gen_state_traits::BeginTerminalIdentifier;
                        else return gen_state_traits::error_state;
                    }
                };
                States[gen_state_traits::BeginSymbolIdentifier] = [](const state& CurrentState, iterator Begin, iterator End) -> state {
                    const char Char = *Begin;
                    switch(Char) {
                    case ' ':
                    case '\n':
                    case '\r':
                    case '\t':
                    case gen_input_traits::end_of_stream:
                        return gen_state_traits::SymbolIdentifier;
                    default:
                        if((Char >= 'A' && Char <= 'Z') || (Char >= 'a' && Char <= 'z') || (Char >= '0' && Char <= '9') || Char == '_') return gen_state_traits::BeginSymbolIdentifier;
                        else return gen_state_traits::error_state;
                    }
                };
                States[gen_state_traits::BeginRegularExpression] = [](const state& CurrentState, iterator Begin, iterator End) -> state {
                    const char Char = *Begin;
                    switch(Char) {
                    case ' ':
                    case '\n':
                    case '\r':
                    case '\t':
                    case gen_input_traits::end_of_stream:
                        return gen_state_traits::RegularExpression;
                    default:
                        return gen_state_traits::BeginRegularExpression;
                    }
                };
                
            }
        };

        class gen_strategy {
            gen_output_traits::return_type _Result;
            std::string _Text;
        public:
            gen_strategy() {}
            void operator()(gen_state_traits::state_type& CurrentState, std::list<char>& InputBuffer, const gen_state_traits::state_type& NewState) { 
                if(NewState == gen_state_traits::final_state) {
                    CurrentState = NewState;
                    return;
                } else if (CurrentState == gen_state_traits::start_state) _Text = "";
                if(NewState == gen_state_traits::error_state) throw std::runtime_error("gen_strategy reached error_state");
                char Char = *InputBuffer.begin();
                switch(NewState) {
                case gen_state_traits::Whitespace:
                    _Result.insert(_Result.end(), std::shared_ptr<gen_token>(new gen_token(gen_token::TerminalWhitespace, std::string({Char, '\0'}))));
                    CurrentState = gen_state_traits::start_state;
                    break;
                case gen_state_traits::Assign:
                    _Result.insert(_Result.end(), std::shared_ptr<gen_token>(new gen_token(gen_token::TerminalAssign)));
                    CurrentState = gen_state_traits::start_state;
                    break;
                case gen_state_traits::BeginTerminalIdentifier:
                    _Text += std::string({Char, '\0'});
                    CurrentState = NewState;
                    break;
                case gen_state_traits::TerminalIdentifier:
                    _Result.insert(_Result.end(), std::shared_ptr<gen_token>(new gen_token(gen_token::TerminalTerminalIdentifier, _Text)));
                    CurrentState = gen_state_traits::start_state;
                    break;
                case gen_state_traits::BeginSymbolIdentifier:
                    _Text += std::string({Char, '\0'});
                    CurrentState = NewState;
                    break;
                case gen_state_traits::SymbolIdentifier:
                    _Result.insert(_Result.end(), std::shared_ptr<gen_token>(new gen_token(gen_token::TerminalSymbolIdentifier, _Text)));
                    CurrentState = gen_state_traits::start_state;
                    break;
                case gen_state_traits::BeginRegularExpression:
                    _Text += std::string({Char, '\0'});
                    CurrentState = NewState;
                    break;
                case gen_state_traits::RegularExpression:
                    _Result.insert(_Result.end(), std::shared_ptr<gen_token>(new gen_token(gen_token::TerminalRegularExpression, _Text)));
                    CurrentState = gen_state_traits::start_state;
                    break;
                case gen_state_traits::Alternative:
                    _Result.insert(_Result.end(), std::shared_ptr<gen_token>(new gen_token(gen_token::TerminalAlternative)));
                    CurrentState = gen_state_traits::start_state;
                    break;
                default:
                    CurrentState = NewState;
                }
                InputBuffer.erase(InputBuffer.begin());
            }
            gen_output_traits::return_type result() {
                return _Result;
            }
        };
        
        class gen_strategy_traits : public util::fsm::strategy_traits<gen_input_traits, gen_output_traits, gen_state_traits, std::list<char>, gen_strategy, 1> {};
        typedef util::fsm::automaton<gen_transition_traits, gen_strategy_traits> regular_expression_lexical_automaton;

        class lexer_generator {
        };
    }
}

#endif
