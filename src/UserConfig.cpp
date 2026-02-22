#include "UserConfig.hpp"
#include "HotKeySequence.hpp"
#include <KeyboardInput.hpp>
#include <MouseInput.hpp>
#include <mutex>
#include <qmutex.h>
#include <QJsonArray>

std::once_flag UserConfig::m_InitFlag;
std::shared_ptr<UserConfig> UserConfig::m_Instance;

UserConfig::~UserConfig()
{
    SaveConfig();
}

std::shared_ptr<UserConfig> UserConfig::GetInstance() noexcept
{
    std::call_once(m_InitFlag, []()
    {
       m_Instance = std::shared_ptr<UserConfig>(new UserConfig(UserConfig::Private()));
    });

    return m_Instance;
}

HotKeySequence UserConfig::GetKeyBind(const QString& hotkey_name) const
{
    if (m_KeyBindsStr.empty())
    {
        return HotKeySequence();
    }

    auto it = m_KeyBindsStr.find(hotkey_name);

    if (it == m_KeyBindsStr.end())
    {
        throw std::invalid_argument("Specified key name doesn't exist.");
    }

    return it->second;
}

void UserConfig::ChangeKeyBind(const QString& hotkey_name, HotKeySequence hotkey_sequence) {
    auto it = m_KeyBindsStr.find(hotkey_name);
    if(it == m_KeyBindsStr.end()){
        qWarning() << "Could'nt find hotkey name, probably it doesnt exists";
        return;
    }
    it->second = hotkey_sequence;
}

void UserConfig::SaveConfig() const {
    QJsonObject root;
    QJsonObject keybinds;

    for (const auto& entry : m_KeyBindsStr)
    {
        const QString& keybind_name = entry.first;
        const HotKeySequence& sequence = entry.second;

        if (sequence.IsSingleKey())
        {
            keybinds.insert(keybind_name, QJsonValue(sequence.GetSequenceAsStr().c_str()));
        }
        else
        {
            QStringList keys = QString::fromStdString(sequence.GetSequenceAsStr()).split('+');
            QJsonArray arr;
            for (const auto& k : keys)
            {
                arr.append(k.trimmed());
            }
            keybinds.insert(keybind_name, arr);
        }
    }

    QJsonObject userconfig_obj;
    userconfig_obj["KeyBinds"] = keybinds;
    root["UserConfig"] = userconfig_obj;

    QJsonDocument document(root);
    QFile file(m_UserConfigPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qCritical() << "Unable to write to config file at: " << m_UserConfigPath;
        return;
    }

    file.write(document.toJson());
    file.close();
}

void UserConfig::LoadConfig(const char delimiter) const
{
    m_KeyBindsStr.clear();

    QFile file(m_UserConfigPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical() << "Failed to open the UserConfig configuration file.";
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isNull() || !doc.object().contains("UserConfig"))
    {
        qWarning() << "JSON parsing failed or missing 'UserConfig' object in config.";
        return;
    }

    const QJsonObject root_object = doc.object();
    const QJsonObject UserConfig_object = root_object.value("UserConfig").toObject();

    if (!UserConfig_object.contains("KeyBinds"))
    {
        qWarning() << "'KeyBinds' section is not found in config.";
        return;
    }

    const QJsonObject keybinds_object = UserConfig_object.value("KeyBinds").toObject();

    for (const auto& keybind_item : keybinds_object.keys())
    {
        const QString keybind_name = keybind_item;
        const QJsonValue value = keybinds_object[keybind_name];

        if (value.isString())
        {
            auto key_str = value.toString().toStdString();
            if (MouseInput::HasButton(key_str))
            {
                m_KeyBindsStr.insert({keybind_name, HotKeySequence(MouseInput::VButtonKeyFromStr(key_str), key_str)});
            }
            else if (KeyboardInput::HasKey(key_str))
            {
                m_KeyBindsStr.insert({keybind_name, HotKeySequence(KeyboardInput::VKeyFromStr(key_str), key_str)});
            }
        }
        else if (value.isArray())
        {
            QStringList keys;
            const QJsonArray array = value.toArray();
            for (const auto& item : array)
            {
                if (item.isString())
                {
                    keys.append(item.toString());
                }
            }

            if (!keys.isEmpty())
            {
                QString seq = keys.join(delimiter);
                m_KeyBindsStr.insert({keybind_name, HotKeySequence(seq.toStdString())});
            }
        }
    }
    emit IsParsed();
}
