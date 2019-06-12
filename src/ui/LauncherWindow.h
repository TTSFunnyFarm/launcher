#ifndef LAUNCHERWINDOW_H
#define LAUNCHERWINDOW_H

#include "FramelessWindow.h"
#include "src/updater/Updater.h"

namespace Ui {
class FFLauncher;
}

class LauncherWindow : public FramelessWindow
{
    Q_OBJECT

public:
    explicit LauncherWindow(QWidget *parent = nullptr);
    ~LauncherWindow();

private slots:
    void on_push_button_close_clicked();
    void on_push_button_minimize_clicked();
    void on_push_button_play_clicked();
    void on_push_button_website_clicked();
    void on_push_button_settings_clicked();
    void on_push_button_discord_clicked();

private:
    Ui::FFLauncher *ui;
    Updater *updater;
};

#endif // LAUNCHERWINDOW_H
