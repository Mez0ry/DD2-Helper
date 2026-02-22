#include "Player.hpp"
#include "Window.hpp"

#include <UserConfig.hpp>
#include <algorithm>
#include "ICommand.hpp"
#include <QObject>
#include <QtConcurrent/QtConcurrent>

Player::Player(std::shared_ptr<Window> window) : m_Window(window)
{

}

Player::~Player(){
    StopInitiateBuffing();
}

void Player::RemoveCommand(ICommand *command)
{
    m_Commands.erase(std::remove_if(m_Commands.begin(),
                                    m_Commands.end(),
                                    [command](const std::shared_ptr<ICommand>
                                              &cmd) { return cmd.get() == command; }),
                     m_Commands.end());
}

void Player::StartInitiateBuffing()
{
    if(m_IsBuffing.load()){
        return;
    }

    m_IsBuffing.store(true);

    m_Futures.push_back(QtConcurrent::run([this](){
        auto win_descriptor = m_Window->GetWindowDescriptor();
        HotKeySequence press_seq("K3+LBUTTON");
        bool endless = true;

        while(endless){
            this->PerformInitiateBuffStep();

            while(!m_IsBuffing.load()){
                if(m_Futures.empty()){
                    endless = false;
                }
                QThread::msleep(2000);
            }
        }
    }));
}

void Player::StopInitiateBuffing() {
    m_IsBuffing.store(false);
    for (auto& future : m_Futures) {
        if(future.isValid()){
            future.cancel();
        }
    }

    m_Futures.clear();
}

void Player::SetBuffing(bool state) {
    m_IsBuffing.store(state);
}

bool Player::IsBuffing() const {
    return m_IsBuffing.load();
}

void Player::DropBuildResource(uint16_t hold_time_ms, uint16_t button_released_time_ms) {
    HotKeySequence key_sequence(UserConfig::GetInstance()->GetKeyBind("DropBuildResourceInGameBind"));
    key_sequence.PressKeys(m_Window->GetWindowDescriptor(), 50 , 50);
}

void Player::DropMana(uint16_t hold_time_ms, uint16_t button_released_time_ms) {
    HotKeySequence key_sequence(UserConfig::GetInstance()->GetKeyBind("DropAbilityManaInGameBind"));
    key_sequence.PressKeys(m_Window->GetWindowDescriptor(), 50 , 50);
}

void Player::StartAutoReady() {
    m_IsAutoReady.store(true);
}

void Player::StopAutoReady() {
    m_IsAutoReady.store(false);
}

void Player::SetAutoReady(bool state) {
    m_IsAutoReady.store(state);
}

bool Player::IsAutoReady() const { return m_IsAutoReady.load(); }

void Player::PerformInitiateBuffStep() {
    auto future = std::async(std::launch::async,[&](){
        if(!m_Window || m_Window.get() == nullptr){
            return;
        }
        auto win_descriptor = m_Window->GetWindowDescriptor();
        HotKeySequence press_seq("K3+LBUTTON");

        press_seq.PressKeys(win_descriptor, 11, 1500);

        SendMessage(win_descriptor, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
        SendMessage(win_descriptor, WM_LBUTTONUP, 0, MAKELPARAM(0, 0));

        press_seq.PressKeys(win_descriptor, 11, 1100);

        SendMessage(win_descriptor, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
        SendMessage(win_descriptor, WM_LBUTTONUP, 0, MAKELPARAM(0, 0));

        press_seq.PressKeys(win_descriptor, 11, 1100);

        SendMessage(win_descriptor, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
        SendMessage(win_descriptor, WM_LBUTTONUP, 0, MAKELPARAM(0, 0));

        press_seq.PressKeys(win_descriptor, 11, 600);

        SendMessage(win_descriptor, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
        SendMessage(win_descriptor, WM_LBUTTONUP, 0, MAKELPARAM(0, 0));
    });
}

void PlayerList::AddPlayer(std::shared_ptr<Player> player) {
    m_Players.push_back(player);
}

void PlayerList::AddPlayer(std::vector<std::shared_ptr<Player>> players) {
    m_Players.insert(m_Players.end(), players.begin(), players.end());
}

void PlayerList::RemovePlayer(std::shared_ptr<Player> player) {
    auto it = std::find(m_Players.begin(), m_Players.end(), player);
    if (it != m_Players.end()) {
        m_Players.erase(it);
    }
}

void PlayerList::ExecuteCommands()
{
    for (auto& player : m_Players) {
        auto& player_commands = player->GetCommands();
        player_commands.erase(std::remove_if(player_commands.begin(), player_commands.end(),
           [](const std::shared_ptr<ICommand>& cmd) {
               cmd->Execute();
               return cmd->GetExecutionPolicy() == ExecutionPolicy::DeleteAfterExecution;
           }),
        player_commands.end());
    }
}
