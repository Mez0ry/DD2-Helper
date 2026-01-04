#ifndef __SEND_INPUT_MIRROR_COMMAND_HPP__
#define __SEND_INPUT_MIRROR_COMMAND_HPP__
#include "ICommand.hpp"
#include <memory>
#include <atomic>
#include "KeySequence.hpp"

class PlayerList;
class KeySequence;
class Player;

class SendInputCommand : public ICommand
{
private:
    KeySequence m_KeySequence;
    std::shared_ptr<Player> m_Player;
    uint16_t m_HoldTime, m_ReleaseTime;
public:
    SendInputCommand  (KeySequence sequence, std::shared_ptr<Player> player, uint16_t hold_time_ms = 50, uint16_t button_released_time_ms = 50);
    ~SendInputCommand  () = default;

    void Execute() override;

    void SetPlayer(std::shared_ptr<Player> player);
};

class SendPlayersInput : public SendInputCommand  
{
private:
    PlayerList &m_PlayerList;
public:
    SendPlayersInput() = delete;
    SendPlayersInput(KeySequence sequence, PlayerList &list, uint16_t hold_time_ms = 50, uint16_t button_released_time_ms = 50);
    ~SendPlayersInput() = default;

    void Execute() override;
};

/**
 * @brief Makes mirrored send input (means that its going to check if specified keybind name was pressed and gonna send those pressed buttons input to players (mirror it))
 */
std::shared_ptr<IConditionalCommand<SendPlayersInput>> MakeMirroredSendPlayersInput(const std::string& keybind_name, PlayerList& list, uint16_t hold_time_ms = 50, uint16_t button_released_time_ms = 50);

#endif //!__SEND_INPUT_MIRROR_COMMAND_HPP__