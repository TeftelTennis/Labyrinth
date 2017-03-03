#include "labyrinthobject.h"
#include "item.h"
#include <vector>
#ifndef PLAYER_H
#define PLAYER_H


class Player : public LabyrinthObject {
public:
    vector<Item> items;
    int life;
    bool alive;
    int itemCount(int itemType);
    int itemCount(Item a);
    void deleteItem(int itemType);
    void deleteItem(Item a);
    Player();
    Player(string Name, int Ammo, int playerLife);
    void take(Player corpse);
    int ammo();
    string tostring();
};

#endif // PLAYER_H
