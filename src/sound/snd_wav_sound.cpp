#include "sound/snd_wav_sound.hpp"

#include <stdexcept>

#include <SDL2/SDL.h>

namespace snd {

namespace {

std::unique_ptr<Mix_Chunk, void (*)(Mix_Chunk *)> load(
    injector::injected_resources resource) {
    auto stream = injector::get_resource_stream(resource);
    Mix_Chunk *chunk = Mix_LoadWAV_RW(
        SDL_RWFromConstMem(stream.data(), stream.size()), 1);
    return {chunk, Mix_FreeChunk};
}

}// namespace

WAVSound::WAVSound(injector::injected_resources resource, int volume)
    : chunk_(load(resource)) {
    if (!chunk_) {
        throw std::runtime_error(std::string("Error loading file [") + Mix_GetError() + "]");
    }
    Mix_VolumeChunk(chunk_.get(), volume);
}

void WAVSound::play() {
    Mix_PlayChannel(-1, chunk_.get(), 0);
}

void WAVSound::set_volume(int value) {
    Mix_VolumeChunk(chunk_.get(), value);
}

}// namespace snd
