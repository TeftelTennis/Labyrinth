#include "labyrinthobject.h"
#ifndef ITEM_H
#define ITEM_H

using namespace std;

class Labyrinth;

class Player;

class Direction;

class Item : public LabyrinthObject {
public:
    int static const ITEM_TYPE_EMPTY = 0;
    int static const ITEM_TYPE_FLOWER = 1;
    int static const ITEM_TYPE_ARMOR_PIERCING_BULLET = 2;
    int static const ITEM_TYPE_BULLET = 3;
    int static const ITEM_TYPE_KEY = 4;

    int itemType;
    string tostring();
    virtual void hitPlayer (Player& player, Labyrinth* field);
    virtual bool hitWall(Direction direction);
    virtual void useaa(Player& player, Labyrinth* field);


    Item();
};

class Bullet : public Item {
public:
    void hitPlayer(Player& player, Labyrinth* field);
    Bullet();
};

class Key : public Item {
public:
    Key();
};


#endif // ITEM_H
