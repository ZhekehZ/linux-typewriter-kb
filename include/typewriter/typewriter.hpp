#pragma once

#include "sound/snd_wav_sound.hpp"
#include "keyboard/kb_button_codes.hpp"

#include <vector>

namespace typewriter {

enum class SoundSelectKind {
    RANDOM
};

struct ButtonConfig {
    std::vector<std::string> down_sounds;
    std::vector<std::string> up_sounds;
    std::vector<std::string> hold_sounds;
    SoundSelectKind selector;
};

struct TypewriterConfig {
    ButtonConfig regular_button;
    ButtonConfig special_button;
    ButtonConfig enter_button;
};

namespace detail {

    struct InnerButtonConfig {
        std::vector<int> down_ids;
        std::vector<int> up_ids;
        std::vector<int> hold_ids;
        SoundSelectKind selector;
    };

    struct InnerConfig {
        InnerButtonConfig regular_button;
        InnerButtonConfig special_button;
        InnerButtonConfig enter_button;
    };

} // namespace detail

class Typewriter {
public:
    Typewriter(TypewriterConfig const & config);

    void down(kb::ButtonType type);

    void up(kb::ButtonType type);

    void hold(kb::ButtonType type);

private:
    std::vector<snd::WAVSound> sounds_;
    detail::InnerConfig config_;
};

} // namespace typewriter