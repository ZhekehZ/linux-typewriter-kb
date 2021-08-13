#pragma once

#include "os_linux/os_shmem.hpp"

namespace utils {

constexpr char CONFIG_MEM_NAME[] = "/typewriter-kb_config_memory_2";
constexpr int DEFAULT_VOLUME = 50;

struct AppConfigValues {
    int volume = DEFAULT_VOLUME;
    bool exit = false;
};

using AppConfig = os::SharedStorage<AppConfigValues>;

}// namespace utils
