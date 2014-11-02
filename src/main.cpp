#include "constr/util/fsm.h"
#include "constr/syntax/lexer_generator.h"
#include <fstream>
#include <list>
using namespace constr::syntax;

int main(int argc, char *argv[]) {
    if(argc>1) {
        std::ifstream stream(argv[1], std::ios::in | std::ios::binary);
        stream.unsetf(stream.skipws);
        regular_expression_lexical_automaton RELA;
        gen_output_traits::return_type Result = RELA(stream);
        std::for_each(Result.begin(), Result.end(), [&](const std::shared_ptr<gen_token>& Token) {
            std::cout << std::string(*Token) << std::endl;
        });
    }
    return 0;
}
