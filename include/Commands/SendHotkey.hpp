#ifndef __SEND_HOTKEY_HPP__
#define __SEND_HOTKEY_HPP__
#include "HotKeySequence.hpp"
#include "ICommand.hpp"
#include <memory>

class Player;

class SendHotkey : public ICommand{
private:
    std::shared_ptr<Player> m_Player;
    HotKeySequence m_Hotkey;
public:
    SendHotkey(std::shared_ptr<Player> player, const HotKeySequence& hotkey);

    void Execute() override;
private:
};

#endif // __SEND_HOTKEY_HPP__
