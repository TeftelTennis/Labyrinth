#include "labyrinthobject.h"
#ifndef TRAP_H
#define TRAP_H

class Labyrinth;

class Player;

class Trap : public LabyrinthObject {
public:
    static const int TRAP_TYPE_EMPTY = 0;
    static const int TRAP_TYPE_LAND_MINE = 1;

    int trapType;
    virtual void cought(Player& player, Labyrinth* field);

    string tostring();

    Trap();
};

//TODO : Traps

class Landmine : public Trap {
public:
    void cought(Player& player, Labyrinth* field);

    Landmine();
};

#endif // TRAP_H
