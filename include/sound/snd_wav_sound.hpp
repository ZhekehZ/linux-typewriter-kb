#pragma once

#include <functional>
#include <injector/injector.hpp>
#include <map>
#include <memory>
#include <optional>
#include <string>

#include <SDL2/SDL_mixer.h>

namespace snd {

class WAVSound {
 public:
    void play();

    WAVSound(injector::injected_resources resource, int volume);

    void set_volume(int value);

 private:
    std::unique_ptr<Mix_Chunk, void (*)(Mix_Chunk *)> chunk_;
};

}// namespace snd