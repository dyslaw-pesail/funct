#include "fsm.h"

namespace lexer {

    fsm::_pair::_pair(func Func = nullptr, unsigned int Arg = 0, unsigned int NextState = 0) {
        this->Arg = Arg;
        this->Func = Func;
        this->NextState = NextState;
    }

    void fsm::add_state(unsigned int State) {
        _StateArray[State] = std::array<_pair, 256>();
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

    void fsm::move_to(unsigned int NewState) {
        _CurrentState = NewState;
    }
    void fsm::emit(unsigned int Token) {
        
    }
}
