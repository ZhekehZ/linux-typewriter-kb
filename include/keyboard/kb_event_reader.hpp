#pragma once

#include "kb_button_codes.hpp"

#include <vector>
#include <optional>
#include <utility>

#include <linux/input.h>


namespace kb {

struct Event {
    enum class Kind {
        DOWN, UP, PRESSED, 
        EXIT, SET_VOLUME,
        ERROR
    };

    ButtonType type;
    Kind kind;
    int value;
};

class event_reader {
public:
    event_reader(std::vector<int> descriptors);

    std::optional<Event> next();

    ~event_reader();

private:
    void make_fd_set(fd_set & fds);

    std::vector<std::pair<int, bool>> read_ds_;
    int max_ds_;
};

} // namespace kb
