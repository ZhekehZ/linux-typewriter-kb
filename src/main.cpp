#include <iostream>

#include "keyboard/kb_event_reader.hpp"
#include "keyboard/kb_linux_fs.hpp"
#include "sound/snd_sdl_mixer.hpp"

#include "typewriter/typewriter.hpp"
#include "typewriter/config.hpp"

std::optional<kb::event_reader> build_reader_for_all_keyboards() {
    auto keyboards_dss = kb::linux_filesystem::open_and_get_all_keyboards();
    if (keyboards_dss.empty()) {
        return std::nullopt;
    }
    return std::optional<kb::event_reader>( std::move(keyboards_dss) );
}


int main(){
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

    typewriter::Typewriter type(DEFAULT_CONFIG);

    std::cout << "SUCCESSFULLY INITIALIZED" << std::endl;

    std::optional<kb::Event> event;
    while (event = kb_event_reader->next()) {
        switch (event->kind) {
            case kb::Event::DOWN: {
                std::cout << "DOWN!" << std::endl;
                type.down(event->button);
                break;
            }
            case kb::Event::UP: {
                type.up(event->button);
                break;
            }
            case kb::Event::PRESSED: {
                type.hold(event->button);
                break;
            }
            case kb::Event::ERROR: {
                std::cerr << "INVALID EVENT!" << std::endl;
                break;
            }
        }
    }

    while (true);

    return EXIT_SUCCESS;    
}
