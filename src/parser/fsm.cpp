#include "fsm.h"

namespace parser {
    fsm::_transition::_transition(fsm::func Func, unsigned int NewState) {
        this->Func = Func;
        this->NewState = NewState;
    }

    fsm::parse_node::parse_node() {}
    fsm::parse_node::parse_node(unsigned int Type, std::string Text) {
        this->Type = Type;
        this->Text = Text;
    }
    fsm::parse_node::parse_node(lexer::token Token) {
        parse_node(Token.get_type(), Token.get_text());
    }


    void fsm::add_state(unsigned int State, unsigned int Destination, fsm::func Func) {
        std::array<_transition, lexer::token::count> Array;
        std::for_each(Array.begin(), Array.end(), 
        [&](_transition& T) {
            T.Func = Func;
            T.NewState =  Destination;
        });
        _StateArray[State] = Array;
    }

    void fsm::add_transition(unsigned int From, unsigned int To, unsigned int Input, fsm::func Func) {
        _StateArray[From][Input] = _transition(Func, To);
    }

    void fsm::set_start_state(unsigned int StartState) {
        _StartState = StartState;
    }

    void fsm::reset() {
        _InputBuffer.clear();
        _Stack.clear();
        _CurrentState = _StartState;
    }

    void fsm::feed(lexer::token Token) {
        _InputBuffer.push_back(std::shared_ptr<parse_node>(new parse_node(Token)));
        while(!_InputBuffer.empty()) {
            unsigned int Current = _InputBuffer.front()->Type;
            _transition& T = _StateArray[_CurrentState][Current];
            if(T.Func != nullptr) T.Func();
            else throw new std::string("Error: nullptr in T.Func!\n");
        }
    }

    void fsm::shift() {
        std::shared_ptr<parse_node> Node = _InputBuffer.front();
        _InputBuffer.pop_front();
        _Stack.push_back(Node);
    }

    std::shared_ptr<fsm::parse_node> fsm::reduce() {
        std::shared_ptr<fsm::parse_node> Node = _Stack.back();
        _Stack.pop_back();
        return Node;
    }

    void fsm::reduce(std::shared_ptr<fsm::parse_node> Node) {
        _InputBuffer.push_front(Node);
    }
}
