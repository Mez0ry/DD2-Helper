#include "DropBuildResource.hpp"
#include "Player.hpp"
#include "Window.hpp"
#include "UserConfig.hpp"
#include <algorithm>

DropBuildResource::DropBuildResource(PlayerList &list, bool only_alts) : m_KeySequence(User::GetInstance()->GetKeyBind("DropBuildResourceButton")), m_PlayerList(list), m_OnlyAlts(only_alts)
{

}

void DropBuildResource::Execute() {
    if(m_KeySequence.IsKeysPressed()){
        auto start_iter = m_PlayerList.begin();
        auto end_iter = m_PlayerList.end();
        
        if(m_OnlyAlts){
            uint8_t skip_n_elems = 1; // skipping host player (always first in vector of players list)
            start_iter = std::next(m_PlayerList.begin(), skip_n_elems);
            end_iter = m_PlayerList.end();
        }
 
        std::for_each(start_iter, end_iter, [](std::shared_ptr<Player> player){
           player->DropBuildResource();
        });
        
    }
}
