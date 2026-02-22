#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__
#include <QTimer>
#include <atomic>
#include <memory>
#include <qfuture.h>
#include <vector>

class Window;
class ICommand;

class Player : public QObject{
    Q_OBJECT
private:
    std::shared_ptr<Window> m_Window;
    std::atomic<bool> m_IsBuffing{false}, m_IsAutoReady{false}, m_IsDropingMana;
    QList<QFuture<void>> m_Futures;
    std::vector<std::shared_ptr<ICommand>> m_Commands;
    QTimer m_InitiateBuffingTimer;
public:
    Player(std::shared_ptr<Window> window);
    ~Player();

    [[nodiscard]] std::shared_ptr<Window> GetWindow() const {return m_Window;}
public:

    template<typename TCommand, typename ... TArgs>
    void AddCommand(TArgs&&... args);
    void RemoveCommand(ICommand* command);

    [[nodiscard]] std::vector<std::shared_ptr<ICommand>>& GetCommands() {return m_Commands;}

    void StartInitiateBuffing();
    void StopInitiateBuffing();

    void SetBuffing(bool state);
    bool IsBuffing() const;

    void DropBuildResource(uint16_t hold_time_ms = (uint16_t)0, uint16_t button_released_time_ms = (uint16_t)0);
    void DropMana(uint16_t hold_time_ms = (uint16_t)0, uint16_t button_released_time_ms = (uint16_t)0);

    void StartAutoReady();
    void StopAutoReady();

    void SetAutoReady(bool state);
    bool IsAutoReady() const;
private:
    void PerformInitiateBuffStep();
};

template<typename TCommand, typename ... TArgs>
inline void Player::AddCommand(TArgs &&...args)
{
    m_Commands.emplace_back(std::make_shared<TCommand>(std::forward<TArgs>(args)...));
}

class PlayerList{
public:
    void AddPlayer(std::shared_ptr<Player> player);
    void AddPlayer(std::vector<std::shared_ptr<Player>> players);
    void RemovePlayer(std::shared_ptr<Player> player);

    void ExecuteCommands();

    auto begin() const {return m_Players.begin();}
    auto end() const{return m_Players.end();}
    void Clear() {m_Players.clear();}
    std::vector<std::shared_ptr<Player>>& GetPlayers() {return m_Players;}

    operator bool() const{
        return !m_Players.empty();
    }
private:
    std::vector<std::shared_ptr<Player>> m_Players;
};

#endif //!__PLAYER_HPP__
