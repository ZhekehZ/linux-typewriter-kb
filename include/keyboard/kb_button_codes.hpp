#pragma once

#include <linux/input.h>

namespace kb {

namespace code {

inline constexpr int ENTER = KEY_ENTER;
inline constexpr int ENTER2 = KEY_KPENTER;

inline constexpr int LEFT_SHIFT = KEY_LEFTSHIFT;
inline constexpr int RIGHT_SHIFT = KEY_RIGHTSHIFT;

inline constexpr int LEFT_ALT = KEY_LEFTALT;
inline constexpr int RIGHT_ALT = KEY_RIGHTALT;

inline constexpr int TAB = KEY_TAB;
inline constexpr int BACKSPACE = KEY_BACKSPACE;

bool is_enter(int code);

bool is_special(int code);

}// namespace code

enum class ButtonType {
    REGULAR,
    SPECIAL,
    ENTER,
    NONE
};

ButtonType code_to_type(int code);

}// namespace kb
