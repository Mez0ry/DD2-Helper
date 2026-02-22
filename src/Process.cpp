#include "Process.hpp"
#include "HandleGuard.hpp"
#include <cstdint>

std::vector<DWORD> Process::GetProcesses()
{
    std::vector<DWORD>
    processes;

    const uint16_t size = 1024;
    DWORD processes_id[size], cb_needed, process_size;

    if (!EnumProcesses(processes_id, sizeof(processes_id), &cb_needed)) {
        return processes;
    }

    process_size = cb_needed / sizeof(DWORD);

    for (auto index = 0; index < process_size; index++) {
        processes.push_back(processes_id[index]);
    }

    return processes;
}

std::string Process::GetProcessName(DWORD process_id)
{
    TCHAR process_name[MAX_PATH] = TEXT("<unknown>");

    HandleGuard h_proc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process_id);

    if (h_proc != NULL) {
        HMODULE h_mod;
        DWORD cb_needed;

        if (EnumProcessModules(h_proc, &h_mod, sizeof(h_mod), &cb_needed)) {
            GetModuleBaseName(h_proc, h_mod, process_name, sizeof(process_name) / sizeof(TCHAR));
        }
    }

    std::wstring ws(process_name);
    return std::string(ws.begin(), ws.end());
}

std::vector<std::wstring>
Process::GetProcessTitle(DWORD process_id)
{
    EnumCallbackData data = {process_id, NULL};
    EnumWindows(EnumWindowsProc,
                reinterpret_cast<LPARAM>
                (&data));
    return data.win_titles;
}

HWND Process::GetProcessHWNDById(DWORD process_id)
{
    EnumCallbackData data = {process_id, NULL};
    EnumWindows(EnumWindowsProc,
                reinterpret_cast<LPARAM>
                (&data));
    return data.hwnd;
}

WINBOOL Process::EnumWindowsProc(HWND hwnd,  LPARAM lParam)
{
    EnumCallbackData* data = reinterpret_cast<EnumCallbackData*>
    (lParam);
    DWORD proc_id;
    GetWindowThreadProcessId(hwnd, &proc_id);

    if (proc_id == data->proc_id) {
        data->hwnd = hwnd;

        int length = GetWindowTextLengthW(hwnd);
        if (length > 0) {
            std::wstring title(length + 1, L'\0');
            GetWindowTextW(hwnd, &title[0], length + 1);

            title.resize(length);
            data->win_titles.push_back(title);
        }

        return FALSE;
    }

    return TRUE;
}
