#ifndef __APP_HPP__
#define __APP_HPP__
#include <vector>
#include <atomic>
#include "Window.hpp"

class App
{
private:
    bool m_bIsRunning;
    Window m_HostWindow;
    std::vector<Window> m_AltWindows;
    std::atomic<bool> m_IsInitiateBuffing{false};
public:
    App();
    ~App();
    
    void Run();

private:
    void HandleInput();
    void Update();
    void Render();
private:
};
#endif //!__APP_HPP__