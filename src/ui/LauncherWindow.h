#ifndef LAUNCHERWINDOW_H
#define LAUNCHERWINDOW_H

#include "FramelessWindow.h"

namespace Ui {
class FFLauncher;
}

class LauncherWindow : public FramelessWindow
{
    Q_OBJECT

public:
    explicit LauncherWindow(QWidget *parent = nullptr);
    ~LauncherWindow();

private:
    Ui::FFLauncher *ui;
};

#endif // LAUNCHERWINDOW_H
