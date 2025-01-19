#include "utils.h"

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#endif

std::string SEMAPHORE = "MAIN_PROCESS_SEMAPHORE";

bool isMainProcess() {
#if defined(_WIN32) || defined(_WIN64)
    HANDLE hSemaphore = CreateSemaphore(NULL, 1, 1, SEMAPHORE.c_str());

    if (hSemaphore == NULL) {
        std::cout << "Failed to create semaphore." << std::endl;

        return false;
    }

    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        CloseHandle(hSemaphore);

        return false;
    }

    static HANDLE main_semaphore = hSemaphore; // for non closing

    return true;

#else
    sem_t* semaphore = sem_open(SEMAPHORE.c_str(), O_CREAT | O_EXCL, 0666, 1);

    if (semaphore == SEM_FAILED) {

        if (errno == EEXIST) {
            return false;
        } else {
            std::cout << "Failed to create semaphore." << std::endl;

            return false;
        }
    }

    static sem_t* main_semaphore = semaphore; // For non closing

    return true;
#endif
}

void closeSemaphore() {
#if defined(_WIN32) || defined(_WIN64)
    HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, SEMAPHORE.c_str());

    if (hSemaphore != NULL) {
        CloseHandle(hSemaphore);
        std::cout << "Semaphore closed: " << SEMAPHORE << std::endl;
    } else {
        std::cout << "Failed to open semaphore for closing." << std::endl;
    }
#else
    if (sem_unlink(SEMAPHORE.c_str()) == 0) {
        std::cout << "Semaphore unlinked: " << SEMAPHORE << std::endl;
    } else {
      std::cout << "Failed unlinking semaphore" << std::endl;
    }
#endif
}

unsigned int getPid() {
#ifdef _WIN32
  return static_cast<unsigned int>(GetCurrentProcessId());
#else
  return static_cast<unsigned int>(getpid());
#endif
}

std::string getTime() {
    auto startTime = std::chrono::system_clock::now();
    std::time_t startTimeT = std::chrono::system_clock::to_time_t(startTime);

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&startTimeT), "%Y-%m-%d %H:%M:%S");

    return oss.str();
}
