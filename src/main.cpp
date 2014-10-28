#include "lexer/fsm.h"

int main() {
    lexer::fsm::func f = &lexer::fsm::move_to;
    lexer::fsm *fsm = new lexer::fsm();
    (fsm->*f)(1);
    return 0;
}
