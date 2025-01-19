#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

constexpr const char* SHARED_MEMORY_NAME = "MY_SHARED_MEMORY";

struct SharedMemory {
  unsigned int counter;
};

SharedMemory* getSharedMemory(bool needCreate = false);

void cleanSharedMemory();

#endif //SHARED_MEMORY_H
