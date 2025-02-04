#include <iostream>
#include <string>
#include <unistd.h>
#include <string.h>

#include "lib/bg.hpp"

int main(int argc, char *argv[])
{
    using namespace std;
    cout << "Main process ID: " << getpid() << endl;
    char* subprogram = "./sub";
    char* sub_arg="0";

    if(argc > 2)
    {
        subprogram = argv[1];
        sub_arg=argv[2];
    }

    int return_code=0;
    startProcess(subprogram, sub_arg, return_code);

    cout << "Return code: " << return_code << endl;
        
    return 0;
}