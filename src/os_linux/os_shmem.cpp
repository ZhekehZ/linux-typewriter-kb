#include "os_linux/os_shmem.hpp"

namespace os::detail {

int get_open_keys(bool owner, StorageAccessMode mode) {
    return static_cast<int>(mode) | (owner ? O_EXCL | O_CREAT : 0);
}

int get_page_keys(StorageAccessMode mode) {
    if (mode == StorageAccessMode::R) {
        return PROT_READ;
    } 
    return PROT_READ | PROT_WRITE;
}

} // namespace os::detail
