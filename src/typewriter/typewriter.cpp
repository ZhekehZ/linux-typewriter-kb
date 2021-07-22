#include "typewriter/typewriter.hpp"
#include "keyboard/kb_button_codes.hpp"

#include <map>
#include <string>
#include <stdexcept>

namespace typewriter {

namespace {

    void load(std::vector<std::string> const & names,
              std::vector<int> & indices,
              std::map<std::string, int> & loaded) {
        indices.reserve(names.size());
        for (auto const & name : names) {
            auto it = loaded.find(name);
            if (it == loaded.end()) {
                int new_idx = loaded.size();
                loaded[name] = new_idx;
                indices.push_back(new_idx);
            } else {
                indices.push_back(it->second);
            }
        }
    }

    void load(ButtonConfig const & btnCfg, 
              detail::InnerButtonConfig & cfg, 
              std::map<std::string, int> & loaded) {
        load(btnCfg.down_sounds, cfg.down_ids, loaded);
        load(btnCfg.up_sounds, cfg.up_ids, loaded);
        load(btnCfg.hold_sounds, cfg.hold_ids, loaded);
        cfg.selector = btnCfg.selector;
    }

    std::vector<std::string> inverse(std::map<std::string, int> const & loaded) {
        std::vector<std::string> result(loaded.size());
        for (auto const & [name, idx] : loaded) {
            result[idx] = name;
        }
        return result;
    }


    int choose(std::vector<int> const & indices, SoundSelectKind selector) {
        switch (selector)
        {
            case SoundSelectKind::RANDOM: {
                return indices[rand() % indices.size()];
            };

            default: throw std::runtime_error("Invalid selector");
        }
    }

    detail::InnerButtonConfig const & get_btn_cfg(
        kb::ButtonType type, 
        detail::InnerConfig const & cfg
    ) {
        if (type == kb::ButtonType::SPECIAL) {
            return cfg.special_button;
        } else if (type == kb::ButtonType::ENTER) {
            return cfg.enter_button;
        } 
        return cfg.regular_button;
    }

} // namespace

Typewriter::Typewriter(TypewriterConfig const & config) {
    std::map<std::string, int> loaded;

    load(config.regular_button, config_.regular_button, loaded);
    load(config.special_button, config_.special_button, loaded);
    load(config.enter_button, config_.enter_button, loaded);

    sounds_.reserve(loaded.size());
    for (auto const & sound_name : inverse(loaded)) {
        sounds_.emplace_back(sound_name, 50);
    }
}

void Typewriter::down(kb::ButtonType type) {
    auto const & bcfg = get_btn_cfg(type, config_);
    if (bcfg.down_ids.empty()) return;
    int idx = choose(bcfg.down_ids, bcfg.selector);
    sounds_[idx].play();
}

void Typewriter::up(kb::ButtonType type) {
    auto const & bcfg = get_btn_cfg(type, config_);
    if (bcfg.up_ids.empty()) return;
    int idx = choose(bcfg.up_ids, bcfg.selector);
    sounds_[idx].play();
}

void Typewriter::hold(kb::ButtonType type) {
    auto const & bcfg = get_btn_cfg(type, config_);
    if (bcfg.hold_ids.empty()) return;
    int idx = choose(bcfg.hold_ids, bcfg.selector);
    sounds_[idx].play();
}

} // namespace typewriter