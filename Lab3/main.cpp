#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <thread>
#include <mutex>
#ifdef _WIN32
#include <windows.h>
#endif

#include "lib/shared_memory.h"
#include "lib/utils.h"
#include "lib/background.h"

std::mutex counterMutex;
std::mutex processMutex;
std::mutex logMutex;
std::mutex statusMutex;

std::string LOG_FILE = "log.txt";
std::ofstream logFile;

void log(std::string message) {
  std::lock_guard<std::mutex> lock(logMutex);
  if (!logFile.is_open()) {
    logFile.open(LOG_FILE, std::ios::app);
  }
  
  logFile << message << std::endl;
}

void setCounter(int newValue) {
  std::lock_guard<std::mutex> lock(counterMutex);
  
  SharedMemory* sharedMemory = getSharedMemory();
  sharedMemory->counter = newValue;
}

void incrementCounterEvery300ms() {
  SharedMemory* sharedMemory = getSharedMemory();

  while(1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    sharedMemory->counter += 1;
  }

}

void loggingStateEvery300ms() {
  SharedMemory* sharedMemory = getSharedMemory();

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    log("Process id: " + std::to_string(getPid()) + " Time: " + getTime() + " Counter " + std::to_string(sharedMemory->counter));
  }
}

void consoleUi() {
  SharedMemory* shared = getSharedMemory();

  while(1) {
    int newValue;

    std::cout << "Enter counter value: ";
    std::cin >> newValue;

    if (newValue < 0) {
      break;
    };

    log( "Counter value changed from " + std::to_string(shared->counter) + "to " + std::to_string(newValue));
  
    setCounter(newValue);
  }
}

void startCopy1Version() { 
  std::lock_guard<std::mutex> lock(processMutex);

  SharedMemory* sharedMemory = getSharedMemory();

  log("Process id: " + std::to_string(getPid()) + " Started at: " + getTime());

  sharedMemory->counter += 10;

  log("Process id: " + std::to_string(getPid()) + " Exited at: " + getTime());
  
  logFile.close();

  exit(0);
}

void startCopy2Version() {
  std::lock_guard<std::mutex> lock(processMutex);
  
  SharedMemory* sharedMemory = getSharedMemory();

  log("Process id: " + std::to_string(getPid()) + " Started at: " + getTime());

  sharedMemory->counter *= 2;

  std::this_thread::sleep_for(std::chrono::seconds(2));

  sharedMemory->counter /= 2;

  log("Process id: " + std::to_string(getPid()) + " Exited at: " + getTime());
  
  logFile.close();

  exit(0);

}

void startupWrapper(std::string appName) {
  bool isRunning[2] = {false, false};
  
  while (1) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    for (int i = 0; i < 2; i++) {
      std::lock_guard<std::mutex> lock(statusMutex);

      if (isRunning[i]) {
        log ("Process copy" + std::to_string(i+1) + " is still running. Skip...");
        continue;
      }

      std::string arg = (i==0) ? " copy1" : " copy2";
      
      isRunning[i] = true;

      std::thread([i, arg, &isRunning, appName]() {
        ProcessResult result = runProcess(appName + arg, false);
        
        if (result.status < 0) {
          std::cout << "Process " << arg << " failed";
        }

        isRunning[i] = false;
      }).detach();
    }

  }
}
  
int main(int argc, char* argv[]) {
  
  bool isMain = isMainProcess();
  
  SharedMemory* sharedMemory = getSharedMemory(isMain);

  if (!sharedMemory) {
    std::cout << "Failed to open shared memory" << std::endl;
    return 1;
  }  
  
  if (argc > 1) {
    std::string mode = argv[1];

    if (mode == "copy1") {
      startCopy1Version();
      
      return 0;
    } else if (mode == "copy2") {
      startCopy2Version();
        
      return 0;
    }

 }

  log("Process ID: " + std::to_string(getPid()) + " started at: " + getTime());
  
  std::thread incrementCounterWorker(incrementCounterEvery300ms);
  std::thread consoleUiWorker(consoleUi); 

  if (isMain) {
    std::thread loggintStateWorker(loggingStateEvery300ms);
    std::thread startupWrapperWorker(startupWrapper, argv[0]);
    loggintStateWorker.detach();
    startupWrapperWorker.detach();
  }
  incrementCounterWorker.detach();
  consoleUiWorker.join();
  

  if (isMain) {
    cleanSharedMemory();
    closeSemaphore();
  }

  logFile.close();
  return 0;
}
