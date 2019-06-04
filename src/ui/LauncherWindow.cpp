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
