#pragma once

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include <optional>
#include <stdexcept>

namespace os {

enum class StorageAccessMode : int {
    R = O_RDONLY, 
    W = O_RDWR, 
    RW = O_RDWR
};


namespace detail {

int get_open_keys(bool owner, StorageAccessMode mode);

int get_page_keys(StorageAccessMode mode);

} // namespace detail

template <typename StorageT>
class SharedStorage {
public:

    template <typename ... Args>
    SharedStorage(
        const char * name, 
        bool owner, 
        StorageAccessMode mode,
        Args && ... args
    )
        : owner_(owner)
        , name_(name) 
    {
        fd_= shm_open(name, detail::get_open_keys(owner, mode), S_IRUSR | S_IWUSR);
        if (fd_ == -1) {
            throw std::runtime_error("can't open shared memory");
        }

        if (ftruncate(fd_, sizeof(StorageT)) == -1) {
            throw std::runtime_error("can't allocate shared memory with size " + 
                                     std::to_string(sizeof(StorageT)));
        }

        data_ = mmap(0, sizeof(StorageT), detail::get_page_keys(mode), MAP_SHARED, fd_, 0);

        if (owner) {
            new (data_) StorageT(std::forward<Args>(args) ...);
        }
    }

    StorageT & get() {
        return *static_cast<StorageT *>(data_);
    }

    ~SharedStorage() {
        if (owner_) {
            get().~StorageT();
        }
        munmap(data_, sizeof(StorageT));
        if (owner_) {
            shm_unlink(name_);
        }
        close(fd_); 
    }

private:
    bool const owner_;
    const char * const name_;
    
    int fd_;
    void * data_;
};

} // namespace os
