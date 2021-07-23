#include "utils/utils.hpp"

#include <iostream>
#include <cstring>

int clamp(int val, int min, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val; 
}


int shutdown(int argc, char const ** argv, int & opt_volume, bool & opt_exit) {
    std::cout << "Shutting down ..." << std::endl;
    opt_exit = true;
    return EXIT_SUCCESS;
}

int setvolume(int argc, char const ** argv, int & opt_volume, bool & opt_exit) {
    int volume = argc == 3 ? clamp(std::atoi(argv[2]), 0, 100) : 0;
    std::cout << "Setting volume to " << volume << "%" << std::endl;
    opt_volume = static_cast<int>(volume * 1.28);
    return EXIT_SUCCESS;
}

int getvolume(int argc, char const ** argv, int & opt_volume, bool & opt_exit) {
    std::cout << "Current volume is " << opt_volume << "%" << std::endl;
    return EXIT_SUCCESS;
}

int main(int argc, char const ** argv) try {

    utils::AppConfig configStorage(utils::CONFIG_MEM_NAME, false, os::StorageAccessMode::W);
    auto & [opt_volume, opt_exit] = configStorage.get();

    using opt_function = int (*)(int, const char **, int &, bool &);
    #define OPT(func) std::pair<const char *, opt_function>(#func, func)
    
    for (auto const & [cmd, callback] : {OPT(shutdown), OPT(getvolume), OPT(setvolume)}) {
        if (argc > 1 && strcmp(argv[1], cmd) == 0) {
            return callback(argc, argv, opt_volume, opt_exit);
        }
    }

    std::cerr << "Invalid command" << std::endl;
    return EXIT_FAILURE;

} catch (...) {
    std::cerr << "Unexpected error." << std::endl 
              << "Maybe `typewriter-kb start` has not been ran." << std::endl;
    return EXIT_FAILURE;
}