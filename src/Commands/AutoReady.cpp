#include "AutoReady.hpp"
#include "Player.hpp"
#include "KeySequence.hpp"
#include "Window.hpp"
#include "UserConfig.hpp"

AutoReady::AutoReady(KeySequence sequence, PlayerList &list, uint16_t hold_time_ms, uint16_t button_released_time_ms) : SendPlayersInput(User::GetInstance()->GetKeyBind("ReadyButton"), list, hold_time_ms, button_released_time_ms), m_KeySequence(sequence), m_PlayerList(list)
{

}

void AutoReady::Execute() {
    if(m_KeySequence.IsKeysPressed()){
        this->ToogleAutoReady();
    }

    if(IsAutoReady()){
        SendPlayersInput::Execute();
    }
}

bool AutoReady::IsAutoReady() const
{
    return m_AutoReady.load();
}

void AutoReady::ToogleAutoReady()
{
    m_AutoReady.store(!IsAutoReady());
}
