#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include "ui_mainwindow.h"
class MainWindow;

namespace Utils {
    inline MainWindow *GetMainWindow();
    Ui::MainWindow* GetUserInterface();
} // namespace Utils

#endif // __UTILS_HPP__
