#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__
#include <vector>
#include <memory>
#include <atomic>
#include <future>

class Window;

class Player{
private:
    std::shared_ptr<Window> m_Window;
    std::atomic<bool> m_IsBuffing{false};
    std::vector<std::future<void>> m_Futures;

public:
    Player(std::shared_ptr<Window> window);
    ~Player() = default;

    [[nodiscard]] std::shared_ptr<Window> GetWindow() const {return m_Window;}
public:
    bool IsBuffing() const {return m_IsBuffing.load();}
    void StartInitiateBuffing();
    void StopInitiateBuffing();
private:
};

class PlayerList{
public:
    void AddPlayer(std::shared_ptr<Player> player);
    void AddPlayer(std::vector<std::shared_ptr<Player>> players);
    void RemovePlayer(std::shared_ptr<Player> player);

    auto begin() const {return m_Players.begin();}
    auto end() const{return m_Players.end();}

    std::vector<std::shared_ptr<Player>>& GetPlayers() {return m_Players;}
private:
    std::vector<std::shared_ptr<Player>> m_Players;
};

#endif //!__PLAYER_HPP__