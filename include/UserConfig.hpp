#ifndef __USER_CONFIG_HPP__
#define __USER_CONFIG_HPP__
#include <memory>
#include <mutex>
#include <unordered_map>
#include "VKeys.hpp"

class User
{
private:
    static std::mutex m_Mutex;
    static std::once_flag m_InitFlag;
    static std::shared_ptr<User> m_Instance;

    struct Private {
        explicit Private() = default;
    };
    
    static std::unordered_map<std::string, VKeys> m_KeyBindsStr;
    
public:
    User(Private);
    [[nodiscard]] static std::shared_ptr<User> GetInstance() noexcept;

    VKeys GetKeyBind(const std::string& key_name) const;

private:
    User() = default;
    ~User() = default;
    User(User const&) = delete;
    void operator=(User const&) = delete;
};
#endif //!__USER_CONFIG_HPP__