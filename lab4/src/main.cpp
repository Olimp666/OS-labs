#include <fstream>
#include <iostream>
#include <chrono>
#include <string>
#include <thread>
#include <vector>
#include <filesystem>
#include <ctime>

#include "my_serial.hpp"

#define ll long long

using namespace std;
namespace fs = std::filesystem;

string DEFAULT_PORT = "COM4";

string LOG_DIR = LOG_DIR + "logs";
string LOG_SEC_NAME = LOG_DIR + "/second.log";
string LOG_HOUR_NAME = LOG_DIR + "/hour.log";
string LOG_DAY_NAME = LOG_DIR + "/day.log";

ll HOUR_SEC = 10;
// ll HOUR_SEC = 60 * 60;
ll DAY_SEC = HOUR_SEC * 24;
ll MONTH_SEC = DAY_SEC * 30;
ll YEAR_SEC = DAY_SEC * 365;

struct record
{
    ll time;
    double temp;
};

struct update_time
{
    ll hour_time;
    ll day_time;
};

vector<string> splitString(string str, char delimeter = ' ')
{
    vector<string> split;
    istringstream iss(str);
    string s;
    while (getline(iss, s, ' '))
    {
        split.emplace_back(s);
    }
    return split;
}

double
getMeanTemp(string file_name, ll now, ll diff_sec)
{
    ifstream log(file_name);

    double mean = 0;
    uint32_t count = 1;

    string line;
    while (getline(log, line))
    {
        auto split = splitString(line);
        if (split.size() <= 0)
        {
            continue;
        }

        ll time = stoll(split[0]);
        if (now - time < diff_sec)
        {
            mean += stod(split[1]);
            break;
        }
    }

    while (getline(log, line))
    {
        auto split = splitString(line);
        mean += stod(split[1]);
        ++count;
    }

    return mean / count;
}

void tryCreateFile(string name)
{
    if (!fs::exists(name))
    {
        ofstream(name).close();
    }
}

ll getUnixTime()
{
    return chrono::system_clock::to_time_t(chrono::system_clock::now());
}

void setTracking(string name, ll &tracking)
{
    tryCreateFile(name);
    ifstream fi(name);
    fi.seekg(0, ios::end);
    tracking = getUnixTime();
    fi.close();
}

void writeTempToFile(string file_name, ll &time, double &temp, ll &now, ll lim_sec)
{
    string temp_name = LOG_DIR + "/temp.log";
    if (fs::exists(temp_name))
    {
        fs::remove(temp_name);
    }

    fs::copy_file(file_name, temp_name);
    ofstream of_log(file_name);
    ifstream temp_log(temp_name);

    string line;
    while (getline(temp_log, line))
    {
        auto split = splitString(line);
        if (split.size() <= 0)
        {
            continue;
        }

        ll time = stoll(split[0]);
        if (now - time < lim_sec)
        {
            break;
        }
    }

    if (!temp_log.eof())
    {
        of_log << line << '\n';
    }
    while (getline(temp_log, line))
    {
        of_log << line << '\n';
    }
    of_log << time << " " << temp << endl;

    of_log.close();
    temp_log.close();
    fs::remove(temp_name);
}

vector<record> parseTemp(string str)
{
    vector<record> result;
    size_t start = str.find('$');
    size_t end = str.rfind('$');
    for (int i = start; i < end; i++)
    {
        string data = "";
        while (str[i] == '$' && i < end)
        {
            i++;
        }
        while (str[i] != '$' && i < end)
        {
            data += str[i];
            i++;
        }
        if (data.size() == 0)
            break;
        size_t split = data.find('_');
        if (split != string::npos)
        {
            record r;
            r.time = stoll(data.substr(0, split));
            r.temp = stod(data.substr(split + 1));
            result.push_back(r);
        }
    }
    return result;
}

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

    cout << "Receiving from port: " << port << endl;
    serial_port.SetTimeout(5.0);

    update_time upd;

    fs::create_directory(LOG_DIR);
    tryCreateFile(LOG_SEC_NAME);

    setTracking(LOG_HOUR_NAME, upd.hour_time);
    setTracking(LOG_DAY_NAME, upd.day_time);

    ll now = getUnixTime();

    string str;
    while (true)
    {
        serial_port >> str;
        auto parsed_temp = parseTemp(str);
        for (int i = 0; i < parsed_temp.size(); i++)
        {
            now = getUnixTime();
            writeTempToFile(LOG_SEC_NAME, parsed_temp[i].time, parsed_temp[i].temp, now, DAY_SEC);
        }
    
        if (now - upd.hour_time >= HOUR_SEC)
        {
            double hour_mean = getMeanTemp(LOG_SEC_NAME, now, HOUR_SEC);
            writeTempToFile(LOG_HOUR_NAME,upd.hour_time, hour_mean, now, MONTH_SEC);
            upd.hour_time = now;
        }
        if (now - upd.day_time >= DAY_SEC)
        {
            double day_mean = getMeanTemp(LOG_HOUR_NAME, now, DAY_SEC);
            writeTempToFile(LOG_DAY_NAME,upd.day_time, day_mean, now, YEAR_SEC);
            upd.day_time = now;
        }
    }
}