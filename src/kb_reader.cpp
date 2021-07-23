#include <iostream>

#include "keyboard/kb_event_reader.hpp"
#include "os_linux/os_fs_get_keyboards.hpp"

std::optional<kb::event_reader> build_reader_for_all_keyboards() {
    auto keyboards_dss = os::filesystem::open_and_get_all_keyboards();
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
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

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

