#pragma once

namespace snd {

class SDLRAIIContextManager {
 public:
    enum Status {
        OK,
        SDL_INIT_ERROR,
        SDL_MIXER_INIT_ERROR,
        SDL_ALLOCATE_CHANNELS_ERROR
    };

    SDLRAIIContextManager(int freq, int max_channels, int chunk_size);

    explicit operator bool() const;

    ~SDLRAIIContextManager();

    [[nodiscard]] char const *get_error_message() const;

 private:
    Status status_;
};

}// namespace snd
