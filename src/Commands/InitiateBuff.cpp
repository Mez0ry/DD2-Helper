#include "InitiateBuff.hpp"
#include "Player.hpp"
#include "Window.hpp"

InitiateBuff::InitiateBuff(PlayerList &list) : m_PlayerList(list)
{

}

void InitiateBuff::Execute() {
    for(auto& player : m_PlayerList){
        auto player_window = player->GetWindow();

        if(player_window->IsForegrounded() && !player->IsBuffing()){
            player->StartInitiateBuffing();
        }else if(player->IsBuffing() && player_window->IsForegrounded()){
            player->StopInitiateBuffing();
        }
    }
}
