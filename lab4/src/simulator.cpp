#include <iostream>
#include <thread>
#include <random>
#include <string>
#include <chrono>

#include "my_serial.hpp"

using namespace std;

string DEFAULT_PORT = "COM3";

int main(int argc, char *argv[])
{
    string port(DEFAULT_PORT);
    if (argc > 1)
    {
        port = argv[1];
    }

    cplib::SerialPort serial_port(port, cplib::SerialPort::BAUDRATE_115200);

    if (!serial_port.IsOpen())
    {
        cout << "Args: [1: port]; default: " << DEFAULT_PORT << endl;
        cerr << "Failed to open " << port << " port!" << endl;
        return -2;
    }
    cout << "Sending to port: " << port << endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution norm(0.0, 8.0);

    std::string out;
    for (;;)
    {
        auto now_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        out = "$$$$$" + std::to_string(now_c) + '_' + std::to_string(norm(gen)) + "$$$$$";
        serial_port << out;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}