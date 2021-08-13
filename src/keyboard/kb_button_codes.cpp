#include "keyboard/kb_button_codes.hpp"

#include <initializer_list>

namespace kb {
namespace code {

bool is_enter(int code) {
    return code == ENTER || code == ENTER2;
}

bool is_special(int code) {
    for (int special :
         {LEFT_SHIFT, RIGHT_SHIFT, LEFT_ALT, RIGHT_ALT, TAB, BACKSPACE}) {
        if (code == special) {
            return true;
        }
    }
    return false;
}

}// namespace code

ButtonType code_to_type(int code) {
    if (code::is_enter(code)) {
        return ButtonType::ENTER;
    }
    if (code::is_special(code)) {
        return ButtonType::SPECIAL;
    }
    return ButtonType::REGULAR;
}

}// namespace kb
