#include <windows.h>
#include <string>

int getCurrentPID(){
    return GetCurrentProcessId();
}

std::string getCurrentTime()
{
	auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
	string time = ctime(&now);
	time.pop_back();
	return time;
}

std::string getExecutablePath()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, (char *)buffer, MAX_PATH);
	return std::string(buffer);
}

int startCopy(std::string args)
{
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	string str = getExecutablePath() + args;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	if (!CreateProcessA(NULL, (char *)str.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		return GetLastError();
	}
	return pi.dwProcessId;
}

bool isProcessExists(int pid)
{
	HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	if (processHandle == NULL)
	{
		return false;
	}

	DWORD exitCode;
	if (GetExitCodeProcess(processHandle, &exitCode))
	{
		CloseHandle(processHandle);
		return exitCode == STILL_ACTIVE;
	}
	CloseHandle(processHandle);
	return false;
}