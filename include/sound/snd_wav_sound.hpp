#pragma once

#include <memory>
#include <string>
#include <optional>
#include <functional>

#include <SDL2/SDL_mixer.h>

namespace snd {

class WAVSound {
public:
    using Ptr = std::shared_ptr<WAVSound>;
    
    void play();

    WAVSound(std::string const & filename, int volume);

private:
    std::unique_ptr<Mix_Chunk, void (*)(Mix_Chunk *)> chunk_;

};

} // namespace snd