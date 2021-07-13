#pragma once

#include <vector>
#include <optional>

#include <linux/input.h>


namespace kb {

struct Event {
    enum Kind {
        DOWN, UP, PRESSED, ERROR
    };

    int button;
    Kind kind;
};

class event_reader {
public:
    event_reader(std::vector<int> descriptors);

    std::optional<Event> next();

    ~event_reader();

private:
    void make_fd_set(fd_set & fds);

    std::vector<int> read_ds_;
    int max_ds_;
};

} // namespace kb
