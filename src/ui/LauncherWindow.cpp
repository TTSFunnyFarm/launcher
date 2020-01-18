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
#include <QStyle>
#include <QGraphicsDropShadowEffect>

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
    ui->progress_bar_update->setVisible(false);
    ui->label_status->setVisible(false);

    // TEMP: Disable settings button:
    ui->push_button_settings->setEnabled(false);

#ifdef Q_OS_WIN
    setupUi();
    setTitleBar(ui->launcher_window);
    addIgnoreWidget(ui->frame_toon_news);
#endif

#ifdef Q_OS_MAC
    setCloseBtnHidden(true);
    setMinBtnHidden(true);
    setZoomBtnHidden(true);
    setResizable(false);

    // Center on the screen:
    QScreen *scrn = QApplication::screens().at(0);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), scrn->geometry()));
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
    // Load Impress BT and antialias it if possible:
    QFont impress("Impress BT");
    impress.setStyleStrategy(QFont::PreferAntialias);

    // Set the launcher & game version label fonts to Impress BT (10pt):
    impress.setPointSize(10);
    ui->label_game_version->setFont(impress);
    ui->label_launcher_version->setFont(impress);

    // Set the status label font to Impress BT (22pt):
    impress.setPointSize(22);
    ui->label_status->setFont(impress);

    // Add a nice drop shadow to the status label:
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(0);
    effect->setColor(QColor("#451616"));
    effect->setOffset(-2.57, 1.5);
    ui->label_status->setGraphicsEffect(effect);
}

void LauncherWindow::scale_fonts()
{
    // Set scale to be used for font sizes, based on user's screen DPI:
    QScreen *scrn = QApplication::screens().at(0);
    double dpi = scrn->logicalDotsPerInch();
    m_scale = (dpi > 0.0) ? (96.0f / static_cast<float>(dpi)) : 1.0;

    // Apply scale to all affected text labels:
    scale_font(ui->label_game_version);
    scale_font(ui->label_launcher_version);
    scale_font(ui->label_status);
}

void LauncherWindow::scale_font(QLabel *label)
{
    QFont label_font;

    // Scale font for specified text label:
    label_font = label->font();
    label_font.setPointSize(static_cast<int>(label_font.pointSize() * m_scale));
    label->setFont(label_font);
}

void LauncherWindow::goto_main_ui()
{
    // Enable the play & settings buttons:
    ui->push_button_play->setEnabled(true);
    //ui->push_button_settings->setEnabled(true);  // TEMP

    // Show play button, hide update frame:
    ui->push_button_play->setVisible(true);
    ui->frame_update->setVisible(false);
    ui->progress_bar_update->setVisible(false);
    ui->label_status->setVisible(false);

    // Reset progress bar values:
    ui->progress_bar_update->setMaximum(100);
    ui->progress_bar_update->setValue(-1);
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
    //ui->push_button_settings->setEnabled(false);  // TEMP

    // Hide play button, show update frame:
    ui->push_button_play->setVisible(false);
    ui->frame_update->setVisible(true);
    ui->progress_bar_update->setVisible(true);
    ui->label_status->setVisible(true);

    // Notify the user that we are checking for updates:
    set_status_text(GUI_CHECKING_FOR_UPDATES, 22);

    // Get up to date with the manifest:
    update_manifest();

    // Attempt to update the game:
    if (!update_game())
    {
        // The update failed, so go back to the main UI state:

        // Go to the main UI:
        goto_main_ui();

        // We're done!
        return;
    }

    // Alright, we're done! Start the game:
    launch_game();
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

void LauncherWindow::download_error(int error_code, const QString &error_string)
{
    set_status_text(QString::number(error_code) + ": " + error_string);
}

void LauncherWindow::download_progress(qint64 bytes_read, qint64 bytes_total, const QString &status)
{
    set_status_text(status);
    ui->progress_bar_update->setMaximum(static_cast<int>(bytes_total));
    ui->progress_bar_update->setValue(static_cast<int>(bytes_read));
}

bool LauncherWindow::update_game()
{
    // Connect the required slots:
    QObject::connect(updater, SIGNAL(download_error(int, const QString &)),
                     this, SLOT(download_error(int, const QString &)));
    QObject::connect(updater, SIGNAL(download_progress(qint64, qint64, const QString &)),
                     this, SLOT(download_progress(qint64, qint64, const QString &)));

    // Begin downloading the updated files:
    return updater->update();
}

void LauncherWindow::launch_game()
{
#if defined(Q_OS_WIN)
    // Set our working directory to the game location:
    QDir::setCurrent(updater->get_directory());

    // Start the game:
    QProcess process;
    process.start("funnyfarm.exe");

    // Hide launcher and wait for application to finish:
    hide();
    process.waitForFinished(-1);

    // Go to the main UI:
    goto_main_ui();

    // Show the launcher:
    show();
#elif defined(Q_OS_MAC)
    // Set our working directory to the game location:
    QDir::setCurrent(updater->get_directory());

    // Start the game:
    QProcess process;
    process.start("funnyfarm.exe");

    // Hide launcher and wait for application to finish:
    hide();
    process.waitForFinished(-1);

    // Go to the main UI:
    goto_main_ui();

    // Show the launcher:
    show();
#endif
}

void LauncherWindow::set_status_text(const QString &status, int pointSize)
{
    QFont font = ui->label_status->font();
    font.setPointSize(22);

    if (pointSize > 0)
    {
        if (font.pointSize() != pointSize)
        {
            font.setPointSize(pointSize);
        }
    }
    else
    {
        bool textFits = false;
        while (!textFits)
        {
            QFontMetrics fontMetrics(font);
            QRect boundingRect = fontMetrics.boundingRect(0, 0, ui->label_status->width(), ui->label_status->height(),
                                                          Qt::TextWordWrap | Qt::AlignCenter, status);

            if (boundingRect.width() <= ui->label_status->width() && boundingRect.height() <= ui->label_status->height())
            {
                textFits = true;
            }
            else
            {
                font.setPointSize(font.pointSize() - 1);
            }
        }
    }

    ui->label_status->setFont(font);
    ui->label_status->setText(status);
    scale_font(ui->label_status);
}
