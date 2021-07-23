#include <iostream>

#include "keyboard/kb_event_reader.hpp"
#include "typewriter/typewriter.hpp"
#include "typewriter/config.hpp"
#include "sound/snd_sdl_mixer.hpp"
#include "utils/utils.hpp"

kb::Event recv() {
    kb::Event event;
    std::cin.read(reinterpret_cast<char *>(&event.type), sizeof(event.type));
    std::cin.read(reinterpret_cast<char *>(&event.kind), sizeof(event.kind));
    return event;
}

bool can_recv() {
    return std::cin.operator bool();
}

int main() {
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::ios_base::sync_with_stdio(false);
    
    snd::SDLRAIIContextManager manager(44100, 7, 1024);
    if (!manager) {
        std::cerr << manager.get_error_message() << std::endl;
        return EXIT_FAILURE;
    }

    typewriter::Typewriter type(DEFAULT_CONFIG, 50);

    utils::AppConfig optionsStorage(utils::CONFIG_MEM_NAME, true, os::StorageAccessMode::RW);
    auto & [opt_volume, opt_exit] = optionsStorage.get();

    while (can_recv() && !opt_exit) {
        if (opt_volume != type.get_volume()) {
            type.set_volume(opt_volume);
        }

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