#include "labyrinthobject.h"

LabyrinthObject::LabyrinthObject() {
    type = TYPE_EMPTY;
    name = "empty";
    function<string (void)> tostring = []() { return name; };

}

void LabyrinthObject::remove() {
    type = TYPE_EMPTY;
    name = "empty";
    function<string (void)> tostring = []() { return name; };
}
