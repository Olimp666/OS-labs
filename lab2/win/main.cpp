#include <iostream>
#include <windows.h>

#include "lib/bg.hpp"

int main(int argc, char *argv[])
{
    using namespace std;
    cout << "Main process ID: " << GetCurrentProcessId() << endl;
    char *subprogram = ".\\sub.exe";

    if (argc > 1)
    {
        string command = "";
        for (int i = 1; i < argc; ++i)
        {
            command += argv[i];
            command += " ";
        }
        subprogram = new char[command.length() + 1];
        strcpy(subprogram, command.c_str());
    }

    int return_code;
    startProcess(subprogram, return_code);

    cout << "Return code: " << return_code << endl;
    return return_code;
}