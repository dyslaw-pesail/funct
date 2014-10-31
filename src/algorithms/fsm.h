#ifndef ALGORITHMS_FSM_H
#define ALGORITHMS_FSM_H

#include <istream>
#include <iterator>
#include <list>
#include <map>
#include <stdexcept>

namespace algo {
    namespace fsm {
        /* === state_traits === */
        template<class State>
        class state_traits {};        

        /* === input_traits === */
        template<class Input>
        class input_traits {};
        
        template<>
        class input_traits<char> {
        public:
            class _forward_iterator : public std::iterator<char, std::forward_iterator_tag> {
                friend input_traits<char>;
                size_t _position;
                _forward_iterator(size_t position) : _position(position) {}
            public:
                _forward_iterator() : _position(0) {}
                _forward_iterator(const _forward_iterator& I) {
                    _position = I._position;
                }
                inline _forward_iterator& operator++() {
                    if(_position < 256) ++_position;
                    return *this;
                }
                inline _forward_iterator operator++(int) {
                    if(_position < 256) return _forward_iterator(_position++);
                    else return *this;
                }
                inline char operator*() {
                    if(_position < 256) return char(_position);
                    else throw new std::out_of_range("Iterator is not dereferencable");
                }
                inline char* operator->() {
                    throw new std::runtime_error("Iterator is not dereferencable");
                }
                inline bool operator==(const _forward_iterator& I) {
                    return _position == I._position;
                }
                inline bool operator!=(const _forward_iterator& I) {
                    return !(*this == I);
                }
            };
            /* Input traits interface */
            typedef char value_type; 
            typedef _forward_iterator iterator;
            inline static iterator begin() {
                return _forward_iterator(0);
            }
            inline static iterator end() {
                return _forward_iterator(256);
            }
            static const char end_symbol = '\0';
        };

        /* === output_traits === */
        template<class Output>
        class output_traits {};

        /* === strategy_traits === */
        template<class StateTraits, class InputTraits, class OutputTraits, int LookAhead>
        class strategy_traits {};

        /* Specialization for default 1- and 2-lookahead strategies */
        template<class StateTraits, class InputTraits, class OutputTraits>
        class strategy_traits<StateTraits, InputTraits, OutputTraits, 1> {
        public:
            typedef StateTraits state_traits_type;
            typedef InputTraits input_traits_type;
            typedef OutputTraits output_traits_type;
            typedef std::list<input_type> container_type;
            typedef typename container_type::iterator iterator_type;
            typedef state_type (*transition_type)(const state_type&, const input_type&);
            static const int LookAhead = 1;
        };
        template<class StateTraits, class InputTraits>
        class strategy_traits<StateTraits, InputTraits, 2> {
        public:
            typedef StateTraits state_traits_type;
            typedef InputTraits input_traits_type;
            typedef OutputTraits output_traits_type;
            typedef std::list<input_type> container_type;
            typedef typename container_type::iterator iterator_type;
            typedef state_type (*transition_type)(const state_type&, const input_type&, const input_type&);
            static const int LookAhead = 2;
        };

        template<class Traits>
        class _base_strategy {
        public:
            _base_strategy(typename Traits::transition_type Default) {
                std::for_each(Traits::state_traits_type::begin(), Traits::state_traits_type::end(),
                    [&](const typename Traits::state_traits_type::value_type& State) {
                        _Transitions[State] = Default;
                    }
                );
            }
        protected:
            std::map<typename Traits::state_traits_type::value_type, typename Traits::transition_type> _Transitions;
        };
        /* Template to be inherited from by strategy implementations */
        template<int LookAhead, class Traits>
        class base_strategy : protected _base_strategy<Traits> {};
        template<class Traits>
        class base_strategy<1, Traits> : protected _base_strategy<Traits> {
        public:
            base_strategy(typename Traits::transition_type Default = nullptr) : _base_strategy(Default) {}
            inline typename Traits::state_traits_type::value_type
            operator()(typename Traits::state_traits_type::value_type& CurrentState, typename Traits::iterator_type Begin, bool& ShouldRemove) {
                return this->_Transitions[CurrentState](CurrentState, *Begin);
            }
        };
        template<class Traits>
        class base_strategy<2, Traits> : protected _base_strategy<Traits> {
        public:
            base_strategy(typename Traits::transition_type Default = nullptr) : _base_strategy(Default) {}
            inline typename Traits::state_traits_type::value_type
            operator()(typename Traits::state_traits_type& CurrentState, typename Traits::iterator_type Begin, bool& ShouldRemove) {
                typename Traits::input_traits_type::value_type I1 = *Begin[0];
                typename Traits::input_traits_type::value_type I2 = *Begin[1];
                return this->_Transitions[CurrentState](CurrentState, I1, I2); 
            }
        };

        template<class Traits>
        class strategy {};

        template<class State, class Input, int LookAhead,
                 class StateTraits = state_traits<State>, 
                 class InputTraits = input_traits<Input>,
                 class StrategyTraits = strategy_traits<StateTraits, InputTraits, LookAhead>,
                 class Strategy = strategy<StrategyTraits>>
        class fsm {
        public:
            inline  operator()(std::basic_istream<typename InputTraits::value_type>& Stream) {
                Strategy Current;
                typename StrategyTraits::container InputBuffer;
                Current.initialize(InputBuffer);
                while(Stream.good()) {
                    typename InputTraits::value_type Input;
                    Stream >> Input;
                    InputBuffer.insert(InputBuffer.end(), Input);
                }          
                InputBuffer.insert(InputBuffer.end(), InputTraits::end_symbol);
                typename StateTraits::value_type CurrentState = StateTraits::Start;
                while(InputBuffer.size() >= LookAhead) {
                    bool ShouldRemove = true;
                    CurrentState = Current(CurrentState, InputBuffer.begin(), ShouldRemove);                   
                    if(ShouldRemove) InputBuffer.erase(InputBuffer.begin());
                }
                return Current.finalize();   
            }
        };
    }
}

#endif
