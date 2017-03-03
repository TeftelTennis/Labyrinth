#include "labyrinthobject.h"

#ifndef LABYRINTHOBJECTFACTORY_H
#define LABYRINTHOBJECTFACTORY_H


class LabyrinthObjectFactory
{
public:
    LabyrinthObject* createTrap(int type);

    LabyrinthObject* createItem(int type);
};

#endif // LABYRINTHOBJECTFACTORY_H
