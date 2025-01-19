#include "background.h"
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#endif


#ifdef _WIN32
ProcessResult runProcess(const std::string programm, bool startInBackgroung) {
    ProcessResult result = { -1, -1 }; // Initail value (error, exitCode -1)
    
    if (programm.empty()) {
        return result; 
    }

    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);

    if (!CreateProcessA(
            NULL, mutableProgramm.data(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        return result;
    }

    CloseHandle(pi.hThread);

    if (!startInBackgroung) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        DWORD status;
        if (!GetExitCodeProcess(pi.hProcess, &status)) {
            CloseHandle(pi.hProcess);
            return result; 
        }
        result.exitCode = static_cast<int>(status);
        result.status = 0;
        CloseHandle(pi.hProcess);
    } else {
        result.status = 0;
    }

    return result;
}

#else

ProcessResult runProcess(const std::string programm, bool startInBackgroung) {
    ProcessResult result = { -1, -1 }; // Initial value (error, exitCode -1)

    pid_t pid = fork();

    if (pid < 0) {
        return result;
    } else if (pid == 0) {
        execl("/bin/sh", "sh", "-c", programm.c_str(), static_cast<char*>(nullptr));
        exit(errno);
    } else {
        if (!startInBackgroung) {
            int status;

            if (waitpid(pid, &status, 0) == -1) {
                return result;
            }

            if (WIFEXITED(status)) {
                result.exitCode = WEXITSTATUS(status);
                result.status = 0;
            } else {
                result.exitCode = -1;
                result.status = -1;
            }

        } else {
            result.status = 0;
        }
    }

    return result;
}

#endif
