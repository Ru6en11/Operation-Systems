#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <string>

struct ProcessResult {
    int status;     // 0 - success, -1 - error
    int exitCode;
};

ProcessResult runProcess(const std::string programm, bool startInBackgroung);

#endif // BACKGROUND_H
