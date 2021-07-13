#include "sound/snd_wav_sound.hpp"

#include <stdexcept>

#include <SDL2/SDL.h>

namespace snd {
    
WAVSound::WAVSound(std::string const & filename, int volume)
    : chunk_(Mix_LoadWAV(filename.c_str()), Mix_FreeChunk) 
{
    if (!chunk_.get()) {
        throw std::runtime_error("Error loading file " + filename);
    }
    Mix_VolumeChunk(chunk_.get(), volume);
}

void WAVSound::play() {
    Mix_PlayChannel(-1, chunk_.get(), 0);
}



} // namespace snd
