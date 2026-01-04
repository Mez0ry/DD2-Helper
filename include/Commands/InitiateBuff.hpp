#ifndef __INITIATE_BUFF_COMMAND_HPP__
#define __INITIATE_BUFF_COMMAND_HPP__
#include "ICommand.hpp"
class PlayerList;

class InitiateBuff : public ICommand
{
    PlayerList& m_PlayerList;
public:
    InitiateBuff(PlayerList& list);
    ~InitiateBuff() = default;

    void Execute() override;
};

#endif //!__INITIATE_BUFF_COMMAND_HPP__