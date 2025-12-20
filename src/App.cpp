#include "App.hpp"
#include "KeyboardInput.hpp"
#include "MouseInput.hpp"
#include "UserConfig.hpp"

#include <iostream>

App::App() : m_bIsRunning(true), m_HostWindow(FindWindowW(NULL, L"Dungeon Defenders 2")){
    
    m_AltWindows = Window::FindAll(L"[#] Dungeon Defenders 2 [#]");
}

App::~App() {

}

void App::Run() {
    while(m_bIsRunning){
       HandleInput();
       Update();
       Render();
    }
}

void App::HandleInput()
{
    auto any_type_button_pressed = [](VKeys key){
        return (MouseInput::IsButtonPressed(key) || KeyboardInput::IsKeyPressed(key));
    };
    
    auto user_ready_button = User::GetInstance()->GetKeyBind("ReadyButton");
    auto user_retry_button = User::GetInstance()->GetKeyBind("RetryButton");

    if (any_type_button_pressed(user_ready_button))
    {
        KeyboardInput::PressKey(m_HostWindow, user_ready_button, 50, 50);
        for (auto &win : m_AltWindows)
        {
            KeyboardInput::PressKey(win, user_ready_button, 50, 50);
        }
        
    }else if(any_type_button_pressed(user_retry_button)){
        KeyboardInput::PressKey(m_HostWindow, user_retry_button, 50, 50);

        for (auto &win : m_AltWindows)
        {
            KeyboardInput::PressKey(win, user_retry_button, 50, 50);
        }
    }
}

void App::Update()
{
   
}

void App::Render() {

}
