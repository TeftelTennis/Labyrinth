#include "item.h"
#include "player.h"
#include "labyrinth.h"
#include "direction.h"
#include <vector>

using namespace std;
string Item::tostring() {
    return "item: " + name;
}

void Item::hitPlayer(Player& player, Labyrinth* field) {}
void Item::useaa(Player& player, Labyrinth* field) {}
bool Item::hitWall(Direction direction) {return false;}

Item::Item() {
    name = "empty";
    itemType = ITEM_TYPE_EMPTY;
    type = TYPE_ITEM;
}


void Bullet::hitPlayer(Player& player, Labyrinth* field) {
    player.life--;
    player.alive = player.life > 0;
}

Bullet::Bullet() {
    itemType = ITEM_TYPE_BULLET;
    name = "bullet";
}

Key::Key() {
    itemType = ITEM_TYPE_KEY;
    name = "key";
}
