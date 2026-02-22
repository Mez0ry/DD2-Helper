#ifndef __USER_CONFIG_HPP__
#define __USER_CONFIG_HPP__

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMutex>
#include <QString>
#include <memory>
#include <mutex>

#include "HotKeySequence.hpp"

#include <QObject>

class UserConfig final : public QObject
{
    Q_OBJECT
public:
    struct Private{};
private:
    static std::once_flag m_InitFlag;
    static std::shared_ptr<UserConfig> m_Instance;
    QString m_ConfigPath;
    mutable std::unordered_map<QString, HotKeySequence> m_KeyBindsStr;
    const QString m_UserConfigPath{QString(QCoreApplication::applicationDirPath() + "/resources/user.json")};

    explicit UserConfig(Private) {}
signals:
    bool IsParsed() const;
public:
    ~UserConfig();

    static std::shared_ptr<UserConfig> GetInstance() noexcept;
    HotKeySequence GetKeyBind(const QString& hotkey_name) const;
    void ChangeKeyBind(const QString& hotkey_name, HotKeySequence hotkey_sequence);

    void SaveConfig() const;
    void LoadConfig(const char delimiter = '+') const;
};
#endif // __USER_CONFIG_HPP__
