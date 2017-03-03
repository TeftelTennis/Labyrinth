#include "labyrinth.h"

#ifndef GAMELOG_H
#define GAMELOG_H


class GameLog : public Labyrinth {\
public:
    //GameLog previousVersion;//Если ловушка переместила тебя.
    Player player;

    vector<Direction> turn;
    int iStart;
    int jStart;
    int iCur;
    int jCur;

    void addObject(LabyrinthObject a);
    GameLog(int w, int h, int ammo, int life, bool border, int i, int j);
    void addMove(Direction direct);
    void addWall(Direction direct, string wall);
};

#endif // GAMELOG_H
