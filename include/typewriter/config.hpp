#pragma once
// clang-format off

#include "sound/snd_wav_sound.hpp"
#include "typewriter/typewriter.hpp"

const typewriter::TypewriterConfig DEFAULT_CONFIG = {
    .regular_button = {
        .down_sounds = { WAV_RESOURCE(down, 1), WAV_RESOURCE(down, 2), WAV_RESOURCE(down, 4) },
        .up_sounds =   { WAV_RESOURCE(up, 1), WAV_RESOURCE(up, 2), WAV_RESOURCE(up, 4) },
        .hold_sounds = {},
        .selector = typewriter::SoundSelectKind::RANDOM
    },

    .special_button = {
        .down_sounds = { WAV_RESOURCE(down, 3), WAV_RESOURCE(down, 5) },
        .up_sounds =   { WAV_RESOURCE(up, 3), WAV_RESOURCE(up, 5) },
        .hold_sounds = {},
        .selector = typewriter::SoundSelectKind::RANDOM
    },

    .enter_button = {
        .down_sounds = { WAV_RESOURCE(down, 1), WAV_RESOURCE(down, 2), WAV_RESOURCE(down, 4) },
        .up_sounds =   { WAV_RESOURCE(up, carrige_return) },
        .hold_sounds = {},
        .selector = typewriter::SoundSelectKind::RANDOM
    }
};

// clang-format on