#include <string>
#include "treasure.h"


string Treasure::tostring() {
    string res = "Treasure. Conntents : ";
    res += content->tostring() + " ";
    return res;
}

Treasure::Treasure() {
    type = TYPE_TREASURE;
}

Treasure::Treasure(LabyrinthObject Content) {
    content = &Content;
    type = TYPE_TREASURE;
}
