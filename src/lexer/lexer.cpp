#include "lexer.h"

namespace lexer {
    v1::v1() {
        _FSM.add_state(state_start);
        _FSM.set_start_state(state_start);
        _FSM.add_state(state_identifier);
        _FSM.add_state(state_variable);
        _FSM.add_state(state_constant);
        _FSM.add_state(state_constant_0);
        _FSM.add_state(state_n);
        _FSM.add_state(state_ne);
        _FSM.add_state(state_new);
        _FSM.add_state(state_bracket_parent_open);    
        _FSM.add_state(state_bracket_parent_close);
        _FSM.add_state(state_bracket_square_open);
        _FSM.add_state(state_bracket_square_close);
        _FSM.add_state(state_bracket_angle_open);
        _FSM.add_state(state_bracket_angle_close);
        _FSM.add_state(state_comma);
        _FSM.add_state(state_minus);
        _FSM.add_state(state_arrow);
        _FSM.add_state(state_match);
        _FSM.add_transition(state_start, state_start, fsm::whitespace, &fsm::move_to);
        _FSM.add_transition(state_start, state_identifier, fsm::lower_alpha, &fsm::move_to);
        _FSM.add_transition(state_start, state_variable, fsm::upper_alpha, &fsm::move_to);
        _FSM.add_transition(state_start, state_constant, fsm::numeric, &fsm::move_to);
        _FSM.add_transition(state_start, state_constant_0, '0', &fsm::move_to);
        _FSM.add_transition(state_start, state_n, 'n', &fsm::move_to);
        _FSM.add_transition(state_start, state_bracket_parent_open, '(', &fsm::move_to);
        _FSM.add_transition(state_start, state_bracket_parent_close, ')', &fsm::move_to);
        _FSM.add_transition(state_start, state_bracket_square_open, '[', &fsm::move_to);
        _FSM.add_transition(state_start, state_bracket_square_close, ']', &fsm::move_to);
        _FSM.add_transition(state_start, state_bracket_angle_open, '<', &fsm::move_to);
        _FSM.add_transition(state_start, state_bracket_angle_close, '>', &fsm::move_to);
        _FSM.add_transition(state_start, state_comma, ',', &fsm::move_to);
        _FSM.add_transition(state_start, state_minus, '-', &fsm::move_to);
        _FSM.add_transition(state_start, state_match, '=', &fsm::move_to);
        _FSM.add_transition(state_identifier, state_identifier, fsm::alphanumeric, &fsm::move_to);
        _FSM.add_transition(state_identifier, state_identifier, '_', &fsm::move_to);
        _FSM.add_transition(state_identifier, state_start, fsm::whitespace, &fsm::emit, token::identifier);
        _FSM.add_transition(state_identifier, state_start, fsm::delimiter, &fsm::emit, token::identifier);
        _FSM.add_transition(state_variable, state_variable, fsm::alphanumeric, &fsm::move_to);
        _FSM.add_transition(state_variable, state_variable, '_', &fsm::move_to);
        _FSM.add_transition(state_variable, state_start, fsm::whitespace, &fsm::emit, token::variable);
        _FSM.add_transition(state_variable, state_start, fsm::delimiter, &fsm::emit, token::variable);
        _FSM.add_transition(state_constant, state_constant, fsm::numeric, &fsm::move_to);
        _FSM.add_transition(state_constant, state_start, fsm::whitespace, &fsm::emit, token::constant);
        _FSM.add_transition(state_constant, state_start, fsm::delimiter, &fsm::emit, token::constant);
        _FSM.add_transition(state_constant_0, state_start, fsm::whitespace, &fsm::emit, token::constant);
        _FSM.add_transition(state_constant_0, state_start, fsm::delimiter, &fsm::emit, token::constant);
        _FSM.add_transition(state_start, state_n, 'n', &fsm::move_to);
        _FSM.add_transition(state_n, state_identifier, '\0', '\255', &fsm::move_to_repeat);
        _FSM.add_transition(state_n, state_ne, 'e', &fsm::move_to);
        _FSM.add_transition(state_ne, state_identifier, '\0', '\255', &fsm::move_to_repeat);
        _FSM.add_transition(state_ne, state_new, 'w', &fsm::move_to);
        _FSM.add_transition(state_new, state_identifier, fsm::alphanumeric, &fsm::move_to_repeat);
        _FSM.add_transition(state_new, state_start, fsm::whitespace, &fsm::emit, token::keyword_new);
        _FSM.add_transition(state_new, state_start, fsm::delimiter, &fsm::emit, token::keyword_new);
        _FSM.add_transition(state_bracket_parent_open, state_start, fsm::alphanumeric, &fsm::emit, token::bracket_parent_open);
        _FSM.add_transition(state_bracket_parent_open, state_start, fsm::whitespace, &fsm::emit, token::bracket_parent_open);
        _FSM.add_transition(state_bracket_parent_open, state_start, fsm::delimiter, &fsm::emit, token::bracket_parent_open);
        _FSM.add_transition(state_bracket_parent_close, state_start, fsm::alphanumeric, &fsm::emit, token::bracket_parent_close);
        _FSM.add_transition(state_bracket_parent_close, state_start, fsm::whitespace, &fsm::emit, token::bracket_parent_close);
        _FSM.add_transition(state_bracket_parent_close, state_start, fsm::delimiter, &fsm::emit, token::bracket_parent_close);
        _FSM.add_transition(state_bracket_square_open, state_start, fsm::alphanumeric, &fsm::emit, token::bracket_square_open);
        _FSM.add_transition(state_bracket_square_open, state_start, fsm::whitespace, &fsm::emit, token::bracket_square_open);
        _FSM.add_transition(state_bracket_square_open, state_start, fsm::delimiter, &fsm::emit, token::bracket_square_open);
        _FSM.add_transition(state_bracket_square_close, state_start, fsm::alphanumeric, &fsm::emit, token::bracket_square_close);
        _FSM.add_transition(state_bracket_square_close, state_start, fsm::whitespace, &fsm::emit, token::bracket_square_close);
        _FSM.add_transition(state_bracket_square_close, state_start, fsm::delimiter, &fsm::emit, token::bracket_square_close);
        _FSM.add_transition(state_bracket_angle_open, state_start, fsm::alphanumeric, &fsm::emit, token::bracket_angle_open);
        _FSM.add_transition(state_bracket_angle_open, state_start, fsm::whitespace, &fsm::emit, token::bracket_angle_open);
        _FSM.add_transition(state_bracket_angle_open, state_start, fsm::delimiter, &fsm::emit, token::bracket_angle_open);
        _FSM.add_transition(state_bracket_angle_close, state_start, fsm::alphanumeric, &fsm::emit, token::bracket_angle_close);
        _FSM.add_transition(state_bracket_angle_close, state_start, fsm::whitespace, &fsm::emit, token::bracket_angle_close);
        _FSM.add_transition(state_bracket_angle_close, state_start, fsm::delimiter, &fsm::emit, token::bracket_angle_close);
        _FSM.add_transition(state_comma, state_start, fsm::alphanumeric, &fsm::emit, token::comma);
        _FSM.add_transition(state_comma, state_start, fsm::whitespace, &fsm::emit, token::comma);
        _FSM.add_transition(state_comma, state_start, fsm::delimiter, &fsm::emit, token::comma);
        _FSM.add_transition(state_minus, state_arrow, '>', &fsm::move_to);
        _FSM.add_transition(state_arrow, state_start, fsm::alphanumeric, &fsm::emit, token::arrow);
        _FSM.add_transition(state_arrow, state_start, fsm::whitespace, &fsm::emit, token::arrow);
        _FSM.add_transition(state_arrow, state_start, fsm::delimiter, &fsm::emit, token::arrow);
        _FSM.add_transition(state_match, state_start, fsm::alphanumeric, &fsm::emit, token::match);
        _FSM.add_transition(state_match, state_start, fsm::whitespace, &fsm::emit, token::match);
        _FSM.add_transition(state_match, state_start, fsm::delimiter, &fsm::emit, token::match);
    }
    
    std::list<token> v1::tokenize(std::string InputString) {
        _CurrentTokenList.clear();
        _FSM.set_receiver(this);
        _FSM.reset();
        std::for_each(InputString.begin(), InputString.end(), [&](char C) { _FSM.feed(C); });
        _FSM.feed('\0');
        _CurrentTokenList.push_back(token(token::end_of_file, "<EOF>"));
        return _CurrentTokenList;
    }

    void v1::receive_token(unsigned int TokenType, std::string TokenText) {
        _CurrentTokenList.push_back(token(TokenType, TokenText));      
    }
}
