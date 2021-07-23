#pragma once

#include <memory>
#include <string>
#include <optional>
#include <functional>
#include <map>

#include <SDL2/SDL_mixer.h>

namespace snd {


namespace detail {

    struct Bytes {
        unsigned char * data;
        unsigned int len;
    };

    std::map<std::string, Bytes> & GET_GLOBAL_STORAGE();

}

#ifndef INJECT_RESOURCES
#define WAV_RESOURCE(folder, name) "assets/"#folder"/"#name".wav"
#else
#define WAV_RESOURCE(folder, name)                     \
[](){                                                  \
    extern unsigned char __##folder##_##name##_wav[];  \
    extern unsigned int __##folder##_##name##_wav_len; \
    std::string what = #folder#name;                   \
    ::snd::detail::GET_GLOBAL_STORAGE()[what] =        \
                         ::snd::detail::Bytes{         \
        __##folder##_##name##_wav,                     \
        __##folder##_##name##_wav_len                  \
    };                                                 \
    return what;                                       \
}()
#endif


class WAVSound {
public:
    using Ptr = std::shared_ptr<WAVSound>;
    
    void play();

    WAVSound(std::string const & filename, int volume);

    void set_volume(int value);

private:
    std::unique_ptr<Mix_Chunk, void (*)(Mix_Chunk *)> chunk_;

};

} // namespace snd