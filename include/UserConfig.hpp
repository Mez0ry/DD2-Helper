#ifndef __USER_CONFIG_HPP__
#define __USER_CONFIG_HPP__

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QObject>

#include <mutex>
#include <qversionnumber.h>

#include "HotKeySequence.hpp"

class UserConfig final : public QObject
{
    Q_OBJECT
public:
    struct Private{};
private:
    static std::once_flag m_InitFlag;
    static std::shared_ptr<UserConfig> m_Instance;
private:
    QString m_ConfigPath;
    mutable std::unordered_map<QString, HotKeySequence> m_KeyBindsStr;
    const QString m_UserConfigPath{QString(QCoreApplication::applicationDirPath() + "/resources/user.json")};
    mutable QVersionNumber m_CurrentVersion;
    mutable QString m_LatestReleaseUrl;
signals:
    bool IsParsed() const;
public:
    ~UserConfig();

    static std::shared_ptr<UserConfig> GetInstance() noexcept;
    HotKeySequence GetKeyBind(const QString& hotkey_name) const;
    void ChangeKeyBind(const QString& hotkey_name, HotKeySequence hotkey_sequence);

    void SaveConfig() const;
    void LoadConfig(const char delimiter = '+') const;
public:
    QVersionNumber GetVersion() const;

    [[nodiscard]] QVersionNumber GetLatestReleaseVersion() const;
private:
    QVersionNumber  NormalizeVersion(QString) const;
    explicit UserConfig(Private) {}
};
#endif // __USER_CONFIG_HPP__
