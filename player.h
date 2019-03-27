#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

class Player
{
public:
    Player();
    ~Player();
    bool PlayMedia(QString path);
    bool SwitchRecord(bool start_stop);
};

#endif // PLAYER_H
