#ifndef __APP_HPP__
#define __APP_HPP__
#include <vector>

#include "Window.hpp"

class App
{
private:
    bool m_bIsRunning;
    Window m_HostWindow;
    std::vector<Window> m_AltWindows;
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