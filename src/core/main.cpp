#include "src/ui/LauncherWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication a(argc, argv);

    LauncherWindow w;
    w.show();

    return a.exec();
}
