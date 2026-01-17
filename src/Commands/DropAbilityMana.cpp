#include "DropAbilityMana.hpp"
#include "UserConfig.hpp"
#include "Player.hpp"
#include <algorithm>

DropAbilityMana::DropAbilityMana(PlayerList &player_list, bool only_alts) : m_KeySequence(User::GetInstance()->GetKeyBind("DropAbilityMana")), m_PlayerList(player_list), m_OnlyAlts(only_alts)
{
}

DropAbilityMana::DropAbilityMana(PlayerList &player_list, const KeySequence &sequence, bool only_alts) : m_KeySequence(sequence), m_PlayerList(player_list), m_OnlyAlts(only_alts)
{

}

void DropAbilityMana::Execute()
{
    if(m_KeySequence.IsKeysPressed()){
        auto start_iter = m_PlayerList.begin();
        auto end_iter = m_PlayerList.end();
        
        if(m_OnlyAlts){
            uint8_t skip_n_elems = 1; // skipping host player (always first in vector of players list)
            start_iter = std::next(m_PlayerList.begin(), skip_n_elems);
            end_iter = m_PlayerList.end();
        }
 
        std::for_each(start_iter, end_iter, [](std::shared_ptr<Player> player){
           player->DropMana();
        });
        
    }
}