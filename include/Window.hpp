#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__
#include <Windows.h>
#include <optional>
#include <string>
#include <vector>
#include <memory>

class Window{
private:
    std::wstring m_WindowTitle, m_PrevWindowTitle;
    HWND m_WindowHandle, m_PrevWindowHandle;
    std::optional<DWORD> m_ProcessId{std::nullopt};
public:
    Window() = default;
    Window(HWND window_handle);
    Window(std::wstring window_title, HWND window_handle, DWORD process_id);

    Window(const Window& other);

    static std::vector<std::shared_ptr<Window>> FindAll(std::wstring window_title);
    static Window Find(std::wstring window_title);

    std::wstring GetTitle() const;
    std::wstring GetPrevTitle() const;
    HWND GetWindowDescriptor() const;

    bool IsValid() const;

    void SetWindowTitle(const std::wstring& window_title);
    bool IsForegrounded() const;
public:// Implicit conversions
    operator bool() const{
        return IsValid();
    }

    operator HWND() const{
        return m_WindowHandle;
    }

    operator HWND(){
        return m_WindowHandle;
    }
};

class WindowList{
public:
    void AddWindow(std::shared_ptr<Window> window);
    void AddWindow(std::vector<std::shared_ptr<Window>> window);
    void RemoveWindow(std::shared_ptr<Window> window);

    std::shared_ptr<Window> GetWindowAt(const std::size_t index) const;

    auto begin() {return m_Windows.begin();}
    auto end(){return m_Windows.end();}

    void Clear();

    std::size_t Size() const {return m_Windows.size();}

    operator bool() const{
        return !m_Windows.empty();
    }

    auto operator[](const std::size_t index){
        return GetWindowAt(index);
    }

private:
    std::vector<std::shared_ptr<Window>> m_Windows;
};
#endif //!__WINDOW_HPP__
