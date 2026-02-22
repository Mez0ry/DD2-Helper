#include "Window.hpp"
#include "Process.hpp"

#include <qDebug>

Window::Window(HWND window_handle)
    : m_WindowHandle(window_handle)
{}

Window::Window(std::wstring window_title, HWND window_handle, DWORD process_id)
    : m_WindowTitle(window_title)
    , m_WindowHandle(window_handle)
    , m_ProcessId(process_id)
{}

Window::Window(const Window& other)
    : m_WindowTitle(other.m_WindowTitle)
    , m_PrevWindowTitle(other.m_PrevWindowTitle)
    , m_WindowHandle(other.m_WindowHandle)
    , m_ProcessId(other.m_ProcessId)
{}

std::vector<std::shared_ptr<Window>> Window::FindAll(std::wstring window_title)
{
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
        qDebug() << "Window not found, probably you didn't launched it";
    }

    std::vector<wchar_t> title(length + 1);

    GetWindowTextW(win_descriptor, title.data(), length + 1);

    DWORD proc_id;
    GetWindowThreadProcessId(win_descriptor, &proc_id);
    return Window(std::wstring(title.data()), win_descriptor, proc_id);
}

std::wstring Window::GetTitle() const
{
    return m_WindowTitle;
}

std::wstring Window::GetPrevTitle() const
{
    return m_PrevWindowTitle;
}

HWND Window::GetWindowDescriptor() const
{
    return m_WindowHandle;
}

bool Window::IsValid() const
{
    return IsWindow(m_WindowHandle);
}

void Window::SetWindowTitle(const std::wstring& window_title)
{
    m_PrevWindowTitle = m_WindowTitle;

    if (!SetWindowTextW((*this), window_title.c_str())) {
        this->m_WindowTitle = window_title;
    }
}

bool Window::IsForegrounded() const
{
    return (*this) == GetForegroundWindow();
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
    m_Windows.erase(std::remove_if(m_Windows.begin(), m_Windows.end(),
        [&](const std::shared_ptr<Window>& w) {
            return w == window;
        }),
    m_Windows.end());
}

std::shared_ptr<Window> WindowList::GetWindowAt(const std::size_t index) const
{
    return m_Windows[index];
}

void WindowList::Clear() {
    m_Windows.clear();
}
