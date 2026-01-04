#ifndef __READY_COMMAND_HPP__
#define __READY_COMMAND_HPP__
#include "ICommand.hpp"
#include <memory>
#include <atomic>
#include "SendInputCommand.hpp"

class AutoReady : public SendPlayersInput
{
private:
    KeySequence m_KeySequence;
    PlayerList &m_PlayerList;
    std::atomic<bool> m_AutoReady{false};
public:
    AutoReady(KeySequence sequence, PlayerList &list, uint16_t hold_time_ms = 50, uint16_t button_released_time_ms = 50);
    ~AutoReady() = default;

    void Execute() override;
private:
    bool IsAutoReady() const{
        return m_AutoReady.load();
    }

    void ToogleAutoReady(){
        m_AutoReady.store(!IsAutoReady());
    }
};

#endif //!__READY_COMMAND_HPP__