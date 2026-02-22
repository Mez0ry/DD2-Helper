#ifndef __THEME_TOGGLE_H
#define __THEME_TOGGLE_H
#include <QWidget>
#include <QPushButton>

class ThemeToggle : public QWidget{
    Q_OBJECT
private:
    std::shared_ptr<QPushButton> m_Light, m_Dark;
public:
    ThemeToggle(QWidget* parent = nullptr);
signals:
    void ThemeChanged(const QString& new_stylesheet);
private:
    void ApplyLightMode();
    void ApplyDarkMode();
    void LoadSavedTheme();
    inline QString GetStyle(const QString& style_path);
};

#endif // __THEME_TOGGLE_H
