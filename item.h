#include "labyrinthobject.h"
#ifndef ITEM_H
#define ITEM_H

using namespace std;

class Labyrinth;

class Player;

class Direction;

class Item : public LabyrinthObject {
public:
    int ITEM_TYPE_EMPTY = 0;
    int ITEM_TYPE_FLOWER = 1;
    int ITEM_TYPE_ARMOR_PIERCING_BULLET = 2;
    int ITEM_TYPE_BULLET = 3;
    int ITEM_TYPE_KEY = 4;

    int itemType;
    string tostring();
    //virtual void infoWindow (int windowID);
    virtual void hitPlayer (Player player, Labyrinth field);
    virtual bool hitWall(Direction direction);
    virtual void useaa(Player player, Labyrinth field);

    //Texture image;

    Item();
};

class ArmorPiercingBullet : public Item {
public:
    //TODO:
    ArmorPiercingBullet();
};

class Flower : public Item {
public:
    //TODO:
    Flower();
};

class Bullet : public Item {
public:
    void hitPlayer(Player player, Labyrinth field);
    Bullet();
};

class Key : public Item {
public:
    Key();
};


#endif // ITEM_H
