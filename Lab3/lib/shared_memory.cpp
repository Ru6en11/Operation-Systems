#include "shared_memory.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#endif

#ifdef _WIN32
SharedMemory* getSharedMemory(bool needCreate = false) {
    HANDLE hMapFile;

    if (needCreate) {
        hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SharedMemory), SHARED_MEMORY_NAME);
    } else {
        hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, SHARED_MEMORY_NAME);
    }

    if (hMapFile == NULL) {
        std::cout << "Failed to open shared memory" << std::endl;

        return nullptr;
    }

    SharedMemory* data = (SharedMemory*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedMemory));

    if (data == nullptr) {
        std::cout << "Failed to open shared memory" << std::endl;
        CloseHandle(hMapFile);

        return nullptr;
    }

    return data;
}

void cleanSharedMemory() {
    UnmapViewOfFile(SHARED_MEMORY_NAME);
}

#else
SharedMemory* getSharedMemory(bool needCreate) {
    int shm_fd;

    if (needCreate) {
        shm_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);

        if (shm_fd == -1) {
            std::cerr << "Failed to create shared memory." << std::endl;

            return nullptr;
        }

        ftruncate(shm_fd, sizeof(SharedMemory));
    } else {
        shm_fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0666);

        if (shm_fd == -1) {
            std::cerr << "Failed to open shared memory." << std::endl;

            return nullptr;
        }
    }

    SharedMemory* data = (SharedMemory*)mmap(nullptr, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (data == MAP_FAILED) {
        std::cout << "Failed to map shared memory." << std::endl;
        close(shm_fd);

        return nullptr;
    }

    return data;
}

void cleanSharedMemory() {
    shm_unlink(SHARED_MEMORY_NAME);
}

#endif
