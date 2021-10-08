#include "typewriter/typewriter.hpp"
#include "keyboard/kb_button_codes.hpp"

#include <map>
#include <stdexcept>
#include <string>

namespace typewriter {

namespace {

size_t find_max_idx(size_t max_idx, ButtonConfig const &buttonConfig) {
    for (auto idx : buttonConfig.down_sounds.storage) {
        max_idx = std::max(max_idx, static_cast<size_t>(idx));
    }
    for (auto idx : buttonConfig.up_sounds.storage) {
        max_idx = std::max(max_idx, static_cast<size_t>(idx));
    }
    for (auto idx : buttonConfig.hold_sounds.storage) {
        max_idx = std::max(max_idx, static_cast<size_t>(idx));
    }
    return max_idx;
}

size_t find_max_idx(TypewriterConfig const &tpConfig) {
    size_t max_idx = 0;
    max_idx = find_max_idx(max_idx, tpConfig.regular_button);
    max_idx = find_max_idx(max_idx, tpConfig.special_button);
    max_idx = find_max_idx(max_idx, tpConfig.enter_button);
    return max_idx;
}

size_t choose(SoundArray const &sounds, SoundSelectStrategy selector) {
    switch (selector) {
        case SoundSelectStrategy::RANDOM: {
            return static_cast<size_t>(
                sounds.storage[static_cast<size_t>(rand()) % sounds.count]);
        };

        default: throw std::runtime_error("Invalid selector");
    }
}

ButtonConfig const &get_btn_cfg(kb::ButtonType type,
                                TypewriterConfig const &cfg) {
    if (type == kb::ButtonType::SPECIAL) {
        return cfg.special_button;
    } else if (type == kb::ButtonType::ENTER) {
        return cfg.enter_button;
    }
    return cfg.regular_button;
}

}// namespace

Typewriter::Typewriter(TypewriterConfig const &config, int default_volume)
    : config_(config), volume_(default_volume) {
    size_t count = find_max_idx(config) + 1;
    sounds_.reserve(count);

    for (size_t idx = 0; idx < count; ++idx) {
        auto resource = static_cast<injector::injected_resources>(idx);
        sounds_.emplace_back(resource, volume_);
    }
}

void Typewriter::down(kb::ButtonType type) {
    auto const &bcfg = get_btn_cfg(type, config_);
    if (bcfg.down_sounds.count == 0) return;
    size_t idx = choose(bcfg.down_sounds, bcfg.selector);
    sounds_[idx].play();
}

void Typewriter::up(kb::ButtonType type) {
    auto const &bcfg = get_btn_cfg(type, config_);
    if (bcfg.up_sounds.count == 0) return;
    size_t idx = choose(bcfg.up_sounds, bcfg.selector);
    sounds_[idx].play();
}

void Typewriter::hold(kb::ButtonType type) {
    auto const &bcfg = get_btn_cfg(type, config_);
    if (bcfg.hold_sounds.count == 0) return;
    size_t idx = choose(bcfg.hold_sounds, bcfg.selector);
    sounds_[idx].play();
}

int Typewriter::get_volume() const {
    return volume_;
}

void Typewriter::set_volume(int value) {
    for (auto &sound : sounds_) {
        sound.set_volume(value);
    }
    volume_ = value;
}

}// namespace typewriter