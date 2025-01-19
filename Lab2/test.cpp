#include "lib/background.h"
#include <iostream>
#include <string>

void testProcess(const std::string command, bool startInBackground) {

    ProcessResult result = runProcess(command, startInBackground);

    if (result.status == 0) {
        std::cout << "Command: " << command << "\n";
        if (!startInBackground) {
            std::cout << "Process finished with exit code: " << result.exitCode << "\n";
        } else {
            std::cout << "Process started in background.\n";
        }
    } else {
        std::cerr << "Failed to execute command: " << command << "\n";
    }
}

int main() {
    std::cout << "Starting tests for start_process function...\n\n";

    std::cout << "Test 1: Run 'echo Hello' and wait for completion\n";
    testProcess("echo Hello", false);

    std::cout << "\nTest 2: Run invalid command 'nonexistent_command'\n";
    testProcess("nonexistent_command", false);

    std::cout << "\nTest 3: Run 'sleep 2' without waiting\n";
    testProcess("sleep 2", true);

    std::cout << "\nTest 4: Run 'ls -l | grep cpp' and wait for completion\n";
#ifdef _WIN32
    testProcess("cmd /C dir", false);
#else
    testProcess("ls -l | grep cpp", false);
#endif

    std::cout << "\nTests completed.\n";
    return 0;
}

