#include "trap.h"
#include "player.h"
#include "labyrinth.h"

string Trap::tostring() {
    return "trap: " + name;
}

void Trap::cought(Player& player, Labyrinth* field){}

Trap::Trap() {
    name = "empty";
    type = TYPE_TRAP;
    trapType = TYPE_EMPTY;
}


void Landmine::cought(Player& player, Labyrinth* field) {
    player.life--;
    player.alive = (player.life > 0);
    remove();
}

Landmine::Landmine() {
    name = "landmine";
    type = TYPE_TRAP;
    trapType = TRAP_TYPE_LAND_MINE;
}
