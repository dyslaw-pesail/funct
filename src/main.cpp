#include "constr/util/fsm.h"
#include <list>
#include <map> 
#include <string>
using namespace std;
using namespace util::fsm;

class state_traits {
public:
    enum value {
        Start = 0,
        Identifier,
        Keyword_New,
        Final
    };
    static const value start_state = Start,
                       final_state = Final;
    typedef value state_type;
};

class output_traits {
public:
    typedef string output_type;
    typedef list<string> return_type;
};

class my_transition_traits : public transition_traits<input_traits<char>, state_traits, list<char>> {
public:
    static void initialize(map<state_traits::state_type, transition_type>&) {}
};

class strategy {
public:
    strategy() {}
    void operator()(state_traits::value&, list<char>&, const state_traits::value&) {}
    output_traits::return_type result() {
        return output_traits::return_type();
    }
};

class my_strategy_traits : public strategy_traits<input_traits<char>,
                                                  output_traits,
                                                  state_traits,
                                                  list<char>,
                                                  strategy,
                                                  1> {};


automaton<my_transition_traits, my_strategy_traits> A;

int main() {
    return 0;
}
