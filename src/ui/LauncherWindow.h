#ifndef LAUNCHERWINDOW_H
#define LAUNCHERWINDOW_H

#include "FramelessWindow.h"
#include "src/updater/Updater.h"

#include <QLabel>

namespace Ui {
class FFLauncher;
}

class LauncherWindow : public FramelessWindow
{
    Q_OBJECT

public:
    explicit LauncherWindow(QWidget *parent = nullptr);
    ~LauncherWindow();

    void update_manifest();

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

    void setup_fonts();
    void scale_fonts();
    void scale_font(QLabel *label);

    float m_scale;
};

#endif // LAUNCHERWINDOW_H
