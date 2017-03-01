#include "labyrinthobject.h"


virtual string tostring() { return name; }

LabyrinthObject::LabyrinthObject() {
    type = TYPE_EMPTY;
    name = "empty";

}

void LabyrinthObject::remove() {
    type = TYPE_EMPTY;
    name = "empty";
}
