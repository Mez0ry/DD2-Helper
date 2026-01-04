#include "Player.hpp"
#include "Window.hpp"
#include "KeySequence.hpp"

#include <thread>
#include <algorithm>

Player::Player(std::shared_ptr<Window> window) : m_Window(window)
{

}

void Player::StartInitiateBuffing()
{
    if(m_IsBuffing.load()){
        return;
    }

    this->m_Window->SetWindowTitle(this->m_Window->GetTitle() + L" [Initiate buffing]");

    m_IsBuffing.store(true);

    m_Futures.push_back(std::async(std::launch::async,[this](){
        auto win_descriptor = m_Window->GetWindowDescriptor();
        KeySequence press_seq("K3 LBUTTON");

        while(m_IsBuffing.load()){
            /**
             * @NOTE better way of sending input or smth that guarantees perfect execution of key sequence in-game to prevent send of multiple LButtons messages
             */
            press_seq.PressKeys(win_descriptor, 11, 1500);

            SendMessage(win_descriptor, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
            SendMessage(win_descriptor, WM_LBUTTONUP, 0, MAKELPARAM(0, 0));
            
            press_seq.PressKeys(win_descriptor, 11, 1100);

            SendMessage(win_descriptor, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
            SendMessage(win_descriptor, WM_LBUTTONUP, 0, MAKELPARAM(0, 0));
            
            std::this_thread::sleep_for(std::chrono::milliseconds(9000));
        }
    }));
}

void Player::StopInitiateBuffing() {
    this->m_Window->SetWindowTitle(m_Window->GetPrevTitle());

    m_IsBuffing.store(false);
    for (auto& future : m_Futures) {
        if(future.valid()){
            future.get();
        }
    }

    m_Futures.clear();
}

void PlayerList::AddPlayer(std::shared_ptr<Player> player) {
    m_Players.push_back(player);
}

void PlayerList::AddPlayer(std::vector<std::shared_ptr<Player>> players) {
    m_Players.insert(m_Players.end(), players.begin(), players.end());
}

void PlayerList::RemovePlayer(std::shared_ptr<Player> player) {
    auto it = std::find(m_Players.begin(), m_Players.end(), player);
    if (it != m_Players.end()) {
        m_Players.erase(it);
    }
}
