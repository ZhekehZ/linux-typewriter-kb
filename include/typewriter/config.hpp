#pragma once

#include "sound/snd_wav_sound.hpp"
#include "typewriter/typewriter.hpp"
#include <injector/injector.hpp>

namespace detail {
    inline consteval typewriter::TypewriterConfig read_config() noexcept {
        auto stream = injector::get_resource_stream<
            injector::constinit_injected_resources::config>();

        typewriter::TypewriterConfig result{};

        size_t btn_type = 0;
        size_t action_type = 0;

        auto button_sounds = [&]() -> typewriter::SoundArray & {
            auto & button = [&]() -> typewriter::ButtonConfig & {
                if (btn_type == 0) return result.regular_button;
                if (btn_type == 1) return result.special_button;
                return result.enter_button;
            }();

            if (action_type == 0) return button.up_sounds;
            if (action_type == 1) return button.down_sounds;
            return button.hold_sounds;
        };

        while (!stream.eof()) {
            size_t initial_pos = stream.current_pos();
            std::string_view word;
            stream >> word;

            if (word == "REGULAR:") {
                btn_type = 0;
            } else if (word == "SPECIAL:") {
                btn_type = 1;
            } else if (word == "ENTER:") {
                btn_type = 2;
            } else if (word == "UP:") {
                action_type = 0;
            } else if (word == "DOWN:") {
                action_type = 1;
            } else if (word == "HOLD:") {
                action_type = 2;
            } else if (word == "SELECTOR:") {
                // unsupported
            } else {
                stream.set_pos(initial_pos);
                injector::injected_resources resource{};
                stream >> resource;
                auto & sounds = button_sounds();
                sounds.storage[sounds.count] = resource;
                ++sounds.count;
            }

            if (stream.last_error() != injector::detail::parse_error_code::NO_ERROR) {
                break;
            }
        }

        return result;
    }
} // namespace detail

constinit typewriter::TypewriterConfig DEFAULT_CONFIG = detail::read_config();
