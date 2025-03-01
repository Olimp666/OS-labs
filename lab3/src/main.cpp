#include <fstream>
#include <iostream>
#include <chrono>
#include <string>
#include <thread>
#include <ctime>

#include "my_shmem.hpp"
#include "util.hpp"

#define ll long long
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

struct Data
{
	ll val = 0;
	int parent_pid = -1;
};

bool is_running = true;

enum PROCESS_TYPE
{
	MAIN,
	COPYA,
	COPYB
};

int pid = getCurrentPID();
int parent_pid = getCurrentPID();
PROCESS_TYPE ptype;

cplib::SharedMem<Data> getShmem()
{
	return cplib::SharedMem<Data>("shared");
}

void write(string str)
{
	ofstream fout("file.log", ios::app);
	fout << getCurrentTime() << " PID: " << pid << " " << str << endl;
	fout.close();
}

void copy_thread()
{
	int copyA_pid = 0;
	int copyB_pid = 0;
	auto sleep_duration = chrono::seconds(3);
	while (true)
	{
		this_thread::sleep_for(sleep_duration);
		if (copyA_pid == 0)
		{
			copyA_pid = startCopy(" COPYA " + to_string(pid));
		}
		else
		{
			if (isProcessExists(copyA_pid))
			{
				write("Copy A with PID " + to_string(copyA_pid) + " is still active!");
			}
			else
			{
				copyA_pid = 0;
			}
		}
		if (copyB_pid == 0)
		{
			copyB_pid = startCopy(" COPYB " + to_string(pid));
		}
		else
		{
			if (isProcessExists(copyB_pid))
			{
				write("Copy B with PID " + to_string(copyB_pid) + " is still active!");
			}
			else
			{
				copyB_pid = 0;
			}
		}
	}
}

void log_thread()
{
	auto shmem = getShmem();

	shmem.Lock();
	int log_pid = shmem.Data()->parent_pid;
	shmem.Unlock();

	auto sleep_duration = chrono::seconds(1);
	while (is_running)
	{
		if (log_pid != parent_pid)
		{
			if (!isProcessExists(log_pid))
			{
				shmem.Lock();
				shmem.Data()->parent_pid = parent_pid;
				shmem.Unlock();
				log_pid = parent_pid;
			}
			this_thread::sleep_for(sleep_duration);
			continue;
		}
		shmem.Lock();
		write("Value: " + to_string(shmem.Data()->val));
		shmem.Unlock();
		this_thread::sleep_for(sleep_duration);
	}
}

void ctr_thread()
{
	auto shmem = getShmem();
	auto sleep_duration = chrono::milliseconds(300);
	while (is_running)
	{
		this_thread::sleep_for(sleep_duration);
		shmem.Lock();
		shmem.Data()->val++;
		shmem.Unlock();
	}
}

// argv[1]=ptype
// argv[2]=parent_pid
int main(int argc, char *argv[])
{

	if (argc != 1 && argc != 3)
	{
		return 1;
	}

	ptype = MAIN;
	if (argc > 1)
	{
		if (!strcmp(argv[1], "MAIN"))
			ptype = MAIN;
		if (!strcmp(argv[1], "COPYA"))
		{
			ptype = COPYA;
			parent_pid = stoi(argv[2]);
		}
		if (!strcmp(argv[1], "COPYB"))
		{
			ptype = COPYB;
			parent_pid = stoi(argv[2]);
		}
	}
	string ptype_str = (ptype == MAIN) ? "MAIN" : ptype == COPYA ? "COPYA"
																 : "COPYB";

	auto shmem = getShmem();

	write("Joined! " + ptype_str);
	if (ptype == COPYA)
	{
		shmem.Lock();
		shmem.Data()->val += 10;
		shmem.Unlock();
	}
	if (ptype == COPYB)
	{
		shmem.Lock();
		shmem.Data()->val *= 2;
		shmem.Unlock();
		this_thread::sleep_for(chrono::seconds(2));
		shmem.Lock();
		shmem.Data()->val /= 2;
		shmem.Unlock();
	}
	if (ptype != MAIN)
	{
		shmem.~SharedMem();
		write("Exited! " + ptype_str);
		exit(0);
	}

	shmem.Lock();
	if (shmem.Data()->parent_pid == -1 && ptype == MAIN)
	{
		shmem.Data()->parent_pid = pid;
	}
	shmem.Unlock();

	thread log(log_thread);
	thread ctr(ctr_thread);
	thread copy;
	if (ptype == MAIN)
		copy = thread(copy_thread);

	while (true)
	{
		char param;
		cin >> param;
		if (param == 'e')
		{
			is_running = false;
			shmem.Lock();
			if (shmem.Data()->parent_pid == pid && ptype == MAIN)
			{
				shmem.Data()->parent_pid = -1;
			}
			shmem.Unlock();
			shmem.~SharedMem();
			log.join();
			ctr.join();
			write("Exited!!!! " + ptype_str);
			return 0;
		}
		if (param == 's')
		{
			ll new_val;
			cin >> new_val;
			if (new_val >= 0)
			{
				shmem.Lock();
				shmem.Data()->val = new_val;
				shmem.Unlock();
			}
		}
	}
}