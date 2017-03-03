#include <string>
#include <functional>

#ifndef LABYRINTHOBJECT_H
#define LABYRINTHOBJECT_H

using namespace std;

class LabyrinthObject {
public:
    int const static TYPE_EMPTY = 0;
    int const static TYPE_ITEM = 1;
    int const static TYPE_PLAYER = 2;
    int const static TYPE_TRAP = 3;
    int const static TYPE_TREASURE = 4;

    int i;
    int j;
    int k;
    string name;
    int type;

    virtual string tostring();
    LabyrinthObject ();
    void remove();
};


#endif // LABYRINTHOBJECT_H
