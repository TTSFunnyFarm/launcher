#include "LauncherWindow.h"
#ifdef Q_OS_MAC
#include "ui_FFLauncher-macx.h"
#endif // Q_OS_MAC

#ifdef Q_OS_WIN
#include "ui_FFLauncher-win32.h"
#endif // Q_OS_WIN

#include "src/core/Constants.h"
#include "src/core/Localizer.h"

#include <QDesktopServices>

LauncherWindow::LauncherWindow(QWidget *parent) :
    FramelessWindow(parent),
    ui(new Ui::FFLauncher),
    updater(new Updater(URL_DOWNLOAD_MIRROR))
{
    ui->setupUi(this);

    // Update frame not visible by default:
    ui->frame_update->setVisible(false);

#ifdef Q_OS_WIN
    setTitleBar(ui->launcher_window);
    addIgnoreWidget(ui->frame_toon_news);
#endif

#ifdef Q_OS_MAC
    this->setCloseBtnHidden(true);
    this->setMinBtnHidden(true);
    this->setZoomBtnHidden(true);
#endif
}

LauncherWindow::~LauncherWindow()
{
    delete updater;
    delete ui;
}

void LauncherWindow::on_push_button_close_clicked()
{
    this->close();
}

void LauncherWindow::on_push_button_minimize_clicked()
{
    this->showMinimized();
}

void LauncherWindow::on_push_button_play_clicked()
{
    // Disable the play & settings buttons:
    ui->push_button_play->setEnabled(false);
    ui->push_button_settings->setEnabled(false);

    // Hide play button, show update frame:
    ui->push_button_play->setVisible(false);
    ui->frame_update->setVisible(true);
}

void LauncherWindow::on_push_button_website_clicked()
{
    QDesktopServices::openUrl(QUrl(URL_WEBSITE));
}

void LauncherWindow::on_push_button_settings_clicked()
{
    // TODO
}

void LauncherWindow::on_push_button_discord_clicked()
{
    QDesktopServices::openUrl(QUrl(URL_DISCORD));
}
