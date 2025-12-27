#include "App.hpp"
#include "KeyboardInput.hpp"
#include "MouseInput.hpp"
#include "UserConfig.hpp"
#include "JsonFileWatcher.hpp"
#include "KeySequence.hpp"

#include <iostream>
#include <algorithm>
#include <thread>

App::App() : m_bIsRunning(true), m_HostWindow(Window::Find(L"Dungeon Defenders 2")){
    
    m_AltWindows = Window::FindAll(L"[#] Dungeon Defenders 2 [#]");
}

App::~App() {

}

void App::Run() {
    JsonFileWatcher user_config_watcher("./resources/user/user.json");
    user_config_watcher.StartMonitoring([&user_config_watcher]() {
        User::GetInstance()->ParseConfig(user_config_watcher.GetFilePath());
    });

    while(m_bIsRunning){
       HandleInput();
       Update();
       Render();

       Sleep(50);
    }
}

void App::HandleInput()
{
    auto user_ready_button = User::GetInstance()->GetKeyBind("ReadyButton");
    auto user_retry_button = User::GetInstance()->GetKeyBind("RetryButton");
    auto initiate_buff = User::GetInstance()->GetKeyBind("InitiateBuff");

    if (user_ready_button.IsKeysPressed())
    {
        user_ready_button.PressKeys(m_HostWindow, 50, 50);

        for (auto &win : m_AltWindows)
        {
            user_ready_button.PressKeys(win, 50, 50);
        }

    }else if(user_retry_button.IsKeysPressed()){
        user_retry_button.PressKeys(m_HostWindow, 50, 50);

        for (auto &win : m_AltWindows)
        {
            user_retry_button.PressKeys(win, 50, 50);
        }
    }else if(initiate_buff.IsKeysPressed()){

        if(m_HostWindow.IsForegrounded() && !m_HostWindow.IsBuffing()){
            m_HostWindow.StartInitiateBuffing();
        }else if(m_HostWindow.IsBuffing()){
            m_HostWindow.StopInitiateBuffing();
        }

        for(auto& win : m_AltWindows){
            if(win.IsForegrounded() && !win.IsBuffing()){
                win.StartInitiateBuffing();
            }else if(win.IsBuffing()){
                win.StopInitiateBuffing();
            }
        }
    }
}

void App::Update()
{
 
}

void App::Render() {

}
