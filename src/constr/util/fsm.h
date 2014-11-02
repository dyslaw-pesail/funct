#ifndef UTIL_FSM_H
#define UTIL_FSM_H

#include <iostream>
#include <istream>
#include <iterator>
#include <map>
#include <stdexcept>

namespace util {
    namespace fsm {
        template<class InputType>
        class input_traits {};
        template<>
        class input_traits<char> {
        public:
            class forward_iterator : public std::iterator<char, std::forward_iterator_tag> {
                uint32_t _Position;
            public:
                forward_iterator(uint32_t Position = 0) : _Position(Position) {}
                forward_iterator(const forward_iterator& It) : _Position(It._Position) {}
                forward_iterator& operator++() {
                    if(_Position < 256) ++_Position;
                    return *this;
                }
                inline forward_iterator operator++(int) {
                   return _Position < 256 ? forward_iterator(_Position++) : *this;  
                }
                inline forward_iterator& operator=(const forward_iterator& It) {
                    _Position = It._Position;
                }
                inline bool operator==(const forward_iterator& It) const {
                    return It._Position == _Position;
                }
                inline bool operator!=(const forward_iterator& It) const {
                    return !(It == *this);
                }
                inline char operator*() {
                    if(_Position < 256) return char(_Position);
                    else throw std::out_of_range("input_traits<char>::forward_iterator out of range");
                }
            };
            typedef char input_type;
            typedef forward_iterator iterator;
            static iterator begin() {
                return forward_iterator();
            }
            static iterator end() {
                return forward_iterator(256);
            }
            static const char end_of_stream;
        };
        const char input_traits<char>::end_of_stream = '\0';
        template<class InputTraits, 
                 class StateTraits, 
                 class Container>
        class transition_traits {
        public:
            typedef typename StateTraits::state_type (*transition_type)(const typename StateTraits::state_type&, 
                                                                        typename Container::iterator, typename Container::iterator);
        };
        template<class InputTraits,
                 class OutputTraits,
                 class StateTraits,
                 class Container,
                 class Strategy,
                 unsigned int LookAhead>
        class strategy_traits {
        public:
            typedef InputTraits input_traits;
            typedef OutputTraits output_traits;
            typedef StateTraits state_traits;
            typedef typename OutputTraits::output_type return_type;
            typedef Strategy strategy_type;
            typedef Container input_container_type;
            static const unsigned int look_ahead;
        };
        template<class IT, class OT, class ST, class C, class S, unsigned int LA>
        const unsigned int strategy_traits<IT, OT, ST, C, S, LA>::look_ahead = LA;

        template<class TransitionTraits,
                 class StrategyTraits>
        class automaton {
            std::map<typename StrategyTraits::state_traits::state_type, 
                     typename TransitionTraits::transition_type> _States;
        public:
            automaton() {
                TransitionTraits::initialize(_States);
            }
            typename StrategyTraits::output_traits::return_type
            operator()(std::basic_istream<typename StrategyTraits::input_traits::input_type>& Stream) {
                typename StrategyTraits::strategy_type Strategy;
                typename StrategyTraits::input_container_type InputBuffer;
                while(Stream.good()) {
                    typename StrategyTraits::input_traits::input_type InputSymbol;
                    Stream >> InputSymbol;
                    InputBuffer.insert(InputBuffer.end(), InputSymbol);
                }
                InputBuffer.insert(InputBuffer.end(), StrategyTraits::input_traits::end_of_stream);
                typename StrategyTraits::state_traits::state_type State = StrategyTraits::state_traits::start_state;
                while(InputBuffer.size() >= StrategyTraits::look_ahead && State != StrategyTraits::state_traits::final_state) {
                    Strategy(State, InputBuffer, _States[State](State, InputBuffer.begin(), InputBuffer.end()));
                }
                return Strategy.result();
            }
        };
    }
}

#endif
