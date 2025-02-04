#pragma once
#include <string>

int getCurrentPID();

std::string getCurrentTime();

std::string getExecutablePath();

int startCopy(std::string args);

bool isProcessExists(int pid);