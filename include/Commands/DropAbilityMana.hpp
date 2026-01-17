#ifndef __DROP_ABILITY_MANA_COMMAND_HPP__
#define __DROP_ABILITY_MANA_COMMAND_HPP__
#include "ICommand.hpp"
#include "KeySequence.hpp"

class PlayerList;
class KeySequence;

class DropAbilityMana final : public ICommand{
private:
    KeySequence m_KeySequence;
    PlayerList &m_PlayerList;
    bool m_OnlyAlts;
public:
    DropAbilityMana() = delete;
    DropAbilityMana(PlayerList& player_list, bool only_alts = true);
    DropAbilityMana(PlayerList& player_list, const KeySequence& sequence, bool only_alts = true);

    ~DropAbilityMana() = default;
    void Execute() override;
private:
};

#endif //!__DROP_ABILITY_MANA_COMMAND_HPP__