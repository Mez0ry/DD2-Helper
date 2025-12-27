#include "Window.hpp"
#include "Process.hpp"
#include <KeySequence.hpp>
#include <thread>
#include <atomic>
#include <future>
#include <iostream>
#include <random>

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

void Window::StartInitiateBuffing() {
    if(m_IsBuffing.load()){
        return;
    }

    this->SetWindowTitle(GetTitle() + L" [Initiate buffing]");

    m_IsBuffing.store(true);

    m_Futures.push_back(std::async(std::launch::async,[this](){
        KeySequence press_seq("K3 LBUTTON");

        while(m_IsBuffing.load()){
            /**
             * @NOTE better way of sending input or smth that guarantees perfect execution of key sequence in-game to prevent send of multiple LButtons messages
             */
            press_seq.PressKeys(m_WindowHandle, 11, 1500);

            SendMessage(m_WindowHandle, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
            SendMessage(m_WindowHandle, WM_LBUTTONUP, 0, MAKELPARAM(0, 0));
            
            press_seq.PressKeys(m_WindowHandle, 11, 1100);

            SendMessage(m_WindowHandle, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
            SendMessage(m_WindowHandle, WM_LBUTTONUP, 0, MAKELPARAM(0, 0));
            
            std::this_thread::sleep_for(std::chrono::milliseconds(9000));
        }
    }));
}

void Window::StopInitiateBuffing() {
    this->SetWindowTitle(m_PrevWindowTitle);

    m_IsBuffing.store(false);
    for (auto& future : m_Futures) {
        if(future.valid()){
            future.get();
        }
    }

    m_Futures.clear();
}
