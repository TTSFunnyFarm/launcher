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
#include <QMessageBox>
#include <QScreen>

LauncherWindow::LauncherWindow(QWidget *parent) :
    FramelessWindow(parent),
    ui(new Ui::FFLauncher),
    updater(new Updater(URL_DOWNLOAD_MIRROR))
{
    ui->setupUi(this);

    // Get up to date with the manifest:
    update_manifest();

    // Set fonts to Impress BT:
    setup_fonts();

    // Scale text font sizes, based on user's screen DPI:
    scale_fonts();

    // Update frame not visible by default:
    ui->frame_update->setVisible(false);

#ifdef Q_OS_WIN
    setupUi();
    setTitleBar(ui->launcher_window);
    addIgnoreWidget(ui->frame_toon_news);
#endif

#ifdef Q_OS_MAC
    setCloseBtnHidden(true);
    setMinBtnHidden(true);
    setZoomBtnHidden(true);
#endif
}

LauncherWindow::~LauncherWindow()
{
    delete updater;
    delete ui;
}

void LauncherWindow::update_manifest()
{
    updater->update_manifest();

    // Update the version labels:
    QString game_version = updater->get_game_version();
    QString launcher_version = updater->get_launcher_version();
    if (game_version.isEmpty())
    {
        ui->label_game_version->setText(GUI_GAME_VERSION.arg(GUI_NO_VERSION));
    }
    else
    {
        ui->label_game_version->setText(GUI_GAME_VERSION.arg("v" + game_version));
    }

    if (launcher_version.isEmpty())
    {
        ui->label_launcher_version->setText(GUI_LAUNCHER_VERSION.arg(GUI_NO_VERSION));
    }
    else
    {
        ui->label_launcher_version->setText(GUI_LAUNCHER_VERSION.arg("v" + launcher_version));
    }

    // Ensure our launcher is at the latest version:
    if (!launcher_version.isEmpty() && (launcher_version != VERSION))
    {
        QMessageBox message_box;
        message_box.setWindowTitle(ERROR_OUT_OF_DATE_TITLE);
        message_box.setText(ERROR_OUT_OF_DATE_TEXT);
        message_box.setIcon(QMessageBox::Warning);
        QAbstractButton* pButtonUpdate = message_box.addButton(tr("OK"), QMessageBox::YesRole);
        message_box.exec();

        if (message_box.clickedButton() == pButtonUpdate)
        {
            QDesktopServices::openUrl(QUrl(URL_PLAY));
        }

        exit(1);
    }
}

void LauncherWindow::setup_fonts()
{
    QFont impress("Impress BT");

    impress.setPointSize(10);
    ui->label_game_version->setFont(impress);
    ui->label_launcher_version->setFont(impress);
}

void LauncherWindow::scale_fonts()
{
    // Set scale to be used for font sizes, based on user's screen DPI:
    QScreen *scrn = QApplication::screens().at(0);
    double dpi = scrn->logicalDotsPerInch();
    m_scale = (dpi > 0.0) ? (96.0f / static_cast<float>(dpi)) : 1.0;

    // Apply scale to all affected text labels:
    scale_font(ui->label_launcher_version);
    scale_font(ui->label_launcher_version);
}

void LauncherWindow::scale_font(QLabel *label)
{
    QFont label_font;

    // Scale font for specified text label:
    label_font = label->font();
    label_font.setPointSize(static_cast<int>(label_font.pointSize() * m_scale));
    label->setFont(label_font);
}

void LauncherWindow::on_push_button_close_clicked()
{
    close();
}

void LauncherWindow::on_push_button_minimize_clicked()
{
    showMinimized();
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
