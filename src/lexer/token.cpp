#include "token.h"

namespace lexer {
    token::token(unsigned int Type, std::string Text) {
        _Type = Type;
        _Text = Text;
    }

    unsigned int token::get_type() {
        return _Type;
    }

    std::string token::get_text() {
        return _Text;
    }

    std::string token::repr() {
        switch(_Type) {
        case identifier:
            return "id-" + _Text;
        case constant:
            return "const-" + _Text;
        case variable:
            return "var-" + _Text;
        case keyword_new:
            return "new";
        default:
            return "\"" + _Text + "\"";
        }
    }
}
