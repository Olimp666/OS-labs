#include <pthread.h>
#include <sys/wait.h>
#include <spawn.h>

int startProcess(char *process_path, char *arg, int &return_code)
{
    using namespace std;
    pid_t child_pid;

    char *argv[]{process_path, arg, NULL};

    int status = posix_spawn(&child_pid, process_path, NULL, NULL,argv,NULL);

    int wstatus;
    waitpid(child_pid,&wstatus,NULL);
    return_code=WEXITSTATUS(wstatus);
    return 0;
}