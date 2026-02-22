#include "ThemeToggle.hpp"
#include <QApplication>
#include <QFile>
#include <QIcon>
#include <QSettings>
#include <qboxlayout.h>

ThemeToggle::ThemeToggle(QWidget *parent) : QWidget(parent){
    m_Light = std::make_shared<QPushButton>(QIcon(":/resources/icons/generic/light_mode.svg"), "", nullptr);
    m_Dark = std::make_shared<QPushButton>(QIcon(":/resources/icons/generic/dark_mode.svg"), "", nullptr);

    connect(m_Light.get(), &QPushButton::pressed, this, [&](){
        ApplyLightMode();
        emit ThemeChanged(qApp->styleSheet());
    });

    connect(m_Dark.get(), &QPushButton::pressed, this, [&](){
        ApplyDarkMode();
        emit ThemeChanged(qApp->styleSheet());
    });

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(m_Light.get());
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    LoadSavedTheme();
}

void ThemeToggle::ApplyLightMode() {
    QSettings settings("Mezory", "ColorPicker");
    settings.beginGroup("styles");
    auto light_theme_style = settings.value("light_theme");
    settings.endGroup();

    settings.setValue("theme_mode", "light");
    settings.sync();

    qApp->setStyleSheet(GetStyle(light_theme_style.toString()));

    m_Dark->show();
    m_Light->hide();

    this->layout()->removeWidget(m_Light.get());
    this->layout()->addWidget(m_Dark.get());
}

void ThemeToggle::ApplyDarkMode() {
    QSettings settings("Mezory", "ColorPicker");
    settings.beginGroup("styles");
    auto dark_theme_style = settings.value("dark_theme");
    settings.endGroup();

    settings.setValue("theme_mode", "dark");
    settings.sync();

    qApp->setStyleSheet(GetStyle(dark_theme_style.toString()));

    m_Light->show();
    m_Dark->hide();

    this->layout()->removeWidget(m_Dark.get());
    this->layout()->addWidget(m_Light.get());
}

void ThemeToggle::LoadSavedTheme() {
    QSettings settings("Mezory", "ColorPicker");
    QString theme_mode = settings.value("theme_mode", "dark").toString();

    if(theme_mode == "dark") {
        ApplyDarkMode();
    } else {
        ApplyLightMode();
    }
}

QString ThemeToggle::GetStyle(const QString &style_path){
    QFile style_file(style_path);

    if(!style_file.open(QFile::ReadOnly)){
        qCritical() << "failed to load style for application" << '\n';
    }

    return QLatin1String(style_file.readAll());
}
