#include <string>
#include <functional>
#include "treasure.h"

Treasure::Treasure(LabyrinthObject Content) {
    content = Content;
    type = TYPE_TREASURE;
    function<string (void)> toString = []() {
        string res = "Treasure. Conntents : ";
        res += content.tostring() + " ";
        return res;
    };
}
