#ifndef UTILS_H
#define UTILS_H

#include <string>

bool isMainProcess();

void closeSemaphore();

unsigned int getPid();

std::string getTime();

#endif //UTILS_H
