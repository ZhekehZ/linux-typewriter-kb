#include "keyboard/kb_event_reader.hpp"

#include <unistd.h>
#include <sys/select.h>

#include <cstring>

namespace kb {

namespace {

    bool is_kb_event(input_event const & event) {
        return event.type == EV_KEY;
    }

    Event ievent_to_event(input_event const & ievent) {
        Event::Kind kind = ievent.value == 0 ? Event::Kind::UP
                         : ievent.value == 1 ? Event::Kind::DOWN
                         : ievent.value == 2 ? Event::Kind::PRESSED
                         :                     Event::Kind::ERROR;
        ButtonType type = code_to_type(ievent.code);
        return Event{type, kind};	
    }

    std::optional<input_event> next_ievent(int fd) {
        input_event event;

        ssize_t left = sizeof(input_event);
        while (left > 0) {
            int ret_val = read(fd, reinterpret_cast<char *>(&event), left);
            if (ret_val < 0) {
                return std::nullopt;
            }
            left -= ret_val;
        }

        return {event};
    }

} // namespace


event_reader::event_reader(std::vector<int> descriptors)
    : read_ds_(std::move(descriptors))
{
    max_ds_ = read_ds_[0];
    for (int fd : read_ds_) {
        max_ds_ = std::max(fd, max_ds_);
    }
}

std::optional<Event> event_reader::next() {
    fd_set fds;
    make_fd_set(fds);

    int ret_val = select(max_ds_ + 1, &fds, nullptr, nullptr, nullptr);

    if (ret_val < 0) {
        return std::nullopt;
    }

    for (auto fd : read_ds_) {
        if (FD_ISSET(fd, &fds)) {
            while (auto ievent = next_ievent(fd)) {
                if (is_kb_event(*ievent)) {
                    return {ievent_to_event(*ievent)};
                }
            }
        }
    }

    return std::nullopt;
}

event_reader::~event_reader() {
    for (int fd : read_ds_) {
        close(fd);
    }
}

void event_reader::make_fd_set(fd_set & fds) {
    FD_ZERO(&fds);
    for (auto fd : read_ds_) {
        FD_SET(fd, &fds);
    }
}


} // namespace kb
