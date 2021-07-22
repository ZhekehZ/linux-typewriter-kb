#include <iostream>

#include "keyboard/kb_event_reader.hpp"
#include "keyboard/kb_linux_fs.hpp"

std::optional<kb::event_reader> build_reader_for_all_keyboards() {
    auto keyboards_dss = kb::linux_filesystem::open_and_get_all_keyboards();
    if (keyboards_dss.empty()) {
        return std::nullopt;
    }
    return std::optional<kb::event_reader>( std::move(keyboards_dss) );
}

void send(kb::Event const & event) {
    std::cout.write(reinterpret_cast<const char *>(&event.type), sizeof(event.type));
    std::cout.write(reinterpret_cast<const char *>(&event.kind), sizeof(event.kind));
    std::cout.flush();
}

int main() {
    if (auto kb_reader = build_reader_for_all_keyboards(); kb_reader) {
        
        std::optional<kb::Event> event;
        while (event = kb_reader->next()) {
            send(*event);
        }

    } else {
        std::cerr << "No keyboards found. Make sure you are root." << std::endl;
        return EXIT_FAILURE;
    }
}

