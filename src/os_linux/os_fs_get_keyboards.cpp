#include "os_linux/os_fs_get_keyboards.hpp"

#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace os::filesystem {

namespace {
constexpr char DEVICES_IDS_DIR[] = "/dev/input/by-path/";

bool is_keyboard_name(char const *name) {
    auto name_len = strlen(name);
    if (name_len < 3) {
        return false;
    }
    return strcmp("kbd", name + name_len - 3) == 0;
}

}// namespace

std::vector<int> open_and_get_all_keyboards() {
    std::vector<int> result;

    auto dir = opendir(DEVICES_IDS_DIR);

    if (!dir) {
        return result;
    }

    char link_filename[256];

    for (auto dir_entry = readdir(dir); dir_entry; dir_entry = readdir(dir)) {
        if (dir_entry->d_type == DT_LNK && is_keyboard_name(dir_entry->d_name)) {
            strcpy(link_filename, DEVICES_IDS_DIR);
            strcat(link_filename, dir_entry->d_name);

            if (int event_file = open(link_filename, O_RDONLY); event_file < 0) {
                continue;
            } else {
                result.push_back(event_file);
            }
        }
    }

    closedir(dir);
    return result;
}

}// namespace os::filesystem
