#include <windows.h>

int startProcess(char *process_path, int &return_code)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    if (!CreateProcess(NULL, process_path, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        return GetLastError();
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    GetExitCodeProcess(pi.hProcess, (LPDWORD)&return_code);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}