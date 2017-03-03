#include "labyrinthobject.h"


string LabyrinthObject::tostring() { return name; }

LabyrinthObject::LabyrinthObject() {
    type = TYPE_EMPTY;
    name = "empty";

}

void LabyrinthObject::remove() {
    type = TYPE_EMPTY;
    name = "empty";
}
