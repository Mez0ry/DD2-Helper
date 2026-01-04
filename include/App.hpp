#ifndef __APP_HPP__
#define __APP_HPP__
#include <vector>
#include <atomic>
#include "Window.hpp"
#include "Commands/ICommand.hpp"
#include "Player.hpp"

class App
{
private:
    bool m_bIsRunning;
    std::shared_ptr<Window> m_HostWindow;
    std::vector<std::shared_ptr<Window>> m_AltWindows;
    WindowList m_WindowList;
    PlayerList m_PlayerList;
    std::atomic<bool> m_IsInitiateBuffing{false};
    std::vector<std::shared_ptr<ICommand>> m_Commands;
    
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