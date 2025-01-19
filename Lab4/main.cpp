#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#endif

std::string LOG_FILE = "log.txt";
std::string AVERAGE_PER_HOUR = "hourly_average.txt";
std::string AVERAGE_PER_DAY = "daily_average.hour.txt";

int SECONDS_IN_HOUR = 3600;
int SECONDS_IN_DAY = SECONDS_IN_HOUR * 24;
int SECONDS_IN_MONTH = SECONDS_IN_DAY * 30;
int SECONDS_IN_YEAR = SECONDS_IN_DAY * 365;

std::mutex recordLogMutex;
std::mutex deleteMutex;
std::mutex getMutex;
std::mutex logFileMutex;

struct TemperatureLog {
  int temperature;
  unsigned int timestemp; 
};

std::vector<TemperatureLog> findTemperatureForLastSeconds(std::string filename, int lastSeconds) {
  std::lock_guard<std::mutex> lock(getMutex);

  std::vector<TemperatureLog> dataList;
  std::ifstream file(filename, std::ios::in);
  
  if (!file.is_open()) {
    std::cout << "Failed to open file " << filename << std::endl;

    return dataList;
  }

  TemperatureLog data; 
  unsigned int currentTime = static_cast<unsigned int>(std::time(nullptr));

  while (file >> data.temperature >> data.timestemp) {
    if (data.timestemp < currentTime - lastSeconds) {
      continue;
    }
    dataList.push_back(data);
  }

  file.close();

  return dataList;
}

void deleteOldLogsIfNeeded(std::string filename, int deleteUntill) {
  std::lock_guard<std::mutex> lock(deleteMutex);
  std::vector<TemperatureLog> actualData = findTemperatureForLastSeconds(filename, deleteUntill);

  for (auto& data : actualData) {
    std::cout << data.temperature << " " << data.timestemp << std::endl;
  }

  std::ofstream file(filename, std::ios::trunc);
  if (!file.is_open()) {
    std::cout << "Failed to open file " << filename << " for re-write";
    return;
  }

  for (auto& data : actualData) {

    file << data.temperature << " " << data.timestemp << std::endl;
  }

  file.close();

}

void recordLog(TemperatureLog data) {
  std::lock_guard<std::mutex> lock(recordLogMutex);
  std::ofstream logFile(LOG_FILE, std::ios::app);

  logFile << data.temperature << " " << data.timestemp << std::endl;

  logFile.close();
  
  deleteOldLogsIfNeeded(LOG_FILE, SECONDS_IN_HOUR);
}

#ifdef _WIN32

bool setNonBlockingMode(HANDLE& hComm) {
    COMMTIMEOUTS timeouts;

    if (!GetCommTimeouts(hComm, &timeouts)) {
        std::cout << "Failed to get port timeouts." << std::endl;

        return false;
    }

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    if (!SetCommTimeouts(hComm, &timeouts)) {
        std::cout << "Failed to set port timeouts." << std::endl;

        return false;
    }

    return true;
}

void listenPort(const std::string& port) {

    HANDLE hComm = CreateFile(port.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

    if (hComm == INVALID_HANDLE_VALUE) {
        DWORD dwError = GetLastError();

        std::cout << "Failed to open port " << port << ". Code: " << dwError << std::endl;

        return;
    }

    DWORD dwError;

    if (!ClearCommError(hComm, &dwError, NULL)) {
        std::cout << "Failed to clear errors for port " << port << ". Code: " << dwError << std::endl;
        CloseHandle(hComm);

        return;
    }

    if (!setNonBlockingMode(hComm)) {
        std::cout << "Failed to set non-blocking mode." << std::endl;
        CloseHandle(hComm);

        return;
    }

    char byte;
    std::string buffer;

    while (1) {
        DWORD bytesRead;
        BOOL result = ReadFile(hComm, &byte, 1, &bytesRead, NULL);

        if (!result) {
            DWORD dwError = GetLastError();

            if (dwError == ERROR_IO_PENDING) {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));

                continue;
            } else {
                std::cout << "Failed to reading from port. Code: " << dwError << std::endl;
                break;
            }
        }

        buffer += byte;

        if (byte == '\n') {
            std::cout << "Received: " << buffer << std::endl;

            std::istringstream iss(buffer);
            TemperatureLog data;

            if (iss >> data.temperature) {
                data.timestemp = static_cast<unsigned int>(std::time(nullptr));
                recordLog(data);
            } else {
                std::cout << "Incorrect data format: " << buffer << std::endl;
            }

            buffer.clear();
        }
    }

    CloseHandle(hComm);
}


#else
void setNonBlockingMode(int fd) {
    fcntl(fd, F_SETFL, O_NONBLOCK); 
}

void listenPort(std::string& port) {

    int fd = open(port.c_str(), O_RDONLY | O_NONBLOCK);

    if (fd == -1) {
        std::cout << "Failed to open port for read data" << std::endl;

        return;
    }

    setNonBlockingMode(fd); 

    char byte;
    std::string buffer; 

    while (1) {

        ssize_t bytesRead = read(fd, &byte, 1);

        if (bytesRead == -1) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            continue;
        }

        buffer += byte;

        if (byte == '\n') {
            std::cout << "Received: " << buffer << std::endl;

            std::istringstream iss(buffer);
            TemperatureLog data;

            if (iss >> data.temperature) {
                data.timestemp = static_cast<unsigned int>(std::time(nullptr));
                recordLog(data);
            } else {
                std::cout << "Incorrect data format: " << buffer << std::endl;
            }

            buffer.clear();
        }
    }

    close(fd);
}
#endif

void recordAverageTemperatureForLastSeconds(std::string filename, int lastSeconds) {
  std::lock_guard<std::mutex> lock(logFileMutex);

  std::vector<TemperatureLog> dataList = findTemperatureForLastSeconds(LOG_FILE, lastSeconds);
  
  std::ofstream logFile(LOG_FILE, std::ios::app);

  unsigned int currentTime = static_cast<unsigned int>(std::time(nullptr));
  double average = 0;
  unsigned int summ = 0;

  if (dataList.size() == 0) {
    logFile << average << " " << currentTime;

    return;
  }

  for (auto& data : dataList) {
    summ += data.timestemp;
  }

  average = static_cast<double>(summ) / dataList.size();

  logFile << average << " " << currentTime; 

  logFile.close();
}

void recordAverageTemperaturePerHour() {
  while (1) {
    std::this_thread::sleep_for(std::chrono::seconds(SECONDS_IN_HOUR));

    recordAverageTemperatureForLastSeconds(AVERAGE_PER_HOUR, SECONDS_IN_HOUR);
    deleteOldLogsIfNeeded(AVERAGE_PER_HOUR, SECONDS_IN_MONTH);
  }
}

void recordAverageTemperaturePerMonth() {
  while (1) {
    std::this_thread::sleep_for(std::chrono::seconds(SECONDS_IN_DAY));

    recordAverageTemperatureForLastSeconds(AVERAGE_PER_DAY, SECONDS_IN_DAY);
    deleteOldLogsIfNeeded(AVERAGE_PER_DAY, SECONDS_IN_YEAR);
  }
}

int main(int argc, char* argv[]) {

  if (argc <= 1) {
    std::cout << "The port must be passed" << std::endl;

    return 0;
  }

  std::string port = argv[1];

  std::thread listenPortThread(listenPort, std::ref(port));
  std::thread recordAverageEveryHour(recordAverageTemperaturePerHour);
  std::thread recordAverageEveryMonth(recordAverageTemperaturePerMonth);

  recordAverageEveryHour.detach();
  recordAverageEveryMonth.detach();
  listenPortThread.join();

  return 0;
}
