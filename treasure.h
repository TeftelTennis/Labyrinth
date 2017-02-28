#ifndef TREASURE_H
#define TREASURE_H

class Treasure : LabyrinthObject {
    String name : String;
    LabyrinthObject content;

    Treasure(LabyrinthObject Content);
};

#endif // TREASURE_H
