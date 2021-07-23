#include <iostream>

#include "keyboard/kb_event_reader.hpp"
#include "os_linux/os_fs_get_keyboards.hpp"
#include "sound/snd_sdl_mixer.hpp"

#include "typewriter/typewriter.hpp"
#include "typewriter/config.hpp"

#include "utils/utils.hpp"

std::optional<kb::event_reader> build_reader_for_all_keyboards() {
    auto keyboards_dss = os::filesystem::open_and_get_all_keyboards();
    if (keyboards_dss.empty()) {
        return std::nullopt;
    }
    return std::optional<kb::event_reader>( std::move(keyboards_dss) );
}


int main() try {
    snd::SDLRAIIContextManager manager(44100, 7, 1024);
    if (!manager) {
        std::cerr << manager.get_error_message() << std::endl;
        return EXIT_FAILURE;
    }

    auto kb_event_reader = build_reader_for_all_keyboards();
    if (!kb_event_reader) {
        std::cerr << "No keyboards found. Make sure you are root." << std::endl;
        return EXIT_FAILURE;
    }

    typewriter::Typewriter type(DEFAULT_CONFIG, 50);

    utils::AppConfig appConfig(utils::CONFIG_MEM_NAME, true, os::StorageAccessMode::R);
    auto & [opt_volume, opt_exit] = appConfig.get();

    std::cout << "SUCCESSFULLY INITIALIZED" << std::endl;

    std::optional<kb::Event> event;
    while (!opt_exit && (event = kb_event_reader->next())) {
        if (type.get_volume() != opt_volume) {
            type.set_volume(opt_volume);
        }

        switch (event->kind) {
            case kb::Event::Kind::DOWN: {
                type.down(event->type);
                break;
            }
            case kb::Event::Kind::UP: {
                type.up(event->type);
                break;
            }
            case kb::Event::Kind::PRESSED: {
                type.hold(event->type);
                break;
            }
            case kb::Event::Kind::ERROR: {
                std::cerr << "INVALID EVENT!" << std::endl;
                break;
            }
        }
    }

    return EXIT_SUCCESS;    
} catch(...) {
    std::cerr << "Unexpected error. " << std::endl
              << "The application may already be running." << std::endl;
    return EXIT_FAILURE;
}
