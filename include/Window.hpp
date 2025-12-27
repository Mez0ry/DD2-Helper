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
    std::vector<std::function<void()>> m_Actions;
    std::atomic<bool> m_IsBuffing{false};
    std::vector<std::future<void>> m_Futures;

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
        , m_Actions(other.m_Actions)
        , m_IsBuffing(false)
        , m_Futures() 
    {}

    static std::vector<Window> FindAll(std::wstring window_title);
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

    void SetWindowTitle(const std::wstring& window_title){
        m_PrevWindowTitle = m_WindowTitle;

        if(!SetWindowTextW((*this), window_title.c_str())){
            this->m_WindowTitle = window_title;
        }
    }

    bool IsForegrounded() const {
        return (*this) == GetForegroundWindow();
    }
    
public:
    bool IsBuffing() const{return m_IsBuffing.load();}
    void StartInitiateBuffing();
    void StopInitiateBuffing();
private:
};
#endif //!__WINDOW_HPP__