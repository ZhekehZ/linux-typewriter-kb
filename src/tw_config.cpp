#include "utils/utils.hpp"

#include <iostream>
#include <cstring>

int clamp(int val, int min, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val; 
}

int main(int argc, char const ** argv) {
    utils::AppConfig configStorage(utils::CONFIG_MEM_NAME, false, os::StorageAccessMode::W);
    auto & [opt_volume, opt_exit] = configStorage.get();

    if (argc == 2) {
        if (strcmp(argv[1], "shutdown") == 0) {
            std::cout << "Shutting down" << std::endl;
            opt_exit = true;
            return EXIT_SUCCESS;
        }
    }
    if (argc == 3) {
        if (strcmp(argv[1], "setvolume") == 0) {
            int volume = clamp(std::atoi(argv[2]), 0, 100);
            std::cout << "Setting volume to " 
                      << volume << "%" << std::endl;
            opt_volume = static_cast<int>(volume * 1.28);
            return EXIT_SUCCESS;
        }
    }

    std::cerr << "Invalid command" << std::endl;
    return EXIT_FAILURE;
}