#include "labyrinthobjectfactory.h"
#include "item.h"
#include "trap.h"

LabyrinthObject* LabyrinthObjectFactory::createTrap(int type) {
    if (type == 0) return new Trap();
    if (type == 1) return new Landmine();
    return nullptr;
}

LabyrinthObject* LabyrinthObjectFactory::createItem(int type) {
    if (type == 0) return new Item();
    //if (type == 1) return Flower();
    //if (type == 2) return ArmorPiercingBullet();
    if (type == 3) return new Bullet();
    if (type == 4) return new Key();
    return nullptr;
}
