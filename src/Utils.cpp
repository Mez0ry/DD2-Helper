#include "Utils.hpp"
#include "mainwindow.h"
#include <QApplication>

inline MainWindow * Utils::GetMainWindow(){
    foreach (QWidget *w, qApp->topLevelWidgets())
    if (QMainWindow* main_win = qobject_cast<QMainWindow*>(w))
        return static_cast<MainWindow*>(main_win);
    return nullptr;
}


Ui::MainWindow* Utils::GetUserInterface(){
    return GetMainWindow()->GetUi();
}
