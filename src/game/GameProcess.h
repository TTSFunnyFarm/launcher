#ifndef GAMEPROCESS_H
#define GAMEPROCESS_H

#include <QObject>

class GameProcess : public QObject
{
    Q_OBJECT

public:
    GameProcess();
    ~GameProcess();

    void launch_game();

signals:
    void process_ended();
};

#endif // GAMEPROCESS_H
