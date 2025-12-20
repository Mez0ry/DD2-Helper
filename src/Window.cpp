#include "Window.hpp"
#include "Process.hpp"

std::vector<Window> Window::FindAll(std::wstring window_title){
    auto processes = Process::GetProcesses();
    std::vector<Window> windows;
    
    for(const auto& process_id : processes){
        const auto &proc_titles = Process::GetProcessTitle(process_id);
        for(auto& title : proc_titles){
            if(title == window_title){
                windows.push_back(Window(window_title, Process::GetProcessHWNDById(process_id), process_id));
            }
        }
    }

    return windows;
}