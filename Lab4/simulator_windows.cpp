#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <chrono>
#include <windows.h>

bool setCommParams(HANDLE hComm) {
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hComm, &dcbSerialParams)) {
        std::cout << "Failed to get COM port state." << std::endl;

        return false;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hComm, &dcbSerialParams)) {
        std::cout << "Failed to set COM port state." << std::endl;

        return false;
    }

    return true;
}

void simulate_temperature_device(const std::string& port) {
    HANDLE hComm = CreateFile(port.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    
    if (hComm == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to open port: " << port << std::endl;

        return;
    }

    if (!setCommParams(hComm)) {
        CloseHandle(hComm);

        return;
    }

    int temperature = 20;
    char buffer[256];

    while (1) {
        temperature += (std::rand() % 8 - 2);
        
        std::snprintf(buffer, sizeof(buffer), "%d\n", temperature);

        DWORD bytesWritten;
        BOOL result = WriteFile(hComm, buffer, std::strlen(buffer), &bytesWritten, NULL);
        
        if (!result) {
            std::cout << "Failed to send data in port." << std::endl;
            break;
        }

        std::cout << "Sended: " << temperature << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    CloseHandle(hComm);
}

int main(int argc, char* argv[]) {

    if (argc <= 1) {
        std::cout << "The port must be passed" << std::endl;

        return 0;
    }

    const std::string port = argv[1];

    simulate_temperature_device(port);

    return 0;
}
