#include "App.hpp"
#include "KeyboardInput.hpp"
#include "MouseInput.hpp"
#include "UserConfig.hpp"
#include "JsonFileWatcher.hpp"
#include "KeySequence.hpp"

#include <iostream>
#include <algorithm>
#include <thread>

#include "Player.hpp"
#include "AutoReady.hpp"
#include "InitiateBuff.hpp"
#include "DropBuildResource.hpp"

App::App() : m_bIsRunning(true), m_HostWindow(std::make_shared<Window>(Window::Find(L"Dungeon Defenders 2"))){
    
    m_AltWindows = Window::FindAll(L"[#] Dungeon Defenders 2 [#]");
    m_WindowList.AddWindow(m_HostWindow);
    m_WindowList.AddWindow(m_AltWindows);

    for(auto& win : m_WindowList){
        m_PlayerList.AddPlayer(std::make_shared<Player>(win));
    }
    
    auto user_auto_ready = User::GetInstance()->GetKeyBind("AutoReady");
    auto user_drop_mana_button = User::GetInstance()->GetKeyBind("DropGreenManaButton");

    auto initiate_buff = User::GetInstance()->GetKeyBind("InitiateBuff");
    
    m_Commands.push_back(MakeMirroredSendPlayersInput("RetryButton", m_PlayerList));
    m_Commands.push_back(MakeMirroredSendPlayersInput("ReadyButton", m_PlayerList));
    
    m_Commands.push_back(std::make_shared<AutoReady>(user_auto_ready, m_PlayerList));
    m_Commands.push_back(MakeConditionalCommand<InitiateBuff>([](){
        return User::GetInstance()->GetKeyBind("InitiateBuff").IsKeysPressed();
    }, m_PlayerList));

    m_Commands.push_back(std::make_shared<DropBuildResource>(m_PlayerList));
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
    for(auto& command : m_Commands){
        command->Execute();
    }
}

void App::Update()
{
 
}

void App::Render() {

}
