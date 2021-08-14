#pragma once

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>

#include <SDL2/SDL_mixer.h>

namespace snd {

namespace detail {

struct Bytes {
    unsigned char *data;
    size_t len;
};

std::map<std::string, Bytes> &GET_GLOBAL_STORAGE();

}// namespace detail

#ifndef INJECT_RESOURCES
#define WAV_RESOURCE(folder, name) "assets/wav/" #folder "/" #name ".wav"
#else
#define WAV_RESOURCE(folder, name)                               \
    []() {                                                       \
        extern unsigned char folder##_##name##_wav[];            \
        extern unsigned int folder##_##name##_wav_len;           \
        std::string what = #folder #name;                        \
        ::snd::detail::GET_GLOBAL_STORAGE()[what] =              \
            ::snd::detail::Bytes{                                \
                folder##_##name##_wav,                           \
                static_cast<size_t>(folder##_##name##_wav_len)}; \
        return what;                                             \
    }()
#endif

class WAVSound {
 public:
    using Ptr = std::shared_ptr<WAVSound>;

    void play();

    WAVSound(std::string const &filename, int volume);

    void set_volume(int value);

 private:
    std::unique_ptr<Mix_Chunk, void (*)(Mix_Chunk *)> chunk_;
};

}// namespace snd