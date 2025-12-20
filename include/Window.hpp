#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__
#include <Windows.h>
#include <optional>
#include <string>
#include <vector>

class Window{
private:
    std::wstring m_WindowTitle;
    HWND m_WindowHandle;
    std::optional<DWORD> m_ProcessId{std::nullopt};
public:
    Window() = default;

    Window(HWND window_handle) : m_WindowHandle(window_handle){

    }

    Window(std::wstring window_title, HWND window_handle, DWORD process_id) : m_WindowTitle(window_title), m_WindowHandle(window_handle), m_ProcessId(process_id){

    }

    static std::vector<Window> FindAll(std::wstring window_title);

    std::wstring GetTitle() const {
        return m_WindowTitle;
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

private:
};
#endif //!__WINDOW_HPP__