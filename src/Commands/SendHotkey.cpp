#include "SendHotkey.hpp"
#include "Player.hpp"
#include "Window.hpp"

SendHotkey::SendHotkey(std::shared_ptr<Player> player, const HotKeySequence& hotkey) : ICommand(ExecutionPolicy::DeleteAfterExecution), m_Player(player), m_Hotkey(hotkey)
{

}

void SendHotkey::Execute() {
    m_Hotkey.PressKeys(*m_Player->GetWindow());
}
