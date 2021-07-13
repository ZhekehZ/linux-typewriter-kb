#include "sound/snd_sdl_mixer.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>


namespace snd {

namespace {
    constexpr char OK_MSG[] = "NO ERROR";
    constexpr char SDL_INIT_ERROR_MSG[] = "SDL INITIALIZATION ERROR";
    constexpr char SDL_MIXER_INIT_ERROR_MSG[] = "SDL MIXER INITIALIZATION ERROR";
    constexpr char SDL_ALLOCATE_CHANNELS_ERROR_MSG[] = "SDL ALLOCATING CHANNELS ERROR";

    SDLRAIIContextManager::Status init_SDL(int freq, int max_channels, int chunk_size) {
        using Status = SDLRAIIContextManager::Status;
        
        if (SDL_Init(SDL_INIT_AUDIO) < 0) {
            return Status::SDL_INIT_ERROR;
        }
        if (Mix_OpenAudio(freq, MIX_DEFAULT_FORMAT, 2, chunk_size) < 0) {
            return Status::SDL_MIXER_INIT_ERROR;
        } 
        if (Mix_AllocateChannels(max_channels) <= 0) {
            return Status::SDL_ALLOCATE_CHANNELS_ERROR;
        }
        return Status::OK;
    }

} // namespace

SDLRAIIContextManager::SDLRAIIContextManager(int freq, int max_channels, int chunk_size)
    : status_(init_SDL(freq, max_channels, chunk_size))
{
}

SDLRAIIContextManager::~SDLRAIIContextManager() {
    Mix_Quit();
}

SDLRAIIContextManager::operator bool() const {
    return status_ == OK;
}
 
char const * SDLRAIIContextManager::get_error_message() const {
    switch (status_)
    {
        case OK: return OK_MSG;
        case SDL_INIT_ERROR: return SDL_INIT_ERROR_MSG;
        case SDL_MIXER_INIT_ERROR: return SDL_MIXER_INIT_ERROR_MSG;
        case SDL_ALLOCATE_CHANNELS_ERROR: return SDL_ALLOCATE_CHANNELS_ERROR_MSG;
    }
    return nullptr;
}


} // namespace snd