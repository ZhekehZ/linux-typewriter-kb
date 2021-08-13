#include <cstring>
#include <fcntl.h>
#include <iostream>

#include "keyboard/kb_event_reader.hpp"
#include "os_linux/os_fs_get_keyboards.hpp"
#include "os_linux/os_open_file.hpp"

constexpr char CONTROL_PIPE_NAME[] = "/tmp/typewriter-kb-pipe";

namespace {

void send(kb::Event const &event) {
    std::cout.write(reinterpret_cast<const char *>(&event.type), sizeof(event.type));
    std::cout.write(reinterpret_cast<const char *>(&event.kind), sizeof(event.kind));
    std::cout.write(reinterpret_cast<const char *>(&event.value), sizeof(event.value));
    std::cout.flush();
}

kb::config_source parse_arguments(int argc, char const *argv[]) {
    kb::config_source result;
    for (int i = 1; i < argc; ++i) {
        if (strcmp("--stdin", argv[i]) == 0) {
            result.use_stdin = true;
        }
        if (strcmp("--pipe", argv[i]) == 0) {
            result.special_fd = os::open_file(CONTROL_PIPE_NAME, O_RDONLY, O_NONBLOCK);
        }
    }
    return result;
}

}// namespace

int main(int argc, char const *argv[]) {
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    auto config = parse_arguments(argc, argv);
    auto keyboards_dss = os::filesystem::open_and_get_all_keyboards();

    if (keyboards_dss.empty()) {
        std::cerr << "No keyboards found. Make sure you are root." << std::endl;
        return EXIT_FAILURE;
    }

    kb::event_reader kb_reader(config, std::move(keyboards_dss));
    std::optional<kb::Event> event;

    while ((event = kb_reader.next())) {
        send(event.value());
        if (event->kind == kb::Event::Kind::EXIT) {
            break;
        }
    }
}
