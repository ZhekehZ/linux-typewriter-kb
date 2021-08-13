#include "typewriter/typewriter.hpp"
#include "keyboard/kb_button_codes.hpp"

#include <map>
#include <stdexcept>
#include <string>

namespace typewriter {

namespace {

void load(std::vector<std::string> const &names,
          std::vector<size_t> &indices,
          std::map<std::string, size_t> &loaded) {
    indices.reserve(names.size());
    for (auto const &name : names) {
        auto it = loaded.find(name);
        if (it == loaded.end()) {
            size_t new_idx = loaded.size();
            loaded[name] = new_idx;
            indices.push_back(new_idx);
        } else {
            indices.push_back(it->second);
        }
    }
}

void load(
    ButtonConfig const &btnCfg,
    detail::InnerButtonConfig &cfg,
    std::map<std::string, size_t> &loaded) {
    load(btnCfg.down_sounds, cfg.down_ids, loaded);
    load(btnCfg.up_sounds, cfg.up_ids, loaded);
    load(btnCfg.hold_sounds, cfg.hold_ids, loaded);
    cfg.selector = btnCfg.selector;
}

std::vector<std::string> inverse(std::map<std::string, size_t> const &loaded) {
    std::vector<std::string> result(loaded.size());
    for (auto const &[name, idx] : loaded) {
        result[idx] = name;
    }
    return result;
}

size_t choose(std::vector<size_t> const &indices, SoundSelectKind selector) {
    switch (selector) {
        case SoundSelectKind::RANDOM: {
            return indices[static_cast<size_t>(rand()) % indices.size()];
        };

        default: throw std::runtime_error("Invalid selector");
    }
}

detail::InnerButtonConfig const &get_btn_cfg(
    kb::ButtonType type,
    detail::InnerConfig const &cfg) {
    if (type == kb::ButtonType::SPECIAL) {
        return cfg.special_button;
    } else if (type == kb::ButtonType::ENTER) {
        return cfg.enter_button;
    }
    return cfg.regular_button;
}

}// namespace

Typewriter::Typewriter(
    TypewriterConfig const &config,
    int default_volume) : volume_(default_volume) {
    std::map<std::string, size_t> loaded;

    load(config.regular_button, config_.regular_button, loaded);
    load(config.special_button, config_.special_button, loaded);
    load(config.enter_button, config_.enter_button, loaded);

    sounds_.reserve(loaded.size());
    for (auto const &sound_name : inverse(loaded)) {
        sounds_.emplace_back(sound_name, volume_);
    }
}

void Typewriter::down(kb::ButtonType type) {
    auto const &bcfg = get_btn_cfg(type, config_);
    if (bcfg.down_ids.empty()) return;
    size_t idx = choose(bcfg.down_ids, bcfg.selector);
    sounds_[idx].play();
}

void Typewriter::up(kb::ButtonType type) {
    auto const &bcfg = get_btn_cfg(type, config_);
    if (bcfg.up_ids.empty()) return;
    size_t idx = choose(bcfg.up_ids, bcfg.selector);
    sounds_[idx].play();
}

void Typewriter::hold(kb::ButtonType type) {
    auto const &bcfg = get_btn_cfg(type, config_);
    if (bcfg.hold_ids.empty()) return;
    size_t idx = choose(bcfg.hold_ids, bcfg.selector);
    sounds_[idx].play();
}

int Typewriter::get_volume() {
    return volume_;
}

void Typewriter::set_volume(int value) {
    for (auto &sound : sounds_) {
        sound.set_volume(value);
    }
    volume_ = value;
}

}// namespace typewriter