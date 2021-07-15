#include "sound/snd_wav_sound.hpp"

#include <stdexcept>

#include <SDL2/SDL.h>

namespace snd {
    
namespace detail {

    std::map<std::string, Bytes> & GET_GLOBAL_STORAGE() {
        static std::map<std::string, Bytes> STORAGE;
        return STORAGE;
    };

} // namespace detail

WAVSound::WAVSound(std::string const & filename, int volume)
    #ifndef INJECT_RESOURCES
    : chunk_(Mix_LoadWAV(filename.c_str()), Mix_FreeChunk) 
    #else
    : chunk_(Mix_LoadWAV_RW(SDL_RWFromMem(detail::GET_GLOBAL_STORAGE()[filename].data, 
                                          detail::GET_GLOBAL_STORAGE()[filename].len), 1), Mix_FreeChunk) 
    #endif
{
    if (!chunk_.get()) {
        throw std::runtime_error("Error loading file " + filename + "[" + Mix_GetError() + "]");
    }
    Mix_VolumeChunk(chunk_.get(), volume);
}

void WAVSound::play() {
    Mix_PlayChannel(-1, chunk_.get(), 0);
}



} // namespace snd
