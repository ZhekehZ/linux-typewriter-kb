#include "keyboard/kb_event_reader.hpp"

#include <sys/select.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <variant>

namespace kb {

namespace {

bool is_kb_event(input_event const &event) {
    return event.type == EV_KEY;
}

Event ievent_to_event(input_event const &ievent) {
    Event::Kind kind = ievent.value == 0 ? Event::Kind::UP
        : ievent.value == 1              ? Event::Kind::DOWN
        : ievent.value == 2              ? Event::Kind::PRESSED
                                         : Event::Kind::ERROR;
    ButtonType type = code_to_type(ievent.code);
    return Event{
        .type = type,
        .kind = kind,
        .value = 0};
}

enum class ExitCode {
    END_OF_INPUT,
    STOP_SYMBOL,
    ERROR
};

std::variant<input_event, ExitCode> next_ievent(int fd) {
    input_event event;

    size_t left = sizeof(input_event);
    while (left > 0) {
        auto ret_val = read(fd, reinterpret_cast<char *>(&event), left);
        if (ret_val < 0) {
            return ExitCode::ERROR;
        } else if (ret_val == 0) {
            return ExitCode::END_OF_INPUT;
        }
        left -= static_cast<size_t>(ret_val);
    }

    if (event.code == 0 && event.value == 0 && event.type == 0) {
        return ExitCode::STOP_SYMBOL;
    }

    return {event};
}

std::optional<Event> get_event_from_fd(int fd) {
    static char message[20];
    auto bytes = read(fd, message, 20);
    if (bytes < 2 || bytes >= static_cast<ssize_t>(sizeof(message))) {
        return std::nullopt;
    }

    if (message[0] == 'v') {
        int volume = atoi(message + 1);
        return {
            Event{
                .type = ButtonType::NONE,
                .kind = Event::Kind::SET_VOLUME,
                .value = volume}};
    } else if (message[0] == 'g') {
        return {
            Event{
                .type = ButtonType::NONE,
                .kind = Event::Kind::GET_VOLUME,
                .value = 0}};
    } else if (strncmp(message, "exit", 4) == 0) {
        return {
            Event{
                .type = ButtonType::NONE,
                .kind = Event::Kind::EXIT,
                .value = 0}};
    }

    return std::nullopt;
}

}// namespace

event_reader::event_reader(config_source config, std::vector<int> descriptors)
    : config_(config) {
    max_ds_ = -1;
    if (config_.use_stdin) {
        read_ds_.emplace_back(STDIN_FILENO, true);
        max_ds_ = std::max(STDIN_FILENO, max_ds_);
    }
    if (config_.special_fd.has_value()) {
        read_ds_.emplace_back(config_.special_fd.value(), true);
        max_ds_ = std::max(config_.special_fd.value(), max_ds_);
    }
    for (int fd : descriptors) {
        read_ds_.emplace_back(fd, true);
        max_ds_ = std::max(fd, max_ds_);
    }
}

std::optional<Event> event_reader::next() {
    int alives_count = 2;

    while (alives_count > 1) {
        fd_set fds;
        make_fd_set(fds);

        if (select(max_ds_ + 1, &fds, nullptr, nullptr, nullptr) < 0) {
            return std::nullopt;
        }

        alives_count = 0;
        for (auto &[fd, fd_is_active] : read_ds_) {
            if (!fd_is_active) {
                continue;
            }
            ++alives_count;

            if (FD_ISSET(fd, &fds)) {

                if (fd == config_.special_fd.value_or(STDIN_FILENO)) {
                    auto stdin_event = get_event_from_fd(fd);
                    if (stdin_event.has_value()) {
                        return stdin_event;
                    }
                    continue;
                }

                std::optional<Event> event;
                while (true) {
                    auto ievent = next_ievent(fd);
                    if (std::holds_alternative<input_event>(ievent)) {
                        auto const &nextEvent = std::get<input_event>(ievent);
                        if (is_kb_event(nextEvent)) {
                            event = ievent_to_event(nextEvent);
                        }
                    } else {
                        auto const &exitCode = std::get<ExitCode>(ievent);
                        if (exitCode != ExitCode::STOP_SYMBOL) {
                            --alives_count;
                        }
                        break;
                    }
                }
                if (event.has_value()) {
                    return event;
                }
            }
        }
    }

    return std::nullopt;
}

event_reader::~event_reader() {
    for (auto [fd, _] : read_ds_) {
        close(fd);
    }
}

void event_reader::make_fd_set(fd_set &fds) {
    FD_ZERO(&fds);
    for (auto [fd, is_active] : read_ds_) {
        if (is_active) {
            FD_SET(fd, &fds);
        }
    }
}

}// namespace kb
