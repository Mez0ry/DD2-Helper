#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__
#include <Windows.h>
#include <optional>
#include <string>
#include <vector>
#include <functional>
#include <future>
#include <atomic>

class Window{
private:
    std::wstring m_WindowTitle, m_PrevWindowTitle;
    HWND m_WindowHandle, m_PrevWindowHandle;
    std::optional<DWORD> m_ProcessId{std::nullopt};
public:
    Window() = default;

    Window(HWND window_handle) : m_WindowHandle(window_handle){

    }

    Window(std::wstring window_title, HWND window_handle, DWORD process_id) : m_WindowTitle(window_title), m_WindowHandle(window_handle), m_ProcessId(process_id){

    }

    Window(const Window& other)
        : m_WindowTitle(other.m_WindowTitle)
        , m_PrevWindowTitle(other.m_PrevWindowTitle)
        , m_WindowHandle(other.m_WindowHandle)
        , m_ProcessId(other.m_ProcessId)
    {}

    static std::vector<std::shared_ptr<Window>> FindAll(std::wstring window_title);
    static Window Find(std::wstring window_title);

    std::wstring GetTitle() const {
        return m_WindowTitle;
    }

    std::wstring GetPrevTitle() const {
        return m_PrevWindowTitle;
    }

    HWND GetWindowDescriptor() const {
        return m_WindowHandle;
    }

    bool IsValid() const{
        return IsWindow(m_WindowHandle);
    }

    operator bool() const{
        return IsValid();
    }

    operator HWND() const{
        return m_WindowHandle;
    }

    operator HWND(){
        return m_WindowHandle;
    }

    void SetWindowTitle(const std::wstring& window_title){
        m_PrevWindowTitle = m_WindowTitle;

        if(!SetWindowTextW((*this), window_title.c_str())){
            this->m_WindowTitle = window_title;
        }
    }

    bool IsForegrounded() const {
        return (*this) == GetForegroundWindow();
    }
};

class WindowList{
public:
    void AddWindow(std::shared_ptr<Window> window);
    void AddWindow(std::vector<std::shared_ptr<Window>> window);
    void RemoveWindow(std::shared_ptr<Window> window);

    auto begin() {return m_Windows.begin();}
    auto end(){return m_Windows.end();}

private:
    std::vector<std::shared_ptr<Window>> m_Windows;
};
#endif //!__WINDOW_HPP__