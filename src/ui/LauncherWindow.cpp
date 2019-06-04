#include "LauncherWindow.h"
#ifdef Q_OS_MAC
#include "ui_FFLauncher-macx.h"
#endif // Q_OS_MAC

#ifdef Q_OS_WIN
#include "ui_FFLauncher-win32.h"
#endif // Q_OS_WIN

#include "src/core/Constants.h"
#include "src/core/Localizer.h"

LauncherWindow::LauncherWindow(QWidget *parent) :
    FramelessWindow(parent),
    ui(new Ui::FFLauncher)
{
    ui->setupUi(this);

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
    // TODO
}
