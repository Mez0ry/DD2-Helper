#ifndef __DROP_GREEN_MANA_COMMAND_HPP__
#define __DROP_GREEN_MANA_COMMAND_HPP__
#include "ICommand.hpp"
#include "KeySequence.hpp"

class PlayerList;

class DropGreenMana : public ICommand
{
private:
    KeySequence m_KeySequence;
    PlayerList &m_PlayerList;
    bool m_OnlyAlts;
public:
    DropGreenMana(PlayerList& list, bool only_alts = true);
    ~DropGreenMana() = default;

    void Execute() override;
private:
};
#endif //!__DROP_GREEN_MANA_COMMAND_HPP__