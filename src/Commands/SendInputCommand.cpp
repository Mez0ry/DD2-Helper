#include "SendInputCommand.hpp"

#include "Player.hpp"
#include "KeySequence.hpp"
#include "Window.hpp"
#include "UserConfig.hpp"

SendInputCommand::SendInputCommand(KeySequence sequence, std::shared_ptr<Player> player, uint16_t hold_time_ms, uint16_t button_released_time_ms) : m_KeySequence(sequence), m_Player(player), m_HoldTime(hold_time_ms), m_ReleaseTime(button_released_time_ms)
{

}

void SendInputCommand::Execute() {
    if(!m_Player){
        return;
    }

    auto player_window = m_Player->GetWindow();
    m_KeySequence.PressKeys(player_window->GetWindowDescriptor(), m_HoldTime, m_ReleaseTime);
}

void SendInputCommand::SetPlayer(std::shared_ptr<Player> player)
{
    this->m_Player = player;
}

SendPlayersInput::SendPlayersInput(KeySequence sequence, PlayerList &list, uint16_t hold_time_ms, uint16_t button_released_time_ms) : SendInputCommand(sequence, nullptr, hold_time_ms, button_released_time_ms), m_PlayerList(list)
{

}

void SendPlayersInput::Execute() {
    for(auto& player : m_PlayerList){
        SendInputCommand::SetPlayer(player);
        SendInputCommand::Execute();
    }
    
}

std::shared_ptr<IConditionalCommand<SendPlayersInput>> MakeMirroredSendPlayersInput(const std::string &keybind_name, PlayerList &list, uint16_t hold_time_ms, uint16_t button_released_time_ms)
{
    return MakeConditionalCommand<SendPlayersInput>([keybind_name](){
        auto key_bind = User::GetInstance()->GetKeyBind(keybind_name);
        return key_bind.IsKeysPressed();
    }, User::GetInstance()->GetKeyBind(keybind_name), list);
}
