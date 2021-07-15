#include "keyboard/kb_button_codes.hpp"

#include <initializer_list>

namespace kb::code {

bool is_enter(int code) {
    return code == ENTER || code == ENTER2;
}

bool is_special(int code) {
    for (int special : 
        { LEFT_SHIFT, RIGHT_SHIFT, LEFT_ALT, RIGHT_ALT, TAB, BACKSPACE}) 
    {
        if (code == special) {
            return true;
        }
    }
    return false;
}

} // namespace kb::code
