#include "fsm.h"
#include <iostream>

namespace lexer {

    fsm::_pair::_pair(func Func = nullptr, unsigned int Arg = 0, unsigned int NextState = 0) {
        this->Arg = Arg;
        this->Func = Func;
        this->NextState = NextState;
    }

    void fsm::add_state(unsigned int State) {
        std::array<_pair, 256> Array;
        std::for_each(Array.begin(), Array.end(), [](_pair& P) { P.Func = &fsm::error; });
        _StateArray[State] = Array;
    } 

    void fsm::add_transition(unsigned int From, unsigned int To, char Input, fsm::func Func, unsigned int Arg) {
        _StateArray[From][(unsigned char)Input] = _pair(Func, Arg, To);
    }
    void fsm::add_transition(unsigned int From, unsigned int To, char InputFrom, char InputTo, fsm::func Func, unsigned int Arg) {
        std::array<_pair, 256>& A = _StateArray[From];
        for(unsigned char I = (unsigned char)InputFrom; I < (unsigned char)InputTo; ++I) {
            A[I] = _pair(Func, Arg, To);
        }
    }
    void fsm::add_transition(unsigned int From, unsigned int To, fsm::transition_range Range, fsm::func Func, unsigned int Arg) {
        static std::array<char, 5> _Whitespaces = { '\n', ' ', '\t', '\t', '\0' };
        static std::array<char, 9> _Delimiters = { '[', ']', '(', ')', ',', '-', '<', '>', '=' };
        switch(Range) {
        case fsm::lower_alpha:
            add_transition(From, To, 'a', 'z', Func, Arg);
            break;
        case fsm::upper_alpha:
            add_transition(From, To, 'A', 'Z', Func, Arg);
            break;
        case fsm::alpha:
            add_transition(From, To, fsm::lower_alpha, Func, Arg);
            add_transition(From, To, fsm::upper_alpha, Func, Arg);
            break;
        case fsm::numeric:
            add_transition(From, To, '0', '9', Func, Arg);
            break;
        case fsm::alphanumeric:
            add_transition(From, To, fsm::alpha, Func, Arg);
            add_transition(From, To, fsm::numeric, Func, Arg);
            break;
        case fsm::whitespace:
            add_transition<std::tuple_size<decltype(_Whitespaces)>::value>(From, To, _Whitespaces, Func, Arg);
            break;
        case fsm::delimiter:
            add_transition<std::tuple_size<decltype(_Delimiters)>::value>(From, To, _Delimiters, Func, Arg);
            break;
        default:
            return;
        }
    } 

    void fsm::feed(char Input) {
        _LastInput = Input;
        if(_StateArray.find(_CurrentState) != _StateArray.end()) {
           auto A = _StateArray[_CurrentState];
           auto P = A[Input];
           if(P.Func != nullptr) {
               (this->*(P.Func))(P.NextState, P.Arg);
           }
        } else throw new std::string("Error: Invalid FSM._CurrentState\n");
        ++_CurrentPosition;
    }

    void fsm::_refeed() {
        feed(_LastInput);
    }

    void fsm::reset() {
        _CurrentState = _StartState;
        _CurrentPosition = 0;
    }

    void fsm::move_to(unsigned int NewState, unsigned int) {
        _CurrentState = NewState;
        if(NewState != _StartState) _InputBuffer.push_back(_LastInput);
        else _InputBuffer.clear();
    }
    void fsm::move_to_repeat(unsigned int NewState, unsigned int) {
        _CurrentState = NewState;
        _refeed();
    }
    void fsm::emit(unsigned int, unsigned int TokenType) {
        /* Print token */
        std::string String(_InputBuffer.size(), ' ');
        int I = 0; 
        std::for_each(_InputBuffer.begin(), _InputBuffer.end(), [&](char C) { String[I++] = C; });
        _CurrentReceiver->receive_token(TokenType, String);
        _CurrentState = _StartState;
        _InputBuffer.clear();
        _refeed();
    }

    void fsm::set_receiver(fsm::receiver *Receiver) {
        _CurrentReceiver = Receiver;
    }
    void fsm::error(unsigned int, unsigned int Arg) {
        std::cout << "Error at position: " << _CurrentPosition << std::endl;
    }
}
