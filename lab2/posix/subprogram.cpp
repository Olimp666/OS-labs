#include <iostream>
#include <unistd.h>
#include <thread>
#include <chrono>

int main(int argc, char *argv[])
{
    using namespace std;
    cout << "Child process ID: " << getpid() << endl;
        
    int return_code = argc > 1 ? stoi(argv[1]) : 0;
    for (int i = 3; i > 0; i--)
    {
        cout << i << "..." << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }

    return return_code;
}