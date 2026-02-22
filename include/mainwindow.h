#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileSystemWatcher>
#include <QMainWindow>
#include "Player.hpp"
#include "Window.hpp"
#include "qmainwindow.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE

class PlayerWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    std::shared_ptr<Window> m_HostWindow;
    std::vector<std::shared_ptr<Window>> m_AltWindows;
    WindowList m_WindowList;
    PlayerList m_PlayerList;
    QList<PlayerWidget*> m_PlayersWidgets;
private:
    QFileSystemWatcher m_FilesystemWatcher;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    [[nodiscard]] Ui::MainWindow* GetUi(){
        return m_UserInterface;
    }

public slots:
    void Update();
private:
    Ui::MainWindow *m_UserInterface;
};

#endif // MAINWINDOW_H
