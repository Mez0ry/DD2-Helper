#include "UserConfig.hpp"
#include "rapidjson/document.h"
#include "MouseInput.hpp"
#include "KeyboardInput.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

std::mutex User::m_Mutex;
std::once_flag User::m_InitFlag;
std::shared_ptr<User> User::m_Instance;
std::unordered_map<std::string, VKeys> User::m_KeyBindsStr = {};

std::shared_ptr<User> User::GetInstance() noexcept {
    std::lock_guard<std::mutex> locker(m_Mutex);

    std::call_once(m_InitFlag, [&]() { 
        m_Instance = std::shared_ptr<User>(new User(Private()), [&](auto l){delete l;});
    });

    return m_Instance;
}

VKeys User::GetKeyBind(const std::string &key_name) const { 
    auto it = m_KeyBindsStr.find(key_name);

    if (it == m_KeyBindsStr.end()) {
        throw std::invalid_argument("Specified key name doesnt exists");
    }

    return it->second;
}

User::User(Private) {
    const std::string file_name = "./resources/user/user.json";

    std::ifstream ifs(file_name);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open a user config (user.json) file to read" << file_name << std::endl;
    }

    std::stringstream buffer;
    buffer << ifs.rdbuf();
    std::string json_content = buffer.str();

    rapidjson::Document document;
    if (document.Parse(json_content.c_str()).HasParseError()) {

        std::cout << "Failed to parse json config, using defaults" << '\n';
        const char* json_default_settings = R"({
            "User": {
                "KeyBinds": {
                    "RetryButton": "Y",
                    "ReadyButton": "G"
                }
            }
        })";
        
        document.Parse(json_default_settings);
    }

    if (document.HasMember("User") && document["User"].IsObject()) {
        const auto& user_obj = document["User"];

        if (user_obj.HasMember("KeyBinds") && user_obj["KeyBinds"].IsObject()) {
            const auto& key_binds_obj = user_obj["KeyBinds"];

            for (auto it = key_binds_obj.MemberBegin(); it != key_binds_obj.MemberEnd(); ++it) {
                if (it->value.IsString()) {
                    auto button_name = std::string(it->name.GetString());
                    if(MouseInput::HasButtonKey(it->value.GetString())){
                        m_KeyBindsStr.insert(std::make_pair(button_name, MouseInput::VButtonKeyFromStr(it->value.GetString())));
                    }else if(KeyboardInput::HasButtonKey(it->value.GetString())){
                        m_KeyBindsStr.insert(std::make_pair(button_name, KeyboardInput::VKeyFromStr(it->value.GetString())));
                    }
                }
            }
        }
    }
}
