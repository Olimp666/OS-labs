#include <iostream>
#include <unistd.h>
#include <spawn.h>
#include <string>
#include <string.h>
#include <sys/wait.h>

// #include "lib/bg.hpp"

char** env;

int startProcess(char *process_path, int &return_code)
{
    using namespace std;
    pid_t child_pid;

    char *argv[]{process_path, "12",NULL};

    int status = posix_spawn(&child_pid, process_path, NULL, NULL,argv,env);

    int wstatus;
    waitpid(child_pid,&wstatus,NULL);
    return_code=WEXITSTATUS(wstatus);
    return 0;
}

int main(int argc, char *argv[])
{
    using namespace std;
    cout << "Main process ID: " << getpid() << endl;
    char* subprogram = "./sub";

    if(argc > 1)
    {
        string command="";
        for(int i=1; i < argc; ++i)
        {
            command += argv[i];
            command += " ";
        }
        subprogram = new char[command.length() + 1];
        strcpy(subprogram, command.c_str());
    }

    int return_code=0;
    startProcess(subprogram, return_code);

    cout << "Return code: " << return_code << endl;
        
    return 0;
}