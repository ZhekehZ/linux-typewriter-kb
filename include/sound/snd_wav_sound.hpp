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
#define WAV_RESOURCE(path, name) "assets/"#path"/"#name".wav"
#else
#define WAV_RESOURCE(path, name)                     \
[](){                                                \
    extern unsigned char __##path##_##name##_wav[];  \
    extern unsigned int __##path##_##name##_wav_len; \
    std::string what = #path#name;                   \
    ::snd::detail::GET_GLOBAL_STORAGE()[what] =      \
                         ::snd::detail::Bytes{       \
        __##path##_##name##_wav,                     \
        __##path##_##name##_wav_len                  \
    };                                               \
    return what;                                     \
}()
#endif


class WAVSound {
public:
    using Ptr = std::shared_ptr<WAVSound>;
    
    void play();

    WAVSound(std::string const & filename, int volume);

private:
    std::unique_ptr<Mix_Chunk, void (*)(Mix_Chunk *)> chunk_;

};

} // namespace snd