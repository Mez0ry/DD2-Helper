#include "Window.hpp"
#include "Process.hpp"
#include <KeySequence.hpp>
#include <thread>
#include <atomic>
#include <future>
#include <iostream>
#include <random>

std::vector<std::shared_ptr<Window>> Window::FindAll(std::wstring window_title){
    auto processes = Process::GetProcesses();
    std::vector<std::shared_ptr<Window>> windows;
    
    for(const auto& process_id : processes){
        const auto &proc_titles = Process::GetProcessTitle(process_id);
        for(auto& title : proc_titles){
            if(title == window_title){
                windows.push_back(std::make_shared<Window>(window_title, Process::GetProcessHWNDById(process_id), process_id));
            }
        }
    }

    return windows;
}

Window Window::Find(std::wstring window_title)
{
    auto win_descriptor = FindWindowW(NULL, window_title.c_str());

    int length = GetWindowTextLengthW(win_descriptor);
    if (length == 0) {
        std::wcout << L"Window has no title or handle is invalid." << std::endl;
    }

    std::vector<wchar_t> title(length + 1);

    GetWindowTextW(win_descriptor, title.data(), length + 1);

    DWORD proc_id;
    GetWindowThreadProcessId(win_descriptor, &proc_id);
    return Window(std::wstring(title.data()), win_descriptor, proc_id);
}

void WindowList::AddWindow(std::shared_ptr<Window> window)
{
    m_Windows.push_back(window);
}

void WindowList::AddWindow(std::vector<std::shared_ptr<Window>> windows) {
    for(auto& win : windows){
        m_Windows.push_back(win);
    }
}

void WindowList::RemoveWindow(std::shared_ptr<Window> window) {

}
