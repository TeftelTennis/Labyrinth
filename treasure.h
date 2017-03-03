#include "labyrinthobject.h"
#ifndef TREASURE_H
#define TREASURE_H

class Treasure : public LabyrinthObject {
public:
    string name;

    string tostring();

    LabyrinthObject* content;

    Treasure();

    Treasure(LabyrinthObject Content);
};

#endif // TREASURE_H
