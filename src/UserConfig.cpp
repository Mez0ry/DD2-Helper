#include "UserConfig.hpp"
#include "KeySequence.hpp"

#include "rapidjson/document.h"
#include "MouseInput.hpp"
#include "KeyboardInput.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

std::mutex User::m_Mutex;
std::once_flag User::m_InitFlag;
std::shared_ptr<User> User::m_Instance;
std::unordered_map<std::string, KeySequence> User::m_KeyBindsStr = {};

User::User(Private)
{
    ParseConfig("./resources/user/user.json");
}

std::shared_ptr<User> User::GetInstance() noexcept
{
    std::lock_guard<std::mutex> locker(m_Mutex);

    std::call_once(m_InitFlag, [&](){ 
        m_Instance = std::shared_ptr<User>(new User(Private()), [&](auto l)
        {
            delete l; 
        }); 
    });

    return m_Instance;
}

KeySequence User::GetKeyBind(const std::string &key_name) const
{
    std::shared_lock lock(m_KeybindsMutex);
    
    if(m_KeyBindsStr.empty()){
        return KeySequence();
    }

    auto it = m_KeyBindsStr.find(key_name);

    if (it == m_KeyBindsStr.end())
    {
        throw std::invalid_argument("Specified key name doesnt exists");
    }

    return it->second;
}

void User::ParseConfig(const std::string_view path) const
{
    std::unique_lock lock(m_KeybindsMutex);

    m_KeyBindsStr.clear();

    std::ifstream ifs(path.data());
    if (!ifs.is_open())
    {
        std::cerr << "Failed to open a user config (user.json) file to read" << path << std::endl;
    }

    std::stringstream buffer;
    buffer << ifs.rdbuf();
    std::string json_content = buffer.str();

    rapidjson::Document document;
    if (document.Parse(json_content.c_str()).HasParseError())
    {

        std::cout << "Failed to parse json config, using defaults" << '\n';
        const char *json_default_settings = R"({
            "User": {
                "KeyBinds": {
                    "RetryButton": "Y",
                    "ReadyButton": "G"
                }
            }
        })";

        document.Parse(json_default_settings);
    }

    if (document.HasMember("User") && document["User"].IsObject())
    {
        const auto &user_obj = document["User"];

        if (user_obj.HasMember("KeyBinds") && user_obj["KeyBinds"].IsObject())
        {
            const auto &key_binds_obj = user_obj["KeyBinds"];

            for (auto it = key_binds_obj.MemberBegin(); it != key_binds_obj.MemberEnd(); ++it)
            {
                if (it->value.IsString())
                {
                    auto button_name = std::string(it->name.GetString());
                    if (MouseInput::HasButton(it->value.GetString()))
                    {
                        m_KeyBindsStr.insert(std::make_pair(button_name, MouseInput::VButtonKeyFromStr(it->value.GetString())));
                    }
                    else if (KeyboardInput::HasKey(it->value.GetString()))
                    {
                        m_KeyBindsStr.insert(std::make_pair(button_name, KeyboardInput::VKeyFromStr(it->value.GetString())));
                    }
                }else if(it->value.IsArray()){
                    std::string sequence;
                    sequence.reserve(10);

                    auto button_name = std::string(it->name.GetString());
                    const auto& value_array = it->value.GetArray();

                    for (auto& v : value_array) {
                        if (v.IsString()) {
                            sequence += std::string(v.GetString()) + ' ';
                        }
                    }

                    m_KeyBindsStr.insert(std::make_pair(button_name, KeySequence(sequence)));
                }
            }
        }
    }
}
