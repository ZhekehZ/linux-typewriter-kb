#pragma once

#include "keyboard/kb_button_codes.hpp"
#include "sound/snd_wav_sound.hpp"

#include <injector/injector.hpp>
#include <vector>

namespace typewriter {

struct SoundArray {
    size_t count;
    std::array<injector::injected_resources,
               MAX_SOUNDS_PER_BUTTON> storage;
};

enum class SoundSelectStrategy {
    RANDOM  // TODO implement other?
};

struct ButtonConfig {
    SoundArray down_sounds;
    SoundArray up_sounds;
    SoundArray hold_sounds;
    SoundSelectStrategy selector;
};

struct TypewriterConfig {
    ButtonConfig regular_button;
    ButtonConfig special_button;
    ButtonConfig enter_button;
};

class Typewriter {
 public:
    Typewriter(TypewriterConfig const &config,
               int default_volume);

    [[nodiscard]]
    int get_volume() const;
    void set_volume(int value);

    void down(kb::ButtonType type);

    void up(kb::ButtonType type);

    void hold(kb::ButtonType type);

 private:
    std::vector<snd::WAVSound> sounds_;
    TypewriterConfig config_;
    int volume_;
};

}// namespace typewriter