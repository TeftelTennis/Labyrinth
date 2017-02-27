#ifndef ITEM_H
#define ITEM_H


class Item : LabyrinthObject {
public:
    int ITEM_TYPE_EMPTY = 0;
    int ITEM_TYPE_FLOWER = 1;
    int ITEM_TYPE_ARMOR_PIERCING_BULLET = 2;
    int ITEM_TYPE_BULLET = 3;
    int ITEM_TYPE_KEY = 4;

    int itemType;
    void infoWindow (int windowID);
    void hitPlayer (Player player, Labyrinth field);
    bool hitWall(Direction direction);
    void use(Player player, Labyrinth field);

    //Texture image;

    Item();
};

class ArmorPiercingBullet : Item {
    //TODO:
    ArmorPiercingBullet();
};

class Flower : Item {
    //TODO:
    Flower();
};

class Bullet : Item {
    Bullet();
};

class Key : Item {
    Key();
};


#endif // ITEM_H
