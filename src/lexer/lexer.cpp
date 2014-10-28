#include "lexer.h"

namespace lexer {
    v1::v1() {
        _FSM.add_state(StateStart);
        _FSM.set_start_state(StateStart);
        _FSM.add_state(State_O);
        _FSM.add_state(State_OK);
        _FSM.add_state(StateIdentifier);
        _FSM.add_transition(StateStart, StateIdentifier, fsm::alpha, &fsm::move_to);
        _FSM.add_transition(StateIdentifier, StateIdentifier, fsm::alphanumeric, &fsm::move_to);
        _FSM.add_transition(StateIdentifier, StateStart, fsm::whitespace, &fsm::emit, StateIdentifier);
        _FSM.add_transition(StateIdentifier, StateStart, fsm::delimiter, &fsm::emit, StateIdentifier);
        _FSM.add_transition(StateStart, State_O, 'o', &fsm::move_to);
        _FSM.add_transition(State_O, StateIdentifier, fsm::alphanumeric, &fsm::move_to);
        _FSM.add_transition(State_O, State_OK, 'k', &fsm::move_to);
        _FSM.add_transition(State_O, StateStart, fsm::whitespace, &fsm::emit, StateIdentifier);
        _FSM.add_transition(State_O, StateStart, fsm::delimiter, &fsm::emit, StateIdentifier);
        _FSM.add_transition(State_OK, StateIdentifier, fsm::alphanumeric, &fsm::move_to);
        _FSM.add_transition(State_OK, StateStart, fsm::whitespace, &fsm::emit, State_OK);
        _FSM.add_transition(State_OK, StateStart, fsm::delimiter, &fsm::emit, State_OK);
    }
    
    void v1::tokenize(std::string InputString) {
        _FSM.reset();
        std::for_each(InputString.begin(), InputString.end(), [&](char C) { _FSM.feed(C); });
        _FSM.feed('\0');
    }
}
