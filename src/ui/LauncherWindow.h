#ifndef LAUNCHERWINDOW_H
#define LAUNCHERWINDOW_H

#include "FramelessWindow.h"
#include "src/updater/Updater.h"
#include "src/game/GameProcess.h"

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

    bool update_game();
    void launch_game();

private slots:
    void on_push_button_close_clicked();
    void on_push_button_minimize_clicked();
    void on_push_button_play_clicked();
    void on_push_button_website_clicked();
    void on_push_button_settings_clicked();
    void on_push_button_discord_clicked();

    void download_error(int error_code, const QString &error_string);
    void download_progress(qint64 bytes_read, qint64 bytes_total, const QString &status);

    void handle_manifest();
    void handle_update();
    void handle_game_exit();

private:
    Ui::FFLauncher *ui;
    Updater *updater;
    GameProcess *game_process;

    void setup_fonts();
    void scale_fonts();
    void scale_font(QLabel *label);
    void goto_main_ui();
    void set_status_text(const QString &status, int pointSize = 0);

    float m_scale;
    QMetaObject::Connection m_update_connection;
};

#endif // LAUNCHERWINDOW_H
