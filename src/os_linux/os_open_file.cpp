#include "os_linux/os_open_file.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace os {

    std::optional<int> open_file(char const * path, int mode, int opts) {
        int fd = open(path, mode, opts);
        if (fd < 0) {
            return std::nullopt;
        }
        return {fd};
    }

}