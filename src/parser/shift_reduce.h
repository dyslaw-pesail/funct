#ifndef PARSER_SHIFT_REDUCE_H
#define PARSER_SHIFT_REDUCE_H

#include <algorithm>
#include <list>
#include <map>
#include <memory>

namespace parser {
    template<class ST, class TT, class NT>
    class shift_reduce {
    public:
        typedef void (*transition_func)(shift_reduce *Parser, ST, const std::shared_ptr<NT>&);
        typedef void (*reduce_callback)(std::list<std::shared_ptr<NT>>&, ST&, const std::shared_ptr<NT>&);
    private:   
        struct _transition {
            _transition(transition_func Func = nullptr) : Func(Func) {}
            transition_func Func;
        };
        ST _CurrentState, _StartState;  
        std::list<std::shared_ptr<NT>> _Stack, _InputBuffer;
        std::map<ST, std::array<_transition, NT::space_size>> _StateArray;
    public:
        shift_reduce() {}
        template<class L>
        inline std::shared_ptr<NT> parse(L& List) {
            reset();
            std::for_each(L.begin(), L.end(), [](TT& Token) {
                _InputBuffer.push_back(std::shared_ptr<NT>(new NT(TT)));
            });
            while(!_InputBuffer.empty()) {
                _StateArray[_CurrentState][_InputBuffer.front()->to_uint()].Func(this, _CurrentState, *_InputBuffer.front());
            }
            return *_Stack.front();
        }
        inline void reset() {
            _CurrentState = _StartState;
        } 
        inline void set_start_state(ST StartState) {
            _StartState = StartState;
        }
        inline void add_state(ST State, transition_func Func = nullptr) {
            std::array<_transition, NT::space_size> &Array = _StateArray[State] = std::array<_transition, NT::space_size>();
            std::for_each(Array.begin(), Array.end(), [&](_transition& T) { T.Func = Func; });
        }        
        inline void add_transition(ST From, unsigned int Input, transition_func Func) {
            _StateArray[From][Input].Func = Func;
        }
        inline void reduce(reduce_callback Callback) {
            std::shared_ptr<NT> NewNode(new NT());
            ST NewState;
            Callback(_Stack, NewState, NewNode);
            _InputBuffer.push_front(NewNode);
            _CurrentState = NewState;
        }
        inline void shift(ST NewState, const std::shared_ptr<NT>& NewNode) {
            _Stack.push_back(NewNode);
            _CurrentState = NewState;
        }
    };
}

#endif
