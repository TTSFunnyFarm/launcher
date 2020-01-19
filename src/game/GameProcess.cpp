#include "GameProcess.h"

#include <QProcess>

GameProcess::GameProcess() : QObject()
{
}

GameProcess::~GameProcess()
{
}

void GameProcess::launch_game()
{
#if defined(Q_OS_WIN)
    // Start the game:
    QProcess process;
    process.start("funnyfarm.exe");

    // Wait for the process to finish:
    process.waitForFinished(-1);

    emit this->process_ended();
#elif defined(Q_OS_MAC)
    // Start the game:
    QProcess process;
    process.start("./funnyfarm");

    // Wait for the process to finish:
    process.waitForFinished(-1);

    emit this->process_ended();
#endif
}
