#include <iostream>

#include "keyboard/kb_event_reader.hpp"
#include "typewriter/typewriter.hpp"
#include "typewriter/config.hpp"
#include "sound/snd_sdl_mixer.hpp"


kb::Event recv() {
    kb::Event event;
    std::cin.read(reinterpret_cast<char *>(&event.type), sizeof(event.type));
    std::cin.read(reinterpret_cast<char *>(&event.kind), sizeof(event.kind));
    return event;
}

int main() {
    snd::SDLRAIIContextManager manager(44100, 7, 1024);
    if (!manager) {
        std::cerr << manager.get_error_message() << std::endl;
        return EXIT_FAILURE;
    }

    typewriter::Typewriter type(DEFAULT_CONFIG);

    while (std::cin) {
        auto event = recv();
        switch (event.kind) {
            case kb::Event::Kind::DOWN: {
                type.down(event.type);
                break;
            }
            case kb::Event::Kind::UP: {
                type.up(event.type);
                break;
            }
            case kb::Event::Kind::PRESSED: {
                type.hold(event.type);
                break;
            }
            case kb::Event::Kind::ERROR: {
                std::cerr << "INVALID EVENT!" << std::endl;
                break;
            }
        }
    }
}