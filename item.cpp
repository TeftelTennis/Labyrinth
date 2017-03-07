#include "item.h"
#include "player.h"
#include "labyrinth.h"
#include "direction.h"
#include <vector>

using namespace std;
string Item::tostring() {
    return "item: " + name;
}


//infoWindow = [](int windowID) {
//    GUILayout.Label("--EMPTY--");
//};
void Item::hitPlayer(Player& player, Labyrinth* field) {}
void Item::useaa(Player& player, Labyrinth* field) {}
bool Item::hitWall(Direction direction) {return false;}

Item::Item() {
    //image = new Texture();
    name = "empty";
    itemType = ITEM_TYPE_EMPTY;
    type = TYPE_ITEM;
}

//ArmorPiercingBullet::ArmorPiercingBullet() {
//    super();
//}

//Flower::Flower() {
//    super();
//}

void Bullet::hitPlayer(Player& player, Labyrinth* field) {
    player.life--;
    player.alive = player.life > 0;
}

Bullet::Bullet() {
    //image = AssetDatabase.LoadAssetAtPath("Assets/Textures/18040.jpg", typeof(Texture)) as Texture;
    itemType = ITEM_TYPE_BULLET;
    name = "bullet";
}

Key::Key() {
    itemType = ITEM_TYPE_KEY;
    name = "key";
}
