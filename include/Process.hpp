#ifndef __PROCESS_HPP__
#define __PROCESS_HPP__
#include <Windows.h>
#include <tchar.h>
#include <psapi.h>
#include <vector>
#include <TlHelp32.h>
#include <string>

class Process{
public:
    static std::vector<DWORD> GetProcesses();
    static std::string GetProcessName(DWORD process_id);
    static std::vector<std::wstring> GetProcessTitle(DWORD process_id);
    static HWND GetProcessHWNDById(DWORD process_id);
private:
    struct EnumCallbackData{
        DWORD proc_id;
        HWND hwnd;
        std::vector<std::wstring> win_titles;
    };

    static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
};
#endif //!__PROCESS_HPP__
