#include "src/ui/LauncherWindow.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication a(argc, argv);
    a.setApplicationName("Toontown's Funny Farm");

    QFontDatabase::addApplicationFont(":/ttf/assets/Impress.ttf");

    LauncherWindow w;
    w.show();

    return a.exec();
}
