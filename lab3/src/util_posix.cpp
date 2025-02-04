#include <string>
#include <chrono>
#include <ctime>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>
#include <unistd.h>
#include <signal.h>
#include <spawn.h>

int getCurrentPID()
{
    return getpid();
}

std::string getCurrentTime()
{
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string time = ctime(&now);
    time.pop_back();
    return time;
}

std::string getExecutablePath()
{
    char buffer[1024];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1)
    {
        buffer[len] = '\0';
        return std::string(buffer);
    }
    else
    {
        return std::string();
    }
}

int startCopy(std::string args)
{
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = SA_NOCLDWAIT; 
    sigaction(SIGCHLD, &sa, NULL);
    args.erase(0, args.find_first_not_of(' '));
    args.erase(args.find_last_not_of(' ') + 1);

    std::istringstream iss(args);
    std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
    char *arg1 = (char *)tokens[0].c_str();
    char *arg2 = (char *)tokens[1].c_str();
    pid_t child_pid;
    char *argv[]{(char *)getExecutablePath().c_str(), arg1, arg2, NULL};

    int status = posix_spawn(&child_pid, (char *)getExecutablePath().c_str(), NULL, NULL, argv, NULL);

    return child_pid;
}

bool isProcessExists(int pid)
{
    if (kill(pid, 0) == 0)
    {
        std::cout<<"Process with PID: "<<pid<<" exists"<<std::endl;
        return true;
    }
    else
    {
        return false;
    }
}